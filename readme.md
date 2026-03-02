# Project Build & Execution Guide

## 1. Building the Project

If the Makefile inside your existing `build/` directory is not working, follow these steps:

1. At the top level of the project, create a fresh `build` directory:
   mkdir build
   cd build

2. Inside `build`, create a directory named `vm_state`:
   mkdir vm_state

3. Generate the Makefile using CMake:
   cmake ..

4. Build the project:
   make

This will create the `vm` executable inside the `build` folder.

## 2. Running Assembly Code

1. Inside the `build` folder, create a file named `test.s` containing the assembly program you want to run.
2. Ensure a `tester_out.txt` file exists (can be empty).

To run your assembly file without entering the interactive VM:
   ./vm --run test.s

To view options or use debugger mode:
   ./vm --help

## 3. Testing the VM Using the Tester

1. Enter into the `sparse_test` directory.
2. Copy the `vm` executable from the `build` directory into `sparse_test` (do not modify `test.s`, `vm_state`, or `tester.cpp` inside this directory).
3. Ensure a `tester_out.txt` file exists in this directory.
4. Create the tester executable:
   g++ tester.cpp -o tester
5. Run the tester:
   ./tester

Provide the required inputs when prompted in the terminal.
