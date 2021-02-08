
#include <ctime>

#include "cuda_runtime.h"
#include "device_launch_parameters.h"

#include <stdio.h>


cudaError_t solver(float** T, int n, int m);


__global__ void solverKernel(float* T, float* T_new, int n, int m, float h, float k, float dt, float dz, float Tb, float T0, float kL, float kR, float kU, float kD, float fluxR, float fluxL, float fluxU, float fluxD, float fluxLgran, float fluxRgran, float fluxUgran, float fluxDgran, float fluxC, float fluxCLgran, float fluxCRgran, float fluxCUgran, float fluxDUgran, float fluxCLUgran, float fluxCRUgran, float fluxCLDgran, float fluxDRUgran)
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
            cflaxL = fluxLgran * (Tb - T[i * m + j]);
        else
            cflaxL = fluxL * (T[(i - 1) * m + j] - T[i * m + j]);
        if (i == n - 1)
            cflaxR = fluxRgran * (T0 - T[i * m + j]);
        else
            cflaxR = fluxR * (T[(i + 1) * m + j] - T[i * m + j]);

        if (j == 0)
            cflaxD = fluxDgran * (T0 - T[i * m + j]);
        else
            cflaxD = fluxD * (T[i * m + j - 1] - T[i * m + j]);
        if (j == m - 1)
            cflaxU = fluxUgran * (T0 - T[i * m + j]);
        else
            cflaxU = fluxU * (T[i * m + j + 1] - T[i * m + j]);

        T_new[i * m + j] = T[i * m + j] - dt * h * h * dz * (cflaxL + cflaxR + cflaxU + cflaxD);
    }
}

int main()
{
    int n = 210;
    int m = 210;
    float** T = new float*[n];
    for (int i = 0; i < n; i++)
    {
        T[i] = new float[m];
        for (int j = 0; j < m; j++)
            T[i][j] = 0;
    }

    clock_t start = clock();
    cudaError_t cudaStatus = solver(T, n, m);
    clock_t end = clock();
    double seconds = (double)(end - start) / CLOCKS_PER_SEC;
    printf("The time: %f seconds\n", seconds);
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "addWithCuda failed!");
        return 1;
    }

    cudaStatus = cudaDeviceReset();
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaDeviceReset failed!");
        return 1;
    }

    return 0;
}



cudaError_t solver(float** T, int n, int m)
{
    double h = 0.1;
    double k = 50;

    double dt = 1;
    double dz = 1;
    double Tb = 240;
    double T0 = 0;

    double kL = 1;
    double kR = 0;
    double kU = 10;
    double kD = 10;

    double** T_new = new double* [n];
    for (int i = 0; i < n; i++)
        T_new[i] = new double[m];

    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            T_new[i][j] = 0;

	
    float* dev_T = 0;
    float* dev_T_new = 0;
	
    cudaError_t cudaStatus;

    // Choose which GPU to run on, change this on a multi-GPU system.
    cudaStatus = cudaSetDevice(0);
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaSetDevice failed!  Do you have a CUDA-capable GPU installed?");
        goto Error;
    }

    // Allocate GPU buffers for three vectors (two input, one output)    .
    cudaStatus = cudaMalloc((void**)&dev_T, n * m * sizeof(float));
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMalloc failed!");
        goto Error;
    }

    // Allocate GPU buffers for three vectors (two input, one output)    .
    cudaStatus = cudaMalloc((void**)&dev_T_new, n * m * sizeof(float));
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMalloc failed!");
        goto Error;
    }

    dim3 gridSize = dim3((n + 31) / 32, (m + 31) / 32, 1);    //Размер используемого грида
    dim3 blockSize = dim3(32, 32, 1); //Размер используемого блока


    // Launch a kernel on the GPU with one thread for each element.
    for (int i = 0; i < 500; i++)
    {
        double fluxR = -k * h;
        double fluxL = -k * h;
        double fluxU = -k * h;
        double fluxD = -k * h;

        double fluxLgran = -kL * h * 2;
        double fluxRgran = -kR * h * 2;
        double fluxUgran = -kU * h * 2;
        double fluxDgran = -kD * h * 2;

        double fluxC = -4 * k * h;

        double fluxCLgran = fluxR + fluxLgran + fluxU + fluxD;
        double fluxCRgran = fluxRgran + fluxL + fluxU + fluxD;
        double fluxCUgran = fluxR + fluxL + fluxUgran + fluxD;
        double fluxDUgran = fluxR + fluxL + fluxU + fluxDgran;

        double fluxCLUgran = fluxR + fluxLgran + fluxUgran + fluxD;
        double fluxCRUgran = fluxRgran + fluxL + fluxUgran + fluxD;
        double fluxCLDgran = fluxR + fluxLgran + fluxU + fluxDgran;
        double fluxDRUgran = fluxRgran + fluxL + fluxU + fluxDgran;

    	
        solverKernel <<<gridSize, blockSize >>> (dev_T, dev_T_new, n, m, h, k, dt, dz, Tb, T0, kL, kR, kU, kD, fluxR, fluxL, fluxU, fluxD, fluxLgran, fluxRgran, fluxUgran, fluxDgran, fluxC, fluxCLgran, fluxCRgran, fluxCUgran, fluxDUgran, fluxCLUgran, fluxCRUgran, fluxCLDgran, fluxDRUgran);
        float* a = dev_T;
        dev_T = dev_T_new;
        dev_T_new = a;
    }
    // Check for any errors launching the kernel
    cudaStatus = cudaGetLastError();
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "addKernel launch failed: %s\n", cudaGetErrorString(cudaStatus));
        goto Error;
    }

    // cudaDeviceSynchronize waits for the kernel to finish, and returns
    // any errors encountered during the launch.
    cudaStatus = cudaDeviceSynchronize();
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaDeviceSynchronize returned error code %d after launching addKernel!\n", cudaStatus);
        goto Error;
    }

    // Copy output vector from GPU buffer to host memory.
    for (int i = 0; i < n; i++)
    {
        cudaStatus = cudaMemcpy(T[i], dev_T + i * m, n * sizeof(float), cudaMemcpyDeviceToHost);
        if (cudaStatus != cudaSuccess) {
            fprintf(stderr, "cudaMemcpy failed!");
            goto Error;
        }
    }
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
            printf("%.2f ", T[i][j]);
        printf("\n");
    }
    
Error:
    cudaFree(dev_T);
    cudaFree(dev_T_new);

    return cudaStatus;
}