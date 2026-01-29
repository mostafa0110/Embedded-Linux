#include "server.hpp"
#include <iostream>
#include <csignal>   
#include <condition_variable>
#include <mutex>

std::mutex term_mutex;
std::condition_variable term_cv;
bool terminated = false;


void handle_signal(int signal) {
    if (signal == SIGINT || signal == SIGTERM) {
        std::cout << "\n[Main] Signal received. Initiating shutdown..." << std::endl;
        
        std::lock_guard<std::mutex> lock(term_mutex);
        terminated = true;
        term_cv.notify_one();
    }
}

int main() {
    std::signal(SIGINT, handle_signal);  // Ctrl+C
    std::signal(SIGTERM, handle_signal); // Kill command

    {
        std::cout << "[Main] Starting Server..." << std::endl;

        service srv; 

        std::cout << "[Main] Server is RUNNING. Press Ctrl+C to stop." << std::endl;

        std::unique_lock<std::mutex> lock(term_mutex);
        term_cv.wait(lock, []{ return terminated; });

        std::cout << "[Main] Exiting scope..." << std::endl;
    }

    std::cout << "[Main] Goodbye!" << std::endl;
    return 0;
}