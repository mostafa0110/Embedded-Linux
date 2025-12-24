#include "TerminalStream.hpp"
#include <iostream>

TerminalStream::TerminalStream() {}
TerminalStream::~TerminalStream() {}

void TerminalStream::writeDigit(int digit) {
    std::cout << digit << "\n";
}