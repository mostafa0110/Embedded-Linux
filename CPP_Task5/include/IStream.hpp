#ifndef ISTREAM_HPP
#define ISTREAM_HPP

#include "Stream.hpp"


class IStream : virtual public Stream {
public:
    IStream();
    ~IStream() override;

    int readDigit() override;
};

#endif // ISTREAM_HPP