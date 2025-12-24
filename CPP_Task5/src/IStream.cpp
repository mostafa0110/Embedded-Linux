#include "IStream.hpp"
#include <iostream>
#include <limits>

IStream::IStream() {}
IStream::~IStream() {}

int IStream::readDigit() {
    int input = -1;
    while (true) {
        std::cout << "Enter a single digit (0-9): ";
        if (std::cin >> input) {
            if (input >= 0 && input <= 9) {
                return input;
            }
            std::cout << "Invalid. Must be 0-9.\n";
        }
    }
}