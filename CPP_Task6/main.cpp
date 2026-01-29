#include <iostream>
#include <stdio.h>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <fstream>

std::mutex mu;
std::condition_variable cv;
int state = -1;

void ReadCapsLockState() {
    std::ifstream file("/sys/class/leds/input2::capslock/brightness");
    int currState = 0;
    while(true){
        file.seekg(0);
 
        if (file >> currState) {
            std::unique_lock<std::mutex> ul(mu);
            if (state != currState) {
                state = currState;
                ul.unlock();
                cv.notify_one();
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}


void Output(){
    std::ofstream file("output.txt", std::ios::trunc);
    if (!file.is_open()) {
        std::cerr << "Failed to open file\n";
        return;
    }
    int last_state = -1;
    while(1){
        std::unique_lock<std::mutex> ul{mu};
        cv.wait(ul , [&] { return state != last_state; });
        last_state = state;
        ul.unlock();
        if (last_state == 1) {
            file << "CapsLock On" << std::endl;
            std::cout << "CapsLock On"; 
        } else {
            file << "CapsLock Off" << std::endl;
            std::cout << "CapsLock Off";
        }
    }
}

int main(int argc, char *argv[]) {
    std::thread th1 (ReadCapsLockState);
    std::thread th2 (Output);
    th1.join();
    th2.join();
    return 0;
}