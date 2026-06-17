# Linux System Programming - C Assignments

This repository contains a collection of C programs demonstrating core system programming concepts in Linux/Unix environments. These assignments cover fundamental operating system concepts including file operations, process management, inter-process communication (IPC), and signal handling.

## 📝 Author
**Shahad K**

## 🛠️ Prerequisites
To compile and run these programs, you will need:
* A Linux/Unix-based operating system.
* GCC (GNU Compiler Collection) installed.
* Basic understanding of the Linux terminal and command-line arguments.

## 📂 Repository Contents

The assignments are divided into several key system programming concepts:

### 1. File I/O & Operations
* **AS_01.c (`cp` command implementation):** Implements a custom copy command with a `-p` flag to preserve file permissions using `open`, `read`, `write`, and `fchmod`.
* **AS-02.c (Word Count):** Counts lines, words, and characters (bytes) from standard input or multiple files, similar to the standard `wc` command.

### 2. File Descriptors & Synchronization
* **AS-03.c (I/O Redirection):** Demonstrates the use of `dup()` and `dup2()` system calls to redirect standard output (`stdout`) to a file.
* **AS-04.c (File Locking):** Synchronizes file access between a parent and child process using `fcntl` read/write locks.

### 3. Process Management
* **AS-05.c (Process States):** Creates scenarios to demonstrate Orphan and Zombie process states, checking their status via `/proc/<pid>/status`.
* **AS-06.c (Preventing Zombies):** Avoids zombie processes without blocking the parent by using `waitpid()` with the `WNOHANG` flag.
* **AS-07.c (Executing Commands):** Uses `fork()` and `execvp()` to execute terminal commands passed via command-line arguments from a child process.
* **AS-08.c (Multiple Children):** Creates three separate child processes from a single parent and uses `wait()` to track their exit statuses.

### 4. Inter-Process Communication (IPC)
* **AS-09.c (Single Pipe):** Simulates a single terminal pipe (`|`) between two processes (e.g., `ls | wc`).
* **AS-10.c (Multiple Pipes):** Extends pipe functionality to handle three commands separated by two pipes (e.g., `ls -l | grep pattern | wc -l`).
* **AS-12 (Shared Memory):** * `AS-12_WRITE.c`: Writes an uppercase string to shared memory.
  * `AS-12_READ.c`: Reads the string from shared memory, reverses it, and writes it back.
* **AS-13 (Combined IPC - Pipe, FIFO, SHM):** * `AS-13_P1.c` & `AS-13_P2.c`: Establishes complex communication between three different processes using an unnamed Pipe, a named FIFO, and System V Shared Memory.

### 5. Signal Handling
* **AS-16.c (Signals):** Demonstrates handling system signals (like `SIGINT` and `SIGTSTP`) using the robust `sigaction` structure.

## 🚀 How to Compile and Run

You can compile any of the C files using the standard `gcc` compiler. 

**Standard Compilation:**
```bash
gcc AS-02.c -o as02
