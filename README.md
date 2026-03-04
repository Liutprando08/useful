# Useful

**Useful** is a personal collection of scripts, exercises, network simulations, and code snippets created for study, practice, and solving various programming problems.

## Repository Structure

The repository is organized into several directories, each dedicated to a specific programming language or study tool:

- **`c/`**: Projects, exercises, and scripts developed in C.
- **`codice da studiare/`**: A collection of code snippets, notes, and algorithms useful for learning and general review.
- **`java/`**: Java exercises and applications, organized with a package structure (e.g., `com.pro.esercizi`).
- **`packettracer/`**: Simulation files and network labs created using Cisco Packet Tracer.
- **`python/`**: Scripts, automations, and programs written in Python.
- **`rust/`**: Projects, experiments, and code for learning and testing the Rust language.

## Projects Overview

### C Projects (`c/`)

The C directory contains multiple subdirectories with focused projects:

- **`AstroStream`**: A streaming data processing application featuring:
  - Custom memory allocator implementation
  - Bloom filter for efficient data deduplication
  - Fixed-point arithmetic utilities
  - Linked list data structure
  - TLV (Type-Length-Value) decoding
  - RLE (Run-Length Encoding) compression
  - Thread pool implementation for concurrent processing

- **`editor`**: A terminal-based text editor with:
  - Terminal settings management
  - Raw mode input handling
  - Graphic rendering for the editor interface

- **`allocator`**: Custom memory allocator implementation demonstrating low-level memory management concepts

- **`pthread`**: Multi-threaded programming examples including:
  - Background thread sorting
  - Custom pthread-based allocator

- **`compression`**: RLE compression algorithm implementation

- **`encoding`**: TLV encoding implementation for structured data serialization

- **`filter`**: Data filtering algorithms including bloom filter and binary search

- **`library`**: Utility functions (dice rolling, number mirroring)

- **`liste`**: Linked list implementations (Linux-style and standard)

- **`math`**: Mathematical utilities (fixed-point arithmetic, prime sieve, string length)

- **`pointers`**: Pointer exercises demonstrating dynamic memory and pointer arithmetic

- **`sort`**: Sorting algorithm implementations (QuickSort, InsertionSort)

- **`bitwise`**: Bitwise operator demonstrations (AND, complement, left shift, odd/even checks)

- **`struct`**: C struct usage examples

### Java Projects (`java/`)

- **`oggetto/`**: Object-oriented programming exercises demonstrating class design and object creation

### Python Projects (`python/`)

- **`opencv_adaptive_threshold.py`**: Image processing script using adaptive thresholding
- **`opencv_threshold_binary.py`**: Binary thresholding implementation with OpenCV
- **`calcolatrice/`**: Calculator application
- **`guessing_game/`**: Number guessing game

### Rust Projects (`rust/`)

Projects for learning and experimenting with the Rust programming language, focusing on memory safety and systems programming concepts.

### Network Simulations (`packettracer/`)

- **`classful.pkt`**: Cisco Packet Tracer simulation demonstrating classful IP addressing and network fundamentals

### Study Materials (`codice da studiare/`)

- **`linux-0.01/`**: Linux kernel source code (version 0.01) for studying operating system fundamentals

## Languages and Technologies

This repository contains code written primarily in the following languages:

- C
- Java
- Python
- Rust
- Makefile (for compiling C projects)

## Getting Started

To download the code to your computer for study or testing, clone the repository using Git:

```bash
git clone https://github.com/Liutprando08/useful.git
```

## Building C Projects

Most C projects use Makefile for compilation:

```bash
cd c/AstroStream
make
```

## License

This project is for personal educational purposes.
