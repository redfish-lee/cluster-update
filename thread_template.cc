// Create a C++11 thread from the main program

#include <iostream>
#include <thread>

// This function will be called from a thread

void thread_func() {
    std::cout << "Hello, World!" << std::endl;
}

int main() {
    // Launch a thread
    std::thread t1(thread_func);

    // Join the thread with the main thread
    t1.join();
    
    return 0;
}
