# macOS EPICS System Monitor (sysmon_lab)

## Overview
This project implements an EPICS soft Input/Output Controller (IOC) on macOS. It utilizes an `aSub` (Array Subroutine) record executing custom C code (POSIX APIs) to monitor real-time system metrics such as CPU load, active logical cores, total RAM, and system uptime. The data is then exposed to the network via EPICS Channel Access (CA).

## Features
* **Real-time OS Monitoring:** Interrogates the macOS kernel directly.
* **Array Subroutine (`aSub`):** Custom C implementation for hardware interfacing.
* **Effect Cascade:** Implements the `PP` (Process Passive) flag for synchronized record updates.
* **Trigger Modes:** Supports both Periodic SCAN (2 seconds) and Event-Driven Passive modes.
* **Jitter Analysis:** Includes logged analysis of OS scheduling behavior (`camonitor.log` and `Protokoll.md`).

## Prerequisites
* EPICS Base (Tested on R7.0.10.1)
* macOS environment (Darwin aarch64 / x86_64)

## Build Instructions
1. Ensure your EPICS Base environment variables are correctly set.
2. Compile the IOC from the root directory:
   ```bash
   make clean
   make
