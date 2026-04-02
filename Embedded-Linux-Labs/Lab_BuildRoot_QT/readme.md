# Embedded Qt 6

## Overview
This project is a bare-metal, hardware-accelerated embedded Linux application designed to run on a custom Buildroot operating system for the Raspberry Pi 3B+. It features a modern Qt 6 QML user interface that runs completely without an X11 desktop (using EGLFS) and communicates directly with a custom C++ sysfs GPIO driver to toggle physical hardware.

## Architecture Stack
* **Hardware:** Raspberry Pi 3B+ (AArch64)
* **Operating System:** Custom Linux generated via Buildroot (BusyBox init, no Desktop Manager).
* **Graphics Backend:** Direct Rendering Manager (DRM) / Mesa3D `vc4` drivers.
* **UI Framework:** Qt 6 (Base & Declarative) running via `eglfs`.
* **Hardware Driver:** Custom C++ RAII wrapper for Linux `/sys/class/gpio`.

## File Structure
* `CMakeLists.txt` - Cross-compilation configuration linking Qt6 and the GPIO driver.
* `main.cpp` - Application entry point; initializes EGLFS, loads the local QML file, and injects the C++ backend.
* `Main.qml` - The frontend user interface featuring a QtQuick Switch.
* `LedController.h` - The C++ bridge exposing `Q_INVOKABLE` hardware toggles to the QML engine.
* `GPIO.hpp` & `GPIO.cpp` - The low-level driver managing file descriptors for GPIO export, direction, and value.

## Hardware Wiring
This application is hardcoded to control **BCM GPIO 18**. 
*(Note: Modern Pi kernels utilize a sysfs base offset of 512, making this pin `530` in the driver).*

1. **LED Anode (+):** Connect to a 330Ω resistor, then to **Physical Pin 12** (GPIO 18).
2. **LED Cathode (-):** Connect to **Physical Pin 14** (Ground).

## System Configuration Requirements
To run the Qt GUI without a desktop manager, the Raspberry Pi's hardware GPU must be awake.
Ensure your `/boot/config.txt` has the following lines at the bottom:
```ini
dtoverlay=vc4-fkms-v3d
gpu_mem=256
