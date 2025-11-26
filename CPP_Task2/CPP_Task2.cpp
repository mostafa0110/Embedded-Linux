#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include <initializer_list>
#include <fcntl.h>
#include <unistd.h>

class FileActions {
private:
    int* fd_;
    int*& fdRef_; 
    std::vector<std::pair<std::string,int>> actions_;

public:
    FileActions() = delete;

    FileActions(std::string& path)
        : fd_(new int), fdRef_(fd_)    
    {
        *fd_ = open(path.c_str(), O_RDWR);
        if (*fd_ < 0) {
            std::cerr << "Failed to open file\n";
        }
        
    }

    FileActions(const FileActions& other)
        : fd_(other.fd_),              // share same pointer
          fdRef_(fd_)                  // reference bound to shared pointer
    {
        actions_ = other.actions_;     // copy actions 
    }

    void registerActions(std::initializer_list<std::pair<std::string,int>> list) {
        for (auto& p : list) {
            actions_.push_back(p);
        }
    }

    void executeActions() {
        for (const auto& a : actions_) {
            if (a.first == "read") {
                char buf[64];
                int n = read(*fd_, buf, a.second);
            } else if (a.first == "write") {
                dprintf(*fd_, "Value = %d\n", a.second);
            } else if (a.first == "close") {
                close(*fd_);
            }
        }
    }

    ~FileActions() {
        if (fd_) {
            close(*fd_);
            delete fd_;
            fd_ = nullptr;
        }
    }
};

int main() {
    std::string path = "/sys/class/leds/input2::capslock/brightness";

    FileActions f1(path);
    f1.registerActions({ {"write", 42}, {"write", 999} });

    FileActions f2 = f1; 

    f2.registerActions({ {"write", 123} });

    f1.executeActions();
    f2.executeActions();
}

