#include "SevenSegment.hpp"
#include <iostream>

static const int PINS[] = {529, 539, 534, 517, 518, 525, 531};

SevenSegment::SevenSegment() : digitMap{
    0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F
} {
    for (int p : PINS) {
        segments.emplace_back(p);

        segments.back().setDirection("out");
    }
}

SevenSegment::~SevenSegment() {
}

void SevenSegment::writeDigit(int digit) {
    if (digit < 0 || digit > 9) {
        std::cerr << "Invalid digit provided to 7-segment: " << digit << std::endl;
        return;
    }

    uint8_t pattern = digitMap[digit];

    for (int i = 0; i < 7; ++i) {
        bool isOn = (pattern >> i) & 1;
        
        segments[i].setValue(isOn ? 1 : 0);
    }
}