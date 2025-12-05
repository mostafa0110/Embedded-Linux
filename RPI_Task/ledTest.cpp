#include "GPIO.hpp"
#include <unistd.h>

int main() {
    Gpio led(529);   
    led.setDirection("out");

    while (1) {
        led.setValue(1);
        usleep(500000);
        led.setValue(0);
        usleep(500000);
    }
}
