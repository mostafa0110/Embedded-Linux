#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include <initializer_list>
#include <fcntl.h>
#include <unistd.h>

class FileActions {
private:
    int* fd;                  // heap pointer
    int*& fdRef;              // reference to the pointer
    std::vector<std::pair<std::string,int>> actions;
    bool owner;               // only the first object deletes it

public:
    FileActions() = delete;

    FileActions(const std::string& path)
        : fd(new int), fdRef(fd), owner(true)
    {
        *fd = open(path.c_str(), O_RDWR | O_CREAT, 0644);
        if (*fd < 0) {
            std::cout << "open failed\n";
        }
    }

    FileActions(const FileActions& other)
        : fd(other.fd), fdRef(fd), actions(other.actions), owner(false)
    {
    }

    void registerActions(std::initializer_list<std::pair<std::string,int>> list) {
        for (auto& a : list) actions.push_back(a);
    }

    void execute() {
        for (auto& a : actions) {
            if (*fd < 0) {
                std::cout << "fd closed\n";
                break;
            }

            if (a.first == "write") {
                dprintf(*fd, "%d\n", a.second);
            }
            else if (a.first == "read") {
                char buf[128]{0};
                int r = read(*fd, buf, sizeof(buf)-1);
                std::cout << "read: " << buf << "\n"; 
            }
            else if (a.first == "close") {
                close(*fd);
                *fd = -1;
            }
        }
        actions.clear();
    }

    ~FileActions() {
        if (owner) {
            if (fd && *fd >= 0) close(*fd);
            delete fd;
        }
    }
};
