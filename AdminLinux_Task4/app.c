#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <0|1>\n", argv[0]);
        return 1;
    }

    int fd = open("/sys/class/leds/input16::capslock/brightness", O_RDWR);
    if (fd < 0) {
        printf("Failed to open file\n");
        return 1;
    }

    if (argv[1][0] == '0' || argv[1][0] == '1') {
        dprintf(fd, "%c\n", argv[1][0]);
    } else {
        printf("Invalid argument. Use 0 or 1\n");
    }

    close(fd);
    return 0;
}