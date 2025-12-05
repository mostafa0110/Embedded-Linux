#ifndef GPIO_HPP
#define GPIO_HPP

#include <string>

class Gpio {
private:
    int pinNumber;
    int valueFd;
    int dirFd;

    static int exportFd;
    static int unexportFd;
    static bool staticOpened;

    static void openStaticExportFds();
    void exportPin();
    void unexportPin();
    void openInstanceFds();
    void closeInstanceFds();

public:
    Gpio(int pin);
    ~Gpio();

    void setDirection(const char* dir);
    void setValue(int v);
    int  getValue();

    // non-copyable
    Gpio(const Gpio&) = delete;
    Gpio& operator=(const Gpio&) = delete;

	// movable
    Gpio(Gpio&& other) ;
    Gpio& operator=(Gpio&& other) ;

};

#endif
