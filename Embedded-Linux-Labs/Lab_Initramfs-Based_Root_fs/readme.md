# Task: Initramfs-Based Root Filesystem Selection (A/B Boot Selector)
**Author:** Mostafa (Intake 46)
**Target Hardware:** Raspberry Pi 3B+ (AArch64)

## Objective Achieved
Designed and implemented an initramfs that acts as a primary boot manager, allowing the user to select between two distinct Ext4 root filesystems located on the physical SD card.

## System Flow & Implementation
1. **Physical Storage:** Partitioned the SD card into `BOOT` (FAT32), `ROOT1` (Ext4), and `ROOT2` (Ext4). Populated both root partitions with a statically compiled BusyBox rootfs and a unique `system_status.txt` flag.
2. **Initramfs Brain (`/init`):** Developed a custom shell script to mount `proc`, `sys`, and `devtmpfs`, wait for the MMC block devices to initialize, and present an interactive selection menu via the console.
3. **The Handoff:** Used `mount --move` to transfer virtual filesystems to the selected physical partition's mount point (`/newroot`), followed by `exec switch_root /newroot /sbin/init` to successfully pivot PID 1 to the hardware drive.
4. **Bootloader:** Wrapped the custom initramfs using `mkimage` to create a `uInitrd` and configured U-Boot's `bootargs` to execute `rdinit=/init`.
