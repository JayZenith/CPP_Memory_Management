# Memory Management/Garbage-Collector`

The goal of this project is to learn and apply memory management concepts in C++ by creating a custom Smart Pointer -- in other words, to create a garbage collector.

# Basic Build Instructions
1. Configure Cmake with running `cmake ..` from the build directory
2. build the program by running `cmake --build .` in the build directory
3. then run `./main`

# The following tests are run from main.cpp and LeakTester.h

```c++
#include "gc_pointer.h"
#include "LeakTester.h"

int main()
{
    // Test Pointer's Initialization Constructor
    Pointer<int> p = new int(19);

    // Test Pointer's Overload assignment of pointer to Pointer
    p = new int(21);
    p = new int(28);

    // Test Pointer's Overload assignment of Pointer to Pointer.
    Pointer<float> pPoint = new float(3);
    Pointer<float> pCopy;
    pCopy = pPoint;

    // Test Pointer's Copy Constructor
    Pointer<float> pOrig = new float(5);
    Pointer<float> pCopyCon = pOrig;

    // Test Pointer's Array
    Pointer<float, 2> pArr = new float[2]{3,4};

    return 0;
}
```

# Result

```
+---------------+
| FINAL REPORT: |
+---------------+

Total number of allocations: 13
Total number of deallocations: 13
Total number of allocations in bytes: 308
Total number of deallocations in bytes: 308
Maximum memory occupation during runtime in bytes: 268
Memory occupation upon completion: 0


GREAT JOB! YOU DO NOT HAVE MEMORY LEAKAGE
```