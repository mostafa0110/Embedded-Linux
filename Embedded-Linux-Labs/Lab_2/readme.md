

# Embedded Linux Lab 2: U-Boot Build and Customization


## Part A: U-Boot Build and Deployment

### 1. What is a Bootloader?

A bootloader is a small piece of software that runs after the computer is powered on (after bootRom , BIOS). Its primary responsibility is to initialize the essential hardware (CPU, RAM, Clocks, Storage) and load the Kernel into DRAM.

### 2. Raspberry Pi vs. PC Boot Chain

#### **Raspberry Pi 3B+ Boot Chain (The GPU Master):**

Unlike a PC, the Raspberry Pi's CPU (ARM) is turned **OFF** when power is applied. The GPU (VideoCore IV) is in charge.

1. **Power On:** The GPU core wakes up.
2. **BootROM (First Stage):** The GPU executes the code burned inside its internal ROM. It searches for `bootcode.bin` on the SD card (FAT32 partition).
3. **Bootcode.bin (Second Stage):** Loaded into the L2 Cache. It enables the SDRAM (RAM) and loads the next file.
4. **Start.elf (Third Stage):** The GPU firmware. It reads `config.txt` (hardware settings) and `cmdline.txt`.
5. **Kernel/U-Boot:** The GPU loads the user kernel / U-Boot into RAM and finally **wakes up the ARM CPU** to execute it.

```text
+---------------------+
|      Power ON       |
+----------+----------+
           |
           v
+---------------------+
|    GPU (VideoCore)  | <--- GPU is ON, ARM CPU is OFF
|   Executes BootROM  |
+----------+----------+
           |
           | (Reads bootcode.bin from SD Card)
           v
+---------------------+
|      L2 Cache       |
| (Loads bootcode.bin)|
+----------+----------+
           |
           | (Enables SDRAM, Loads start.elf)
           v
+---------------------+
|      Main RAM       |
| (GPU Firmware Runs) |
+----------+----------+
           |
           | (Reads config.txt, cmdline.txt, bcm2710*.dtb)
           | (Loads kernel8.img / u-boot.bin to RAM)
           v
+---------------------+
|   ARM Cortex-A53    | <--- GPU wakes up the CPU
| (CPU Starts U-Boot) |
+----------+----------+
           |
           v
+---------------------+
|    U-Boot Prompt    |
+---------------------+

```

---
#### PC Boot Chain (x86)

1. **Power On & Core 0 Reset:** Upon power-up, the Chipset holds the CPU in reset. Once power is stable, **Core 0** wakes up. It is the only active core (others are sleeping).
2. **BootROM / Reset Handler:** Core 0 reads the **Reset Vector** (usually at address `0xFFFFFFF0`). It executes the **Reset Handler** from the internal BootROM.
* **Tasks:** Initializes the **Stack Pointer (SP)**, sets up the initial **Clock**, and prepares the CPU to fetch the main firmware.


3. **Jump to BIOS/UEFI:** The CPU jumps to the memory-mapped Flash chip to execute the BIOS/UEFI code.
4. **Hardware Initialization (POST):** The BIOS performs the **Power-On Self-Test (POST)**.
* **Critical Tasks:** Initializes **DRAM** (including **DRAM Calibration/Training**), enables the **Keyboard**, **USB Controller**, and **Ethernet** interface.


5. **Bootloader (GRUB):** The BIOS reads the boot sector (MBR or EFI) from the storage device and loads the Bootloader (GRUB) into the now-calibrated RAM.
6. **Kernel & Init:** GRUB loads the Linux Kernel (`vmlinuz`) and the RAM Disk (`initrd`). The Kernel initializes the OS services and finally runs the **Init Process** (PID 1), which starts the user space.

```text
+---------------------+
|      Power ON       |
+----------+----------+
           |
           v
+---------------------+
|   Core 0 Wake Up    |
| (Reset Vector Read) |
+----------+----------+
           |
           | (Locates Reset Handler)
           v
+---------------------+
| BootROM / Reset Hdlr|
| (Init SP & Clock)   |
+----------+----------+
           |
           | (Jumps to Flash Memory)
           v
+---------------------+
|     BIOS / UEFI     |
| (Init DRAM, USB, KBD)|
| (DRAM Calibration)  |
+----------+----------+
           |
           | (Loads MBR or EFI binary)
           v
+---------------------+
|    Bootloader       |
|      (GRUB)         |
+----------+----------+
           |
           | (Loads Kernel to RAM)
           v
+---------------------+
|    Linux Kernel     |
+----------+----------+
           |
           | (Starts PID 1)
           v
+---------------------+
|    Init Process     |
| (Systemd / SysVinit)|
+---------------------+

```
---

### 3. Difference Between U-Boot and GRUB

| Feature | U-Boot (Universal Bootloader) | GRUB (Grand Unified Bootloader) |
| --- | --- | --- |
| **Primary Usage** | Embedded Systems (ARM, MIPS, RISC-V). | Desktop/Server (x86, PowerPC). |
| **Complexity** | Lightweight, designed for specific hardware. | Heavy, designed for flexibility and user interaction. |
| **Hardware Init** | Performs heavy low-level hardware initialization (DDR, Clocks). | Relies on BIOS/UEFI to initialize hardware before it runs. |
| **Configuration** | Static (recompiled for each board) / parse the .dtb file | Dynamic (reads `grub.cfg` at runtime). |

---

### 4. Required Files for Raspberry Pi Booting

To boot U-Boot on RPi 3B+, these files must be in the FAT32 "boot" partition:

