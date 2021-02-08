#include <ctime>
#include <iostream>
#include <vector>


std::vector<std::vector<double>> run_phis(std::vector<std::vector<double>> T, int n, int m)
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
	
    std::vector<std::vector<double>> T_new;
    for (int i = 0; i < n; i++)
    {
        std::vector<double> a;
        T_new.push_back(a);
    }
	
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            T_new[i].push_back(0);


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

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            double cflaxL = 0;
            double cflaxR = 0;
            double cflaxU = 0;
            double cflaxD = 0;
            double cflaxC = 0;

            if (i == 0)
                cflaxL = fluxLgran * (Tb - T[i][j]);
            else
                cflaxL = fluxL * (T[i - 1][j] - T[i][j]);
            if (i == n - 1)
                cflaxR = fluxRgran * (T0 - T[i][j]);
            else
                cflaxR = fluxR * (T[i + 1][j] - T[i][j]);

            if (j == 0)
                cflaxD = fluxDgran * (T0 - T[i][j]);
            else
                cflaxD = fluxD * (T[i][j - 1] - T[i][j]);
            if (j == m - 1)
                cflaxU = fluxUgran * (T0 - T[i][j]);
            else
                cflaxU = fluxU * (T[i][j + 1] - T[i][j]);

            T_new[i][j] = T[i][j] - dt * h * h * dz * (cflaxL + cflaxR + cflaxU + cflaxD);
        }
    }
    return  T_new;
}

double** run_phis2(double** T, int n, int m)
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
    for (int i = 0; i < m; i++)
        T_new[i] = new double[m];

    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            T_new[i][j] = 0;


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

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            double cflaxL = 0;
            double cflaxR = 0;
            double cflaxU = 0;
            double cflaxD = 0;
            double cflaxC = 0;

            if (i == 0)
                cflaxL = fluxLgran * (Tb - T[i][j]);
            else
                cflaxL = fluxL * (T[i - 1][j] - T[i][j]);
            if (i == n - 1)
                cflaxR = fluxRgran * (T0 - T[i][j]);
            else
                cflaxR = fluxR * (T[i + 1][j] - T[i][j]);

            if (j == 0)
                cflaxD = fluxDgran * (T0 - T[i][j]);
            else
                cflaxD = fluxD * (T[i][j - 1] - T[i][j]);
            if (j == m - 1)
                cflaxU = fluxUgran * (T0 - T[i][j]);
            else
                cflaxU = fluxU * (T[i][j + 1] - T[i][j]);

            T_new[i][j] = T[i][j] - dt * h * h * dz * (cflaxL + cflaxR + cflaxU + cflaxD);
        }
    }
    return  T_new;
}


int main()
{
    int n = 210;
    int m = 210;
    std::vector<std::vector<double>> T;
    for (int i = 0; i < n; i++)
    {
        std::vector<double> a;
        T.push_back(a);
    }

    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            T[i].push_back(0);

    clock_t start = clock();
    for (int i = 0; i < 500; i++)
        T = run_phis(T, n, m);
    clock_t end = clock();
    double seconds = (double)(end - start) / CLOCKS_PER_SEC;
    
	
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++)
        {
            printf("%0.2f ", T[i][j]);
        }
        printf("\n");
    }
    printf("The time: %f seconds\n", seconds);
}
