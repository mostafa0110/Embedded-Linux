#ifndef TERMINALSTREAM_HPP
#define TERMINALSTREAM_HPP

#include "IStream.hpp"
#include "OStream.hpp"

class TerminalStream : public IStream, public OStream {
public:
    TerminalStream();
    ~TerminalStream() override;

    void writeDigit(int digit) override;
};

#endif // TERMINALSTREAM_HPP