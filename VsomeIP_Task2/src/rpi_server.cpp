#include "rpi_server.hpp"
#include <iostream>
#include <csignal>   
#include <condition_variable>
#include <mutex>
#include <atomic>

std::atomic<bool> quit_requested(false);

void handle_signal(int signal) {
    quit_requested = true;
}

int main() {
    std::signal(SIGINT, handle_signal);  // Ctrl+C
    std::signal(SIGTERM, handle_signal); // Kill command

    service srv;
    std::cout << "[Main] Running. Ctrl+C to stop." << std::endl;
    
    while (!quit_requested) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    std::cout << "[Main] Shutdown requested..." << std::endl;
    return 0;
}