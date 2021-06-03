#pragma once

#ifdef CUDA_SOL_LIBRARY_EXPORTS
#define CUDA_SOL_LIBRARY_API extern "C" __declspec(dllexport)
#else
#define CUDA_SOL_LIBRARY_API extern "C" __declspec(dllimport)
#endif
#include "../Utils/utils.h"

CUDA_SOL_LIBRARY_API float** cuda_solver(float** T, int n, int m, int time, params param, int rank = 0, int size = 1, int miniN = -1, float* left_buffer = nullptr, float* right_buffer = nullptr);
CUDA_SOL_LIBRARY_API bool isSupportedCUDA();