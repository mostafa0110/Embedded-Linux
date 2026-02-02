#include "gpio.hpp"
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>

// static init
// static as we need one export and unexport fd for all instances
int Gpio::exportFd = -1;
int Gpio::unexportFd = -1;
bool Gpio::staticOpened = false;

Gpio::Gpio(int pin)
{
	pinNumber = pin;
	valueFd = -1;
	dirFd = -1;
    openStaticExportFds();
    exportPin();
    usleep(100000);  // wait 100ms for sysfs to create the gpio pin folder
    openInstanceFds();
}

Gpio::~Gpio() {
    closeInstanceFds();
    unexportPin();
}

void Gpio::openStaticExportFds() {
    if (staticOpened) return;

    exportFd = open("/sys/class/gpio/export", O_WRONLY);
    if (exportFd == -1)
        std::cout << "Failed to open /sys/class/gpio/export:  \n";

    unexportFd = open("/sys/class/gpio/unexport", O_WRONLY);
    if (unexportFd == -1)
        std::cout << "Failed to open /sys/class/gpio/unexport: \n";

    staticOpened = true;
}

void Gpio::exportPin() {
    if (exportFd == -1) {
        std::cout << "exportPin: export fd not available\n";
        return;
    }

    std::string s = std::to_string(pinNumber);
    if (write(exportFd, s.c_str(), s.size()) == -1)
        std::cout << "exportPin failed for pin " << pinNumber << "\n";
}

void Gpio::unexportPin() {
    if (unexportFd == -1) {
        std::cout << "unexportPin: unexport fd not available\n";
        return;
    }

    std::string s = std::to_string(pinNumber);
    if (write(unexportFd, s.c_str(), s.size()) == -1)
        std::cout << "unexportPin failed for pin " << pinNumber <<"\n";
}

void Gpio::openInstanceFds() {
    std::string base = "/sys/class/gpio/gpio" + std::to_string(pinNumber) + "/";
    std::string dirPath = base + "direction";
    std::string valPath = base + "value";

    dirFd = open(dirPath.c_str(), O_WRONLY);
    if (dirFd == -1)
        std::cout << "Failed to open " << dirPath <<"\n";

    valueFd = open(valPath.c_str(), O_RDWR);
    if (valueFd == -1)
        std::cout << "Failed to open " << valPath << "\n";
}

void Gpio::closeInstanceFds() {
    if (valueFd != -1) { close(valueFd); valueFd = -1; }
    if (dirFd != -1)   { close(dirFd);   dirFd = -1; }
}

void Gpio::setDirection(const char* dir) {
    if (dirFd == -1) {
        std::cout << "setDirection: dirFd not open for pin " << pinNumber << "\n";
        return;
    }

    lseek(dirFd, 0, SEEK_SET); 	// reset file offset to handle multiple writes
    if (write(dirFd, dir, strlen(dir)) == -1)
        std::cout << "setDirection failed for pin " << pinNumber << "\n";
}

void Gpio::setValue(int v) {
    if (valueFd == -1) {
        std::cout << "setValue: valueFd not open for pin " << pinNumber << "\n";
        return;
    }

    char c = (v ? '1' : '0');
    lseek(valueFd, 0, SEEK_SET); // reset file offset to handle multiple writes
    if (write(valueFd, &c, 1) == -1)
        std::cout << "setValue failed for pin " << pinNumber << "\n";
}

int Gpio::getValue() {
    if (valueFd == -1) {
        std::cout << "getValue: valueFd not open for pin " << pinNumber << "\n";
        return -1;
    }

    char c = '0';
    lseek(valueFd, 0, SEEK_SET); // reset file offset to handle multiple reads
    if (read(valueFd, &c, 1) == -1) {
        std::cout << "getValue failed for pin " << pinNumber
                  << " : " << strerror(errno) << "\n";
        return -1;
    }

    return (c == '1') ? 1 : 0;
}

// move constructor
Gpio::Gpio(Gpio&& other)  {
    pinNumber = other.pinNumber;
    valueFd   = other.valueFd;
    dirFd     = other.dirFd;

    other.pinNumber = -1;
    other.valueFd = -1;
    other.dirFd = -1;
}

// move assignment
Gpio& Gpio::operator=(Gpio&& other)  {
    if (this != &other) {
        closeInstanceFds();  // close existing fds

        pinNumber = other.pinNumber;	// transfer ownership
        valueFd   = other.valueFd;
        dirFd     = other.dirFd;

        other.pinNumber = -1; 			 
        other.valueFd = -1;
        other.dirFd = -1;
    }
    return *this;
}
