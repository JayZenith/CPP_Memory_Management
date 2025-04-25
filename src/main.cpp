#include <iostream>
#include <chrono>
#include "../include/pointer.h"
#include "../include/LeakTester.h"

int main(){
    auto start = std::chrono::high_resolution_clock::now();
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
    
    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double, std::milli> duration = end - start;
    std::cout << "Execution time: " << duration.count() << " ms" << std::endl;
}