#include <cstdio>
#include <ctime>
#include <iostream>
#include <mpi.h>
#include <vector>




void run_phis(double** T, double** T_new, int n, int m, int rank, int size, int miniN, double* left_buffer, double* right_buffer)
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

    for (int gloval_i = miniN * rank; gloval_i < std::min(miniN * (rank + 1), n); gloval_i++) {
        
        int i = gloval_i - miniN * rank;
        for (int j = 0; j < m; j++) {
            double cflaxL;
            double cflaxR;
            double cflaxU;
            double cflaxD;
            double cflaxC;
            
            if (gloval_i == 0)
                cflaxL = fluxLgran * (Tb - T[i][j]);
            else
                if (i == 0)
                    cflaxL = fluxL * (left_buffer[j] - T[i][j]);
                else
					cflaxL = fluxL * (T[i - 1][j] - T[i][j]);
            if (gloval_i == n - 1)
                cflaxR = fluxRgran * (T0 - T[i][j]);
            else
                if ((i + 1) % miniN == 0)
                    cflaxR = fluxR * (right_buffer[j] - T[i][j]);
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
}



int main(int argc, char* argv[]) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    

    
    int n = 10;
    int m = 10;
    
    int miniN = ceil(double(n) / size);

    double* t = new double[miniN * m];
    double** T = new double* [miniN];
    for (int i = 0; i < miniN; i++)
    {
	    
        T[i] = t + i * m;
        for (int j = 0; j < m; j++)
        {
            T[i][j] = 0;
        }
    }
    
	
    double* t_new = new double[miniN * m];
    double** T_new = new double* [miniN];
    for (int i = 0; i < miniN; i++)
        T_new[i] = t_new + i * m;
    
    
    
    const auto start = clock();
	
    double* left_buffer = new double[m];
    double* right_buffer = new double[m];
    for (int i = 0; i < 500; i++)
    {
        MPI_Status stat;
        if (rank == 0 && rank != size - 1)
        {
            MPI_Send(T[miniN - 1], m, MPI_DOUBLE, rank + 1, 0, MPI_COMM_WORLD);
            MPI_Recv(right_buffer, m, MPI_DOUBLE, rank + 1, MPI_ANY_TAG, MPI_COMM_WORLD, &stat);
        }
        else if (rank == size - 1)
        {
            MPI_Recv(left_buffer, m, MPI_DOUBLE, rank - 1, MPI_ANY_TAG, MPI_COMM_WORLD, &stat);
            MPI_Send(T[miniN - 1], m, MPI_DOUBLE, rank - 1, 1, MPI_COMM_WORLD);
        }
        else
        {
            MPI_Sendrecv(T[0], m, MPI_DOUBLE, rank + 1, 0, left_buffer, m, MPI_DOUBLE, rank - 1, MPI_ANY_TAG, MPI_COMM_WORLD, &stat);
            MPI_Sendrecv(T[miniN - 1], m, MPI_DOUBLE, rank - 1, 0, right_buffer, m, MPI_DOUBLE, rank + 1, MPI_ANY_TAG, MPI_COMM_WORLD, &stat);
        }
    	
    	run_phis(T, T_new, n, m, rank, size, miniN, left_buffer, right_buffer);
        double** q = T;
        T = T_new;
        T_new = q;
        double* qq = t;
        t = t_new;
        t_new = qq;

        MPI_Barrier(MPI_COMM_WORLD);
    }

	
    double* t_res = nullptr;
    double** T_res = nullptr;
    if (rank == 0)
    {
        t_res = new double[n * m];
        T_res = new double* [n];
        for (int i = 0; i < n; i++)
            T_res[i] = t_res + i * n;
    }
    auto qq = MPI_Gather(t, miniN * m, MPI_DOUBLE, t_res, miniN * m, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	
    
	
    clock_t end = clock();
	/*if (rank == 0)
	{

		for (int i = 0; i < n; i++)
		{
            for (int j = 0; j < m; j++)
	            std::cout << T_res[i][j] << ", ";
            std::cout << std::endl;
		}
		
	}*/
    double seconds = (double)(end - start) / CLOCKS_PER_SEC;

    MPI_Barrier(MPI_COMM_WORLD);

	
	
    if (rank == 0)
    {
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < m; j++)
                std::cout << T[i][j] << ' ';
            std::cout << std::endl;
        }
        printf("%f,", seconds);
    }
    MPI_Finalize();
    
	
	
	return 0;
}
