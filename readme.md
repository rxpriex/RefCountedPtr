# RxPointer

RxPointer is a custom C++ shared pointer implementation designed to manage the lifetime of dynamically allocated objects. It uses reference counting to track shared ownership, automatically deleting the object when the last reference is destroyed.

## Features
- **Shared Ownership**: Multiple RxPointer instances can share the same object.
- **Automatic Cleanup**: Deletes the managed object when no references remain.
- **Variadic Constructor**: Supports constructing objects with any number of arguments.
- **Lightweight**: Minimal overhead for simple memory management.

## Requirements
- **C++11 or later**: Uses features like variadic templates and perfect forwarding.
- **CMake 3.10 or later**: For building the project.
- **Clang++**: Preferred compiler, installed via MSYS2 MinGW64 (other compilers may work).

## Building the Project
This project uses CMake and is optimized for compilation with Clang++ from MSYS2 MinGW64. Follow these steps:

1. **Install Dependencies**:
   - Install CMake: Download from [cmake.org](https://cmake.org/download/).
   - Install MSYS2 MinGW64 and Clang++:
     ```bash
     pacman -S mingw-w64-x86_64-clang mingw-w64-x86_64-cmake