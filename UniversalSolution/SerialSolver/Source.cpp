#include <vector>
#include "SerialSolver.h"
#include "../Utils/utils.h"

void run_phis(float** T, float** T_new, int n, int m, params param, int rank, int size, int miniN, float* left_buffer, float* right_buffer)
{

    float fluxR = -param.k * param.h;
    float fluxL = -param.k * param.h;
    float fluxU = -param.k * param.h;
    float fluxD = -param.k * param.h;

    float fluxLgran = -param.kL * param.h * 2;
    float fluxRgran = -param.kR * param.h * 2;
    float fluxUgran = -param.kU * param.h * 2;
    float fluxDgran = -param.kD * param.h * 2;

    float fluxC = -4 * param.k * param.h;

    float fluxCLgran = fluxR + fluxLgran + fluxU + fluxD;
    float fluxCRgran = fluxRgran + fluxL + fluxU + fluxD;
    float fluxCUgran = fluxR + fluxL + fluxUgran + fluxD;
    float fluxDUgran = fluxR + fluxL + fluxU + fluxDgran;

    float fluxCLUgran = fluxR + fluxLgran + fluxUgran + fluxD;
    float fluxCRUgran = fluxRgran + fluxL + fluxUgran + fluxD;
    float fluxCLDgran = fluxR + fluxLgran + fluxU + fluxDgran;
    float fluxDRUgran = fluxRgran + fluxL + fluxU + fluxDgran;

    for (int gloval_i = miniN * rank; gloval_i < std::min(miniN * (rank + 1), n); gloval_i++) {

        int i = gloval_i - miniN * rank;
        for (int j = 0; j < m; j++) {
            float cflaxL;
            float cflaxR;
            float cflaxU;
            float cflaxD;
            float cflaxC;

            if (gloval_i == 0)
                cflaxL = fluxLgran * (param.Tb - T[i][j]);
            else
                if (i == 0)
                    cflaxL = fluxL * (left_buffer[j] - T[i][j]);
                else
                    cflaxL = fluxL * (T[i - 1][j] - T[i][j]);
            if (gloval_i == n - 1)
                cflaxR = fluxRgran * (param.T0 - T[i][j]);
            else
                if ((i + 1) % miniN == 0)
                    cflaxR = fluxR * (right_buffer[j] - T[i][j]);
                else
                    cflaxR = fluxR * (T[i + 1][j] - T[i][j]);

            if (j == 0)
                cflaxD = fluxDgran * (param.T0 - T[i][j]);
            else
                cflaxD = fluxD * (T[i][j - 1] - T[i][j]);
            if (j == m - 1)
                cflaxU = fluxUgran * (param.T0 - T[i][j]);
            else
                cflaxU = fluxU * (T[i][j + 1] - T[i][j]);

            T_new[i][j] = T[i][j] - param.dt * param.h * param.h * param.dz * (cflaxL + cflaxR + cflaxU + cflaxD);
        }
    }
}


float** serial_solver(float** T_inp, int n, int m, int time, params param, int rank, int size, int miniN, float* left_buffer, float* right_buffer)
{
    if (rank == 0 && size == 1)
        miniN = n;
    float** T = T_inp;//copy2DArray<float>(T_inp, n, m);
    float** T_new = allocate2DArray<float>(n, m);
    for (int i = 0; i < time; i++)
    {
        run_phis(T, T_new, n, m, param, rank, size, miniN, left_buffer, right_buffer);
        float** q = T;
        T = T_new;
        T_new = T;
    }

    return T;
}
