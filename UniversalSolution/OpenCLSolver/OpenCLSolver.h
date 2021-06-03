#pragma once

#ifdef OPENCL_SOL_LIBRARY_EXPORTS
#define OPENCL_SOL_LIBRARY_API extern "C" __declspec(dllexport)
#else
#define OPENCL_SOL_LIBRARY_API extern "C" __declspec(dllimport)
#endif
#include "../Utils/utils.h"

OPENCL_SOL_LIBRARY_API float** opencl_solver(float** T, int n, int m, int time, params param, int rank = 0, int size = 1, int miniN = -1, float* left_buffer = nullptr, float* right_buffer = nullptr);
OPENCL_SOL_LIBRARY_API bool isSupportedOpenCL();