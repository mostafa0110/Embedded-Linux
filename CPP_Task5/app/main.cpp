#include <iostream>
#include <memory>
#include "SevenSegment.hpp"
#include "TerminalStream.hpp"


void runSystem(Stream& device) {
    std::cout << "\nSystem Running\n";
    
    int val = device.readDigit();
    
    device.writeDigit(val);
}

int main() {
    int mode = 0;
    std::cout << "Select Output Mode:\n1. 7-Segment Hardware\n2. Terminal Simulation\n> ";
    std::cin >> mode;

    if (mode == 1) {
        SevenSegment hw;
        runSystem(hw);
    } else {
        TerminalStream sim;
        runSystem(sim);
    }

    return 0;
}