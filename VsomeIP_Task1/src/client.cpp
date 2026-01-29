#include "client.hpp"
#include <iostream>
#include <chrono>
#include <csignal>

std::atomic<bool> running{true};

void handle_signal(int signal) {
    running = false;
}

int main() {
    std::signal(SIGINT, handle_signal);

    {
        std::cout << "[Main] Starting Client..." << std::endl;
        
        client my_client;

        std::this_thread::sleep_for(std::chrono::seconds(1));

        bool state = true;

        while (running) {
            std::cout << "[Main] Toggling CapsLock..." << std::endl;
            
            // Call the public method
            my_client.set_remote_caps_lock(state);
            
            // Toggle state
            state = !state;

            // Sleep 2 seconds
            for(int i=0; i<50 && running; i++) {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
        }
        
    }

    std::cout << "[Main] Client Exited." << std::endl;
    return 0;
}