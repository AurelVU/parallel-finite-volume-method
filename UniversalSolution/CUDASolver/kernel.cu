#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include "CUDASolver.h"
#include "../Utils/utils.h"


__global__ void solverKernel(float* T, float* T_new, int n, int m, params param, float fluxR, float fluxL, float fluxU, float fluxD, float fluxLgran, float fluxRgran, float fluxUgran, float fluxDgran, float fluxC, float fluxCLgran, float fluxCRgran, float fluxCUgran, float fluxDUgran, float fluxCLUgran, float fluxCRUgran, float fluxCLDgran, float fluxDRUgran)
{
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    int j = blockIdx.y * blockDim.y + threadIdx.y;

    if (i >= 0 && i < n && j >= 0 && j < m)
    {
        double cflaxL = 0;
        double cflaxR = 0;
        double cflaxU = 0;
        double cflaxD = 0;

        if (i == 0)
            cflaxL = fluxLgran * (param.Tb - T[i * m + j]);
        else
            cflaxL = fluxL * (T[(i - 1) * m + j] - T[i * m + j]);
        if (i == n - 1)
            cflaxR = fluxRgran * (param.T0 - T[i * m + j]);
        else
            cflaxR = fluxR * (T[(i + 1) * m + j] - T[i * m + j]);

        if (j == 0)
            cflaxD = fluxDgran * (param.T0 - T[i * m + j]);
        else
            cflaxD = fluxD * (T[i * m + j - 1] - T[i * m + j]);
        if (j == m - 1)
            cflaxU = fluxUgran * (param.T0 - T[i * m + j]);
        else
            cflaxU = fluxU * (T[i * m + j + 1] - T[i * m + j]);

        T_new[i * m + j] = T[i * m + j] - param.dt * param.h * param.h * param.dz * (cflaxL + cflaxR + cflaxU + cflaxD);
    }
}

cudaError_t internel_solver(float** T, int n, int m, int time, params param)
{
    double** T_new = new double* [n];
    for (int i = 0; i < n; i++)
        T_new[i] = new double[m];

    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            T_new[i][j] = 0;


    float* dev_T = 0;
    float* dev_T_new = 0;

    cudaError_t cudaStatus;

    cudaStatus = cudaSetDevice(0);
    if (cudaStatus != cudaSuccess) {
        goto Error;
    }

    cudaStatus = cudaMalloc((void**)&dev_T, n * m * sizeof(float));
    if (cudaStatus != cudaSuccess) {
        goto Error;
    }

    cudaStatus = cudaMalloc((void**)&dev_T_new, n * m * sizeof(float));
    if (cudaStatus != cudaSuccess) {
        goto Error;
    }

    dim3 gridSize = dim3((n + 31) / 32, (m + 31) / 32, 1);
    dim3 blockSize = dim3(32, 32, 1);

    for (int i = 0; i < time; i++)
    {
        double fluxR = -param.k * param.h;
        double fluxL = -param.k * param.h;
        double fluxU = -param.k * param.h;
        double fluxD = -param.k * param.h;

        double fluxLgran = -param.kL * param.h * 2;
        double fluxRgran = -param.kR * param.h * 2;
        double fluxUgran = -param.kU * param.h * 2;
        double fluxDgran = -param.kD * param.h * 2;

        double fluxC = -4 * param.k * param.h;

        double fluxCLgran = fluxR + fluxLgran + fluxU + fluxD;
        double fluxCRgran = fluxRgran + fluxL + fluxU + fluxD;
        double fluxCUgran = fluxR + fluxL + fluxUgran + fluxD;
        double fluxDUgran = fluxR + fluxL + fluxU + fluxDgran;

        double fluxCLUgran = fluxR + fluxLgran + fluxUgran + fluxD;
        double fluxCRUgran = fluxRgran + fluxL + fluxUgran + fluxD;
        double fluxCLDgran = fluxR + fluxLgran + fluxU + fluxDgran;
        double fluxDRUgran = fluxRgran + fluxL + fluxU + fluxDgran;


        solverKernel <<<gridSize, blockSize>>> (dev_T, dev_T_new, n, m, param, fluxR, fluxL, fluxU, fluxD, fluxLgran, fluxRgran, fluxUgran, fluxDgran, fluxC, fluxCLgran, fluxCRgran, fluxCUgran, fluxDUgran, fluxCLUgran, fluxCRUgran, fluxCLDgran, fluxDRUgran);
        float* a = dev_T;
        dev_T = dev_T_new;
        dev_T_new = a;
    }

    cudaStatus = cudaGetLastError();
    if (cudaStatus != cudaSuccess) {
        goto Error;
    }

    cudaStatus = cudaDeviceSynchronize();
    if (cudaStatus != cudaSuccess) {
        goto Error;
    }

    for (int i = 0; i < n; i++)
    {
        cudaStatus = cudaMemcpy(T[i], dev_T + i * m, n * sizeof(float), cudaMemcpyDeviceToHost);
        if (cudaStatus != cudaSuccess) {
            goto Error;
        }
    }

Error:
    cudaFree(dev_T);
    cudaFree(dev_T_new);

    return cudaStatus;
}


float** cuda_solver(float** T_inp, int n, int m, int time, params param, int rank, int size, int miniN, float* left_buffer, float* right_buffer)
{
    if (rank == 0 && size == 1)
        miniN = n;
	
    float** T = T_inp;//copy2DArray<float>(T_inp, n, m);
	
    cudaError_t cudaStatus = internel_solver(T, n, m, time, param);

    if (cudaStatus != cudaSuccess)
        throw cudaStatus;

    cudaStatus = cudaDeviceReset();
    if (cudaStatus != cudaSuccess)
        throw cudaStatus;
    return T;
}

bool isSupportedCUDA()
{
	int num_gpus = 0;
    cudaError_t err = cudaGetDeviceCount(&num_gpus);
    return num_gpus > 0;
}