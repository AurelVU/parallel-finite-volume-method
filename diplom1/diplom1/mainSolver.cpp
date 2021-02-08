#include "mainSolver.h"

bool mainSolver::create(string filename_)
{
    
    delt = 0.2;
    FaceArea = 1;
    k = 1;
    dh = 1;
    n = 40;
    jmax = 40;
    this->T = new double* [n];
    this->T_new = new double* [n];
    for (int i = 0; i < n; i++)
    {
        T[i] = new double[jmax];
        T_new[i] = new double[jmax];
        for (int j = 0; j < jmax; j++)
        {
            T[i][j] = 0;
            T_new[i][j] = 0;
        }
    }

    return true;
    
}

void mainSolver::RunPhysic()
{
    double Tc, Te, Tw, Tn, Ts;
    double FluxC, FluxE, FluxW, FluxN, FluxS;
    double dx = 0;
    double Tb = 240;
    double Tb0 = 0;

    int i, j;
    for (i = 0; i < n; i++)
        for (j = 0; j < jmax; j++)
        {
            Tc = T[i][j];
            dx = dh;

            if (i == n - 1) { Te = Tb0; dx = dx / 2; }
            else
                Te = T[i + 1][j];
            FluxE = (-k * FaceArea) / dx;

            if (i == 0) { Tw = Tb0; dx = dx / 2; }
            else
                Tw = T[i - 1][j];
            FluxW = (-k * FaceArea) / dx;

            if (j == jmax - 1) { Tn = Tb0; dx = dx / 2; }
            else
                Tn = T[i][j + 1];
            FluxN = (-k * FaceArea) / dx;

            if (j == 0) { Ts = Tb; dx = dx / 2; }
            else
                Ts = T[i][j - 1];
            FluxS = (-k * FaceArea) / dx;

            FluxC = FluxE + FluxW + FluxN + FluxS;

            T[i][j] = Tc + delt * (FluxC * Tc - (FluxE * Te + FluxW * Tw + FluxN * Tn + FluxS * Ts));
        }
    
}

void mainSolver::RunPhysicWithNew()
{
    double Tc, Te, Tw, Tn, Ts;
    double FluxC, FluxE, FluxW, FluxN, FluxS;
    double dx = 0;
    double Tb = 240;
    double Tb0 = 0;

    int i, j;
    for (i = 0; i < n; i++)
        for (j = 0; j < jmax; j++)
        {
            Tc = T[i][j];
            dx = dh;

            if (i == n - 1) { Te = Tb0; dx = dx / 2; }
            else
                Te = T[i + 1][j];
            FluxE = (-k * FaceArea) / dx;

            if (i == 0) { Tw = Tb0; dx = dx / 2; }
            else
                Tw = T[i - 1][j];
            FluxW = (-k * FaceArea) / dx;

            if (j == jmax - 1) { Tn = Tb0; dx = dx / 2; }
            else
                Tn = T[i][j + 1];
            FluxN = (-k * FaceArea) / dx;

            if (j == 0) { Ts = Tb; dx = dx / 2; }
            else
                Ts = T[i][j - 1];
            FluxS = (-k * FaceArea) / dx;

            FluxC = FluxE + FluxW + FluxN + FluxS;

            T_new[i][j] = Tc + delt * (FluxC * Tc - (FluxE * Te + FluxW * Tw + FluxN * Tn + FluxS * Ts));
        }
    double** q = T;
    T = T_new;
    T_new = q;
}



void mainSolver::RunPhysic2()
{
    double Tc, Te, Tw, Tn, Ts;
    double FluxC, FluxE, FluxW, FluxN, FluxS;
    double dx = 0;
    double Tb = 240;
    double Tb0 = 0;

    int i, j;
    for (i = 0; i < n; i++)
        for (j = 0; j < jmax; j++)
        {
            Tc = T[i][j];
            dx = dh;

            Te = 0; Tw = 0;
            if (i == n - 1)
                FluxE = 0;
            else
            {
                Te = T[i + 1][j];
                FluxE = (-k * FaceArea) / dx;
            }

            if (i == 0)
                FluxW = 0;
            else
            {
                Tw = T[i - 1][j];
                FluxW = (-k * FaceArea) / dx;
            }

            if (j == jmax - 1) { Tn = Tb0; dx = dx / 2; }
            else
                Tn = T[i][j + 1];
            FluxN = (-k * FaceArea) / dx;

            if (j == 0) { Ts = Tb; dx = dx / 2; }
            else
                Ts = T[i][j - 1];
            FluxS = (-k * FaceArea) / dx;

            FluxC = FluxE + FluxW + FluxN + FluxS;

            T[i][j] = Tc + delt * (FluxC * Tc - (FluxE * Te + FluxW * Tw + FluxN * Tn + FluxS * Ts));
        }
    
}
