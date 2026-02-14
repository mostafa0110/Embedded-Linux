

# Embedded Linux Lab: Creating a Virtual SD Card
---

## 1. Create a 1 GiB Virtual Disk Image

To create a virtual disk image that mimics an SD card, use the `dd` command (Data Duplicator). This utility copies data byte-by-byte from a source to a destination.

**Command:**
```bash
dd if=/dev/zero of=sd.img bs=1M count=1024

```

**Explanation:**

* `if=/dev/zero`: **Input File.** Reads from a special file that provides an infinite stream bytes.
* `of=sd.img`: **Output File.** The name of the file being created (the virtual SD card).
* `bs=1M`: **Block Size.** Reads and writes 1 Megabyte at a time.
* `count=1024`: **Count.** The number of blocks to copy (1024 * 1M = 1024 MB = 1 GiB).

**Output:**
<img width="745" height="83" alt="ddCMD" src="https://github.com/user-attachments/assets/3ef775a6-9192-4a1a-a76e-3c8383acd9fe" />
---

## 2. DOS/MBR vs. GPT Partition Schemes

| Feature | DOS/MBR (Master Boot Record) | GPT (GUID Partition Table) |
| --- | --- | --- |
| **Structure** | Stores partition info in the first 512 bytes (MBR). | Stores info across multiple sectors; keeps a backup at the end of the disk. |
| **Max Disk Size** | 2 TB (Terabytes). | 9.4 ZB (Zettabytes). |
| **Partitions** | Max **4 Primary** partitions. | Max **128 Primary** partitions by default. |
| **Usage** | Standard for older BIOS systems and embedded boards (e.g., Raspberry Pi). | Standard for modern UEFI systems and large servers. |

---

## 3. File Systems: Usage and Differences

* **FAT16 (File Allocation Table 16-bit):**
* **Usage:** Very small embedded storage or legacy systems.
* **Difference:** Maximum volume size is 2GB. Does *not* support Linux permissions.

* **FAT32 (File Allocation Table 32-bit):**
* **Usage:** The standard boot partition format for many embedded boards (like RPi) and USB drives.
* **Difference:** Compatible with Windows/Linux/Mac. Max file size is 4GB. Does *not* support Linux permissions.


* **EXT4 (Fourth Extended Filesystem):**
* **Usage:** The standard Linux root filesystem.
* **Difference:** Supports Linux permissions (ownership/groups), journaling (prevents corruption), and files larger than 4GB. Not natively readable by Windows.



---

## 4. Formatting and Partitioning the Virtual Disk Image

use `cfdisk` to create the partition table.

**Requirements:**

1. **Partition 1:** Primary, 200MB, Bootable, FAT16.
2. **Partition 2:** Primary, Rest of space, EXT4 (Linux).

**Output:**
<img width="745" height="456" alt="cfdisk" src="https://github.com/user-attachments/assets/8140c8da-81ca-46b8-904c-3c94592107c6" />
---

## 5. Loop Devices in Linux

**Definition:**
A **Loop Device** is a pseudo-device that makes a regular file accessible as a block device.

**Why Linux uses them:**
Filesystems usually live on physical block devices (like hard drives). Loop devices allow the OS to mount a file (like `sd.img`) as if it were a physical disk, allowing format, partition, and modify the files inside it.

* **a. Command to create a loop device:**
```bash
sudo losetup -f --partscan --show sd.img

```


* **b. Command to list all loop devices currently in use:**
```bash
lsblk

```


* **c. Command to detach a loop device:**
```bash
sudo losetup -d /dev/loopX

```



---

## 6. How to check the current loop device limit?

check the limit defined in the kernel parameters by reading the max_loop file.
**Command:**

```bash
cat /sys/module/loop/parameters/max_loop
```

---

## 7. Can you expand the number of loop devices in Linux?

**Yes.**
 increase it by passing the `max_loop` parameter when loading the kernel module.

**Command:**

```bash
sudo modprobe loop max_loop=64

```

Alternatively, add `max_loop=64` to the GRUB boot parameters.

---

## 8. Attach the Virtual Disk Image as a Loop Device

use `losetup` to attach the image.

**Command:**

```bash
sudo losetup -f --partscan --show sd.img

```

* `-f`: Find the first unused loop device.
* `--show`: Print the name of the assigned loop device (e.g., `/dev/loop0`).

**Output:**
<img width="745" height="49" alt="losetup" src="https://github.com/user-attachments/assets/ae6f88ff-4797-486a-b6bf-4416b9b7441a" />
---

## 9. Format the Virtual Disk Image Partitions

apply the filesystems to the partitions created by the loop device (assuming `/dev/loop0`).

**a. First Partition (Boot → FAT16):**

```bash
sudo mkfs.vfat -F 16 -n "boot" /dev/loop19p1

```

* `mkfs.vfat`: Creates a FAT filesystem.
* `-F 16`: Specifies FAT16.
* `-n "boot"`: Sets the volume label/name to "boot".

**b. Second Partition (Rootfs → EXT4):**

```bash
sudo mkfs.ext4 -L "rootfs" /dev/loop19p2

```

* `mkfs.ext4`: Creates an EXT4 filesystem.
* `-L "rootfs"`: Sets the volume label/name to "rootfs".

**Output:**
<img width="745" height="304" alt="mkfs" src="https://github.com/user-attachments/assets/180f58d8-7d18-48af-ab2a-267adda3ce9b" />

<img width="745" height="71" alt="lsblk" src="https://github.com/user-attachments/assets/79ec2610-8691-42b6-a45d-435466a3a019" />
---

## 10. The "mount" and "umount" Commands

* **mount:**
This command instructs the Operating System to make a filesystem (located on a device) accessible at a specific directory (the mount point). It attaches the device to the file tree.
* *Usage:* `mount [device] [directory]`


* **umount:**
This command detaches the filesystem from the file tree. Crucially, it syncs any cached data to the disk to prevent data corruption before the device is removed.
* *Usage:* `umount [directory]`



---

## 11. Block Device vs. Character Device

| Feature | Block Device | Character Device |
| --- | --- | --- |
| **Data Access** | Reads/Writes in fixed-size blocks (e.g., 512 bytes). | Reads/Writes as a stream of characters (bytes). |
| **Access Type** | Random Access (can jump to any location). | Sequential Access (must read in order). |
| **Buffering** | Buffered by the OS for performance. | Unbuffered (direct access). |
| **Examples** | Hard Drives, SD Cards, Loop Devices. | Keyboards, Mice, Serial Ports, Sound Cards. |

---

## 12. Create Mount Points and Mount the Virtual Disk Image

We create directories to serve as mount points and then mount the partitions.

**Commands:**

```bash
# Create directories
mkdir boot
mkdir rootfs

# Mount Boot Partition (FAT16)
sudo mount /dev/loop19p1 boot

# Mount Root Partition (EXT4)
sudo mount /dev/loop19p2 rootfs

```


**Output:**
<img width="745" height="71" alt="lsblk2" src="https://github.com/user-attachments/assets/7183b1b3-39df-428c-8726-be3364a11601" />
