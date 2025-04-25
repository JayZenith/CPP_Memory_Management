#include <iostream>
#include <chrono>

int main(){
    auto start = std::chrono::high_resolution_clock::now();
    std::cout << "here" << std::endl;
    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double, std::milli> duration = end - start;
    std::cout << "Execution time: " << duration.count() << " ms" << std::endl;
}