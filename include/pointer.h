#include <list>
#include <typeinfo>
#include <cstdlib>
#include "details.h"
#include "iterator.h"


/*
    Pointer implements a pointer type that uses
    garbage collection to release unused memory.
    A Pointer must only be used to point to memory
    that was dynamically allocated using new.
    When used to refer to an allocated array,
    specify the array size.
*/
template <class T, int size = 0>
class Pointer{
private:
    // refContainer maintains the garbage collection list.
    // static that all Pointer objects will reference the same exact instance
    // of refContainer.
    static std::list<PtrDetails<T>> refContainer;

    // addr points to the allocated memory to which
    // this Pointer pointer currently points.
    T *addr;

    // true if pointing to array
    bool isArray;

    // size of the array, if we are pointing to an array
    unsigned arraySize;

    // true when first Pointer is created
    static bool first;

    // Return an iterator to PtrDetails in refContainer.
    typename std::list<PtrDetails<T>>::iterator findPtrInfo(T *ptr);

public:
    // Define an iterator type for Pointer<T>.
    typedef Iter<T> GCiterator;

    // Empty constructor
    // NOTE: templates aren't able to have prototypes with default arguments
    // which is why constructor is designed like this:
    Pointer() { Pointer(NULL); }
    // want we want ultimately just want this
    Pointer(T *);

    // Copy constructor.
    Pointer(const Pointer &);

    // Destructor for Pointer.
    ~Pointer();

    // Collect garbage. Returns true if at least
    // one object was freed.
    static bool collect();

    // Overload assignment of pointer to Pointer.
    T *operator=(T *t);

    // Overload assignment of Pointer to Pointer.
    Pointer &operator=(Pointer &rv);

    // Return a reference to the object pointed
    // to by this Pointer.
    // This overloaded operator is like a proxy
    T &operator*() { return *addr; }

    // Return the address being pointed to.
    T *operator->() { return addr; }

    // Return a reference to the object at the
    // index specified by i.
    T &operator[](int i) { return addr[i]; }

    // Conversion function to T *.
    operator T *() { return addr; }

    // Return an Iter to the start of the allocated memory.
    Iter<T> begin()
    {
        int _size;
        if (isArray)
            _size = arraySize;
        else
            _size = 1;
        return Iter<T>(addr, addr, addr + _size);
    }

    // Return an Iter to one past the end of an allocated array.
    Iter<T> end()
    {
        int _size;
        if (isArray)
            _size = arraySize;
        else
            _size = 1;
        return Iter<T>(addr + _size, addr, addr + _size);
    }

    // Return the size of refContainer for this type of Pointer.
    static int refContainerSize() { return refContainer.size(); }

    // A utility function that displays refContainer.
    static void showlist();

