#ifndef STREAM_HPP
#define STREAM_HPP

class Stream {
public:
    Stream();
    virtual ~Stream(); 


    virtual int readDigit() = 0;
    virtual void writeDigit(int digit) = 0;
};

#endif // STREAM_HPP