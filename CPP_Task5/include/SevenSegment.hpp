#ifndef SEVENSEGMENT_HPP
#define SEVENSEGMENT_HPP

#include "IStream.hpp"
#include "OStream.hpp"
#include "GPIO.hpp" 
#include <vector>
#include <cstdint>

class SevenSegment : public IStream, public OStream {
private:
    std::vector<Gpio> segments; 
    const uint8_t digitMap[10]; 

public:
    SevenSegment();
    ~SevenSegment() override;

    void writeDigit(int digit) override;
};

#endif // SEVENSEGMENT_HPP