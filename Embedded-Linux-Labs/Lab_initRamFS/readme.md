# Lab 07: Create Your Own Initramfs and Reach a Working Shell
**Author:** Mostafa (Intake 46)
**Target Hardware:** Raspberry Pi 3B+ (AArch64)

## Project Overview
In this lab,successfully resolved a "Kernel panic - not syncing: No working init found" error by building a custom Initial RAM Filesystem (initramfs) from scratch. Using a statically-linked compilation of BusyBox, created a minimal directory structure, packed it into a `.cpio` archive, and updated the U-Boot bootloader script to load it into memory alongside the Kernel and Device Tree.

## Execution Proof & Deliverables
* **Initramfs Archive:** Successfully generated `mostafa-initramfs.cpio` containing the `/sbin/init` statically linked shell.
* **U-Boot Script:** Updated `boot.scr` to automatically load the initRamfs to RAM address `0x01000000`.
* **Execution:** The Raspberry Pi 3B+ successfully booted to the `/ #` prompt without any SD card Ext4 partitions.
* **Commands Tested:** Successfully ran `ls` and `echo "Hello from Mostafa"`.

---

## U-Boot Configuration (`boot.cmd`)
```text
setenv bootargs "console=tty1 console=ttyS0,115200 rdinit=/bin/sh"

fatload mmc 0:1 0x00080000 Image

fatload mmc 0:1 0x04000000 uInitrd

fatload mmc 0:1 0x05000000 bcm2710-rpi-3-b-plus.dtb

booti 0x00080000 0x04000000 0x05000000
```

---

## Understanding Check Questions

**1. What is initramfs? Why use it instead of mounting the real rootfs directly?**
An initramfs (Initial RAM Filesystem) is a temporary, compressed root filesystem loaded into RAM by the bootloader. It is used to provide the minimal drivers, tools, and scripts needed to mount the "real" physical filesystem (like an Ext4 partition on an SD card or NVMe drive). In embedded systems, it is often used as the permanent filesystem because running purely from RAM is incredibly fast and completely prevents SD card corruption during sudden power loss.

**2. Why cpio format for initramfs? Why not tar or zip?**
The Linux kernel developers specifically chose the `.cpio` format because the algorithm required to unpack it is extremely simple and requires very little code. Putting a massive `.tar` or `.zip` extraction engine directly inside the kernel would unnecessarily bloat the kernel's binary size.

**3. What does `rdinit=` do? What happens if wrong path?**
`rdinit=` (RAM Disk Init) is a kernel command-line parameter that tells the kernel exactly which executable file to run as the very first process (PID 1) out of the initramfs. If the path is wrong or the file is missing, the kernel fails to start user-space, throws the "Kernel panic - not syncing: No working init found" error, and completely halts.

**4. Why must init be statically linked? What if dynamic?**
Our bare-bones initramfs does not have a `/lib` folder populated with heavy shared C libraries (like `glibc`). If the BusyBox `init` program was dynamically linked, it would attempt to find those libraries at boot, fail, and crash immediately. Static linking bundles all the required library code directly into the BusyBox binary itself so it can run entirely standalone.

**5. Difference: initramfs vs initrd?**
* **initrd (Initial RAM Disk):** The older method. It creates a fixed-size, rigid block device in RAM, formats it with a filesystem (like ext2), and requires the kernel to load block device drivers just to read it.
* **initramfs (Initial RAM Filesystem):** The modern method. It is simply a `.cpio` archive that the kernel unpacks directly into its internal page cache (`tmpfs`). It dynamically scales in size, uses memory much more efficiently, and requires zero block device drivers.

**6. Where is initramfs loaded in memory? Who decompresses it?**
The compressed `initramfs` archive is loaded into physical RAM by the bootloader (in our case, U-Boot loaded it to address `0x01000000`). Once U-Boot hands control over to the Linux Kernel, the Kernel itself takes responsibility for decompressing and unpacking the archive into the virtual root filesystem.

**7. How does kernel switch from initramfs to real rootfs?**
Once the initramfs has loaded the necessary drivers and mounted the physical storage drive, it executes a utility called `switch_root` (or `pivot_root`). This command moves the virtual filesystems (`/proc`, `/sys`, `/dev`) to the new physical mount, completely deletes the initramfs from RAM to free up memory, and then executes the "real" `/sbin/init` on the newly mounted physical drive.
```
