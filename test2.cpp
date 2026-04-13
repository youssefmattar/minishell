#include <iostream>
#include <thread> // For std::this_thread::sleep_for
#include <chrono> // For std::chrono::seconds

int main() {
    int count = 1;

    std::cout << "Starting timer... Press Ctrl+C to stop.\n" << std::endl;

    while (true) {
        std::cout << "Message #" << count << ": Still running..." << std::endl;
        
        // Pause the current thread for exactly 1 second
        std::this_thread::sleep_for(std::chrono::seconds(1));
        
        count++;
    }

    return 0;
}