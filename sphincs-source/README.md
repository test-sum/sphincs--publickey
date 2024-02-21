# How to setup

Instructions on setting up the environment and building the project, which utilizes the liboqs library for post-quantum cryptography with SPHINCS+(Hash-based DSA).

## Prerequisites

Ensure you have the following installed on your system:
- C++ compiler 
- CMake (version 3.18 or higher)
- Make (or equivalent build system)
- Git

## Environment Setup

1. **Install liboqs**:

   First, clone the liboqs repository and build it:

   ```bash
   git clone --branch main https://github.com/open-quantum-safe/liboqs.git
   cd liboqs
   mkdir build && cd build
   cmake -GNinja ..
   ninja
   sudo ninja install
   ```

2. **Build**:

   ```bash
   g++ -o sphincs_sign src/sign_sphincs.cpp -loqs -std=c++11
   g++ -o sphincs_verify src/verify_sphincs.cpp -loqs -std=c++11
   ```

3. **Run**:

   ```bash
   ./sphincs_sign
   ./sphincs_verify
   ```







