# Concurrent Gradient Descent

## Overview
This repository contains a C++ implementation of the Gradient Descent optimization algorithm. It features both sequential and parallel execution models to demonstrate the performance benefits of multithreading in mathematical optimization. The concurrent version is built using **Intel's oneAPI Threading Building Blocks (oneTBB)**.

## Key Features
* **Parallel Optimization:** Compares a linear gradient descent approach with a multithreaded implementation.
* **oneTBB Integration:** Utilizes oneTBB's `task_arena` and `task_group` classes for efficient thread pooling and concurrent task management.
* **Interval Constraints:** Includes a modified version of the algorithm that restricts the search for optimal values within specifically defined interval boundaries.
* **Multi-variable Support:** Capable of finding the minimum of functions with one and two variables. 

## Performance & Speedup
The performance of the multithreaded algorithm was measured against the sequential version by finding the minimum of a 2-variable function. Testing was conducted on an Intel Core i7-10750H processor (6 physical cores, 12 threads).

* **Maximum Speedup:** The concurrent algorithm achieved a **3.72x speedup** when utilizing 12 threads compared to the baseline sequential execution.
* **Efficiency:** The advantages of parallel computation become highly noticeable and stable starting from 100 algorithm iterations.

## Prerequisites & Build Instructions
To build and run this project, you need:
* A modern C++ compiler.
* **Intel oneTBB Library**: Must be installed and configured in your environment.
* **Visual Studio**: The project is structured as a VS solution.

**How to build:**
1. Clone this repository to your local machine.
2. Open the `.sln` file in Visual Studio.
3. Ensure that the oneTBB library `include` and `lib` directories are correctly configured in the Project Properties (C/C++ -> General -> Additional Include Directories, and Linker -> General -> Additional Library Directories).
4. Build the solution (Ctrl+Shift+B) and run.