1. **bootcode.bin:** The 2nd stage bootloader; initializes RAM.
2. **start.elf:** The GPU firmware; handles the hardware abstraction.
3. **fixup.dat:** Configures the SDRAM partitions between CPU and GPU.
4. **bcm2710-rpi-3-b-plus.dtb:** The Device Tree Blob. Describes the hardware (GPIOs, Wi-Fi, etc.) to the kernel.
5. **config.txt:** System configuration file (like BIOS settings). Controls clock speeds and tells the GPU which kernel file to load.
6. **kernel8.img / U-Boot** .

---

### 5. Build and Test Custom U-Boot in QEMU (Cortex-A9)

**a. Build Steps:**

1. **Configure:** `make vexpress_ca9x4_defconfig` (Sets up for the VExpress board).
2. **Customize:** `make menuconfig`


3. **Build:** `make -j4`

**b. Run in QEMU:**
Command used:

```bash
qemu-system-arm -M vexpress-a9 -nographic -kernel u-boot

```

* `-M vexpress-a9`: Emulate the Versatile Express board.
* `-nographic`: Output to the terminal instead of a window.
* `-kernel u-boot`: Run the compiled u-boot file.

  **(Output Screenshot)**

<img width="951" height="386" alt="PA-Q5" src="https://github.com/user-attachments/assets/1a7428d6-96c0-4776-9668-348e3f028ada" />
---

### 6. Build and Deploy U-Boot on Real Raspberry Pi 3B+

**a. Build Steps:**

1. **Export Compiler:** `export CROSS_COMPILE=~/x-tools/aarch64-rpi3-linux-gnu/bin/aarch64-rpi3-linux-gnu-`
2. **Configure:** `make rpi_3_b_plus_defconfig`
3. **Customize:** `make menuconfig`
4. **Build:** `make -j4`

**b. SD Card Preparation:**

1. Mount the FAT32 boot partition.
2. Copy the firmware files (`bootcode.bin`, `start.elf`, `fixup.dat` ,`u-boot.bin`) to the SD card.

---

### 7. Hardware Description File

The file is **`bcm2710-rpi-3-b-plus.dtb`**.

* **Stage Loaded:** It is usually loaded by the **start.elf** into RAM before the kernel/U-Boot starts. The firmware passes the memory address of the DTB to U-Boot via a CPU register (typically `r2` or `x0`).

---

### 8. How Linux knows Partition offsets (Loop Devices)

Run `losetup --partscan`, the Linux kernel reads the **Master Boot Record (MBR)** located in the first 512 bytes of the image file (`sd.img`).
This table contains the exact **Start Sector** and **End Sector** for every partition. The kernel reads this and creates the `/dev/loopXpY` block devices mapped to those specific byte ranges within the file.

---

## Part B: U-Boot Commands Environment

### 1. The `bdinfo` Command

Prints **Board Information**. It displays critical global data structures like:

* DRAM start address and size.
* Relocation address (where U-Boot copied itself in RAM).
* CPU frequency.
* Boot arguments location.

**(Screenshot of bdinfo)**
<img width="951" height="711" alt="PB-Q1" src="https://github.com/user-attachments/assets/8a174d82-6de2-4e88-b2f1-f0c37eb8672e" />

---
### 2. The `printenv` Command

Lists all **Environment Variables**. These are persistent settings stored in U-Boot (like `bootdelay`, `bootcmd`, `serverip`) that control how the system boots.

**(Screenshot of printenv)**
<img width="951" height="711" alt="PB-Q2" src="https://github.com/user-attachments/assets/ab55aa0d-08e6-4881-a17a-73d4de3d7c67" />

---
### 3. DRAM Start Address

Based on the `bdinfo` output for vexpress-a9

<img width="939" height="119" alt="PB-Q3" src="https://github.com/user-attachments/assets/de614e59-3c91-4003-b8ea-da39df79cbae" />

---
### 4. List and Load Files from FAT Partition

**List files (`fatls`):**

```bash
fatls mmc 0:1

```

*(Lists files on MMC device 0, Partition 1).*

**Load file (`fatload`):**

```bash
fatload mmc 0:1 0x60000000 test.txt

```

*(Loads `test.txt` from MMC 0:1 into RAM address `0x60000000`).*

**(Screenshot)**
<img width="939" height="133" alt="PB" src="https://github.com/user-attachments/assets/41f4c04f-f9f0-4867-b753-772a6851d932" />
<img width="939" height="462" alt="PB-Q4" src="https://github.com/user-attachments/assets/3cd801e2-3a02-4a07-a9ce-4a11dd967b94" />


---

### 5. Change U-Boot Banner


**(Screenshot of new banner)**
<img width="939" height="441" alt="PB-Q5" src="https://github.com/user-attachments/assets/1507c427-ccab-49d0-8229-d3f92b25859f" />

---

### 6. Add Custom Command `hello`

I created a new file `cmd/hello.c` and registered it using the `U_BOOT_CMD` macro.

**File Content (`cmd/hello.c`):**

```c
#include <common.h>
#include <command.h>

static int do_hello(struct cmd_tbl *cmdtp, int flag, int argc, char *const argv[])
{
    printf("Hello Mostafa, Welcome to Intake 46!\n");
    return 0;
}

U_BOOT_CMD(
    hello,      // Command Name
    1,          // Max args
    1,          // Repeatable
    do_hello,   // Function to execute
    "Prints a hello message",  // Short help
    "Usage: hello\nPrints a personalized greeting." // Long help
);

```

**Register in Makefile (`cmd/Makefile`):**
Added the line:

```makefile
obj-y += hello.o

```

**Test:**
Run `make` and start QEMU. Type `hello` at the prompt.

**(Screenshot of hello command)**

<img width="939" height="441" alt="PB-Q6" src="https://github.com/user-attachments/assets/0d6481a1-0e08-442c-b046-e89bf14eb18b" />


