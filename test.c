#include <iostream>
#include <string>

int main() {
    std::string input;
    std::cout << "Waiting for data..." << std::endl;
    
    // This normally waits for the keyboard
    while (std::getline(std::cin, input)) {
        std::cout << "Program received: [" << input << "]" << std::endl;
    }

    std::cout << "--- End of Input ---" << std::endl;
    return 0;
}
