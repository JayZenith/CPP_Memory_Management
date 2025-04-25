
//class defines element that is stored in the 
//garbage collection information list
template<class T>
class PtrDetails{
public:
    unsigned refCount = 0; // curr reference count
    T *memPtr;             // pionter to allocated memory

    //Assign true if memPtr is pointing to an array
    bool isArray;

    //Assign arraySize if memPtr is pointing to an array
    unsigned arraySize;

    PtrDetails(T *memPtr_, int size) : memPtr(memPtr_), arraySize(size){
        refCount ++;
        if(arraySize > 0){
            isArray = true;
        }
    }

    //Copy Constructor such as PtrDetails ob2 = ob1; implicitly
    // PtrDetails ob3(ob1); explicitly 
    PtrDetails(const PtrDetails& other){
        memPtr = other.memPtr;
        refCount++;
        if(other.arraySize>0){
            isArray = true;
        }
    }
};


//Overloading operator== allows two classes to be compared.
//This is needed by the STL list class
template <class T>
bool operator==(const PtrDetails<T> &obj1,
                const PtrDetails<T> &obj2)
{
    return (obj1.memPtr == obj2.memPtr);
}