    // Clear refContainer when program exits.
    static void shutdown();

};

    // STATIC INITIALIZATION
    // Creates storage for the static variables
    template <class T, int size>
    std::list<PtrDetails<T>> Pointer<T, size>::refContainer;

    template <class T, int size>
    bool Pointer<T, size>::first = true;


    // Constructor for both initialized and uninitialized objects.
    template <class T, int size>
    Pointer<T, size>::Pointer(T *t)
    {
        addr = t;
        // Register shutdown() as an exit function.
        if (first)
            atexit(shutdown);
        first = false;

        if (size)
        {
            arraySize = size;
            isArray = true;
        }
        else
            isArray = false;

        typename std::list<PtrDetails<T>>::iterator p;
        p = findPtrInfo(t);
        PtrDetails<T> pd(t, size);
        refContainer.emplace_back(pd);
        p = refContainer.end();
    }

    // Copy constructor.
    template <class T, int size>
    Pointer<T, size>::Pointer(const Pointer &ob)
    {
        addr = ob.addr;

        typename std::list<PtrDetails<T>>::iterator p;
        p = findPtrInfo(ob.addr);

        // increment ref count
        if (p->memPtr == ob.addr)
        {
            // update PtrDetails<T>'s refCount
            (p->refCount)++;
        }
        else
        {
            // add a new PtrDetails instance to refCounter.
            PtrDetails<T> pd(ob.addr, size);
            refContainer.emplace_back(pd);
        }
    }

    // Destructor for Pointer.
    template <class T, int size>
    Pointer<T, size>::~Pointer()
    {
        typename std::list<PtrDetails<T>>::iterator p;
        p = findPtrInfo(addr);

        // decrement ref count
        (p->refCount)--;

        // Collect garbage when a pointer goes out of scope.
        // TIP: For real use, you might want to collect unused memory less frequently,
        // such as after refContainer has reached a certain size, after a certain number of Pointers have gone out of scope,
        // or when memory is low.
        collect();
    }


    // Collect garbage. Returns true if at least
    // one object was freed.
    template <class T, int size>
    bool Pointer<T, size>::collect()
    {
        bool memfreed = false;
        typename std::list<PtrDetails<T>>::iterator p;
        do
        {
            // Scan refContainer looking for unreferenced pointers.
            for (p = refContainer.begin(); p != refContainer.end(); p++)
            {
                // If the memory is still in-use, continue through loop.
                if (p->refCount > 0)
                    continue;

                if (p->memPtr)
                {
                    if (p->isArray)
                        delete[] p->memPtr;
                    else
                        delete p->memPtr;
                    memfreed = true;
                }
                refContainer.erase(p);
                break;
            }
        } while (p != refContainer.end());
        return memfreed;
    }

    // Overload assignment of pointer to Pointer.
    template <class T, int size>
    T *Pointer<T, size>::operator=(T *t)
    {
        /* 
        Example:
            Pointer<int> p = new int(19);
            p = new int(21);
        */

        // find the current PtrDetails that contains this' addr
        typename std::list<PtrDetails<T>>::iterator p;
        p = findPtrInfo(addr);
        (p->refCount)--;

        p = findPtrInfo(t);
        if(p!=refContainer.end()){
            p->refCount++;
        } else {
            // Add a new PtrDeatils to refContainer
            PtrDetails<T> pd(t, size);
            refContainer.push_back(pd);
        }
        addr = t;
        return addr;
    }

    // Overload assignment of Pointer to Pointer.
    template <class T, int size>
    Pointer<T, size> &Pointer<T, size>::operator=(Pointer &rv)
    {
        typename std::list<PtrDetails<T>>::iterator p;
        p = findPtrInfo(addr);
        (p->refCount)--;

        p = findPtrInfo(rv.addr);
        if(p!=refContainer.end()){
            p->refCount++;
        } else {
            // Add a new PtrDeatils to refContainer
            PtrDetails<T> pd(rv, size);
            refContainer.push_back(pd);
        }    
        return rv;
    }

    // Display refContainer.
    template <class T, int size>
    void Pointer<T, size>::showlist()
    {
        typename std::list<PtrDetails<T>>::iterator p;
        std::cout << "--------------------\n";
        std::cout << "refContainer<" << typeid(T).name() << ", " << size << ">:\n\n";
        std::cout << "memPtr refcount value\n";
        if (refContainer.begin() == refContainer.end())
        {
            std::cout << "-->Container is empty!\n\n ";
        }

        // Else, refContainer is full of data
        for (p = refContainer.begin(); p != refContainer.end(); p++)
        {
            // point to the memory address that memPtr is holding,
            // then dereference to get the value at that address
            std::cout << "address=" << (void *)p->memPtr
                    << " refcount=" << p->refcount << " ";

            // If p does points to a valid memory address, then derefernce it
            if (p->memPtr)
                std::cout << " value=" << *p->memPtr;
            else
                std::cout << "value=None";
            std::cout << std::endl;
        }
        std::cout << std::endl;
        std::cout << "--------------------\n";
    }


    // Find a pointer in refContainer.
    template <class T, int size>
    typename std::list<PtrDetails<T>>::iterator
    Pointer<T, size>::findPtrInfo(T *ptr)
    {
        typename std::list<PtrDetails<T>>::iterator p;
        // Maybe find ptr in refContainer.
        for (p = refContainer.begin(); p != refContainer.end(); p++)
            if (p->memPtr == ptr)
                return p;
        // Else, return an unassigned p of type iterator
        return p;
    }

    // Clear refContainer when program exits.
    template <class T, int size>
    void Pointer<T, size>::shutdown()
    {
        if (refContainerSize() == 0)
            return; // list is empty
        typename std::list<PtrDetails<T>>::iterator p;
        for (p = refContainer.begin(); p != refContainer.end(); p++)
        {
            // Set all reference counts to zero
            p->refCount = 0;
        }
        collect();
    }