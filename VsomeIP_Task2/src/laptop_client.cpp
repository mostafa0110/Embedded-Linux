#include "laptop_client.hpp"
#include <iostream>
#include <chrono>
#include <csignal>
#include <fstream>

std::atomic<bool> running{true};

void handle_signal(int signal) {
    running = false;
}


int main() {
    std::signal(SIGINT, handle_signal);
    std::ifstream led("/sys/class/leds/input2::capslock/brightness"); 
    if(!led.is_open()){
        std::cerr << "Failed to open CapsLock file" << std::endl;
        return 1;
    }



    {
        std::cout << "[Main] Starting Client..." << std::endl;
        
        client my_client;

        std::this_thread::sleep_for(std::chrono::seconds(1));

        int current_val = 0;
        int last_val = -1; 

        while (running) {
            led.seekg(0, std::ios::beg); 
            
            if (led >> current_val) {
                
                if (current_val != last_val) {
                    std::cout << "[Main] CapsLock Changed to: " << current_val << std::endl;
                    my_client.set_remote_led(current_val == 1);
                    last_val = current_val;
                }
            }
            else {
                led.clear(); 
            }
            
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
        
    }
    led.close();
    std::cout << "[Main] Client Exited." << std::endl;
    return 0;
}