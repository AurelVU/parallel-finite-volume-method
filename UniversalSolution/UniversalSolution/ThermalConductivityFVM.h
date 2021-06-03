#pragma once

#ifdef UNIVERSAL_SOL_LIBRARY_EXPORTS
#define UNIVERSAL_SOL_LIBRARY_API extern "C" __declspec(dllexport)
#else
#define UNIVERSAL_SOL_LIBRARY_API extern "C" __declspec(dllimport)
#endif

#include "../SerialSolver/SerialSolver.h"
#include "../OpenCLSolver/OpenCLSolver.h"
#include "../CUDASolver/CUDASolver.h"


#define SERIAL_VERSION 0
#define OPENCL_VERSION 1
#define CUDA_VERSION 2

UNIVERSAL_SOL_LIBRARY_API float** run_simulation(float** T, int n, int m, int time, params param, char type=SERIAL_VERSION);
UNIVERSAL_SOL_LIBRARY_API float** run_simulation_with_MPI(float** T, int n, int m, int time, params param, int argc, char* argv[]);