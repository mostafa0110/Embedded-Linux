#ifndef OSTREAM_HPP
#define OSTREAM_HPP

#include "Stream.hpp"

class OStream : virtual public Stream {
public:
    OStream();
    ~OStream() override;

};

#endif // OSTREAM_HPP