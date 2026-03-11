CC = aarch64-linux-gnu-gcc
LD = aarch64-linux-gnu-ld
OBJCOPY = aarch64-linux-gnu-objcopy

CFLAGS = -mcpu=cortex-a53 -fpic -ffreestanding -std=gnu99 -O2 -Wall
LDFLAGS = -T linker.ld -nostdlib

all: mostafa_blinky.img

startup.o: startup.s
	$(CC) $(CFLAGS) -c startup.s -o startup.o

main.o: main.c
	$(CC) $(CFLAGS) -c main.c -o main.o

mostafa_blinky.elf: startup.o main.o linker.ld
	$(LD) $(LDFLAGS) startup.o main.o -o mostafa_blinky.elf

mostafa_blinky.img: mostafa_blinky.elf
	$(OBJCOPY) -O binary mostafa_blinky.elf mostafa_blinky.img

clean:
	rm -f *.o *.elf *.img