#pragma once

#ifdef SERIAL_SOL_LIBRARY_EXPORTS
#define SERIAL_SOL_LIBRARY_API extern "C" __declspec(dllexport)
#else
#define SERIAL_SOL_LIBRARY_API extern "C" __declspec(dllimport)
#endif
#include "../Utils/utils.h"

SERIAL_SOL_LIBRARY_API float** serial_solver(float** T, int n, int m, int time, params param, int rank = 0, int size = 1, int miniN = -1, float* left_buffer = nullptr, float* right_buffer = nullptr);
