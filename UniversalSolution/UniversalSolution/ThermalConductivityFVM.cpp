#include "ThermalConductivityFVM.h"

#include <cmath>
#include <mpi.h>

float** run_simulation(float** T, int n, int m, int time, params param, char type)
{
	switch (type)
	{
	case SERIAL_VERSION:
		return serial_solver(T, n, m, time, param);
	case OPENCL_VERSION:
		return opencl_solver(T, n, m, time, param);
	case CUDA_VERSION:
		return cuda_solver(T, n, m, time, param);
	}
	return nullptr;
}


float** run_simulation_with_MPI(float** T_inp, int n, int m, int time, params param, int argc, char* argv[])
{
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

	int miniN = ceil(float(n) / size);

    //float** T = copy2DArray<float>(T_inp, n, m);

    float** T = T_inp; // йняршкэ!!!!
	float* t = T[0];
    
    float* t_new = new float[miniN * m];
    float** T_new = new float* [miniN];
    for (int i = 0; i < miniN; i++)
        T_new[i] = t_new + i * m;

    float* left_buffer = new float[m];
    float* right_buffer = new float[m];
    for (int i = 0; i < time; i++)
    {
        MPI_Status stat;
        if (rank == 0 && rank != size - 1)
        {
            MPI_Send(T[miniN - 1], m, MPI_FLOAT, rank + 1, 0, MPI_COMM_WORLD);
            MPI_Recv(right_buffer, m, MPI_FLOAT, rank + 1, MPI_ANY_TAG, MPI_COMM_WORLD, &stat);
        }
        else if (rank == size - 1)
        {
            MPI_Recv(left_buffer, m, MPI_FLOAT, rank - 1, MPI_ANY_TAG, MPI_COMM_WORLD, &stat);
            MPI_Send(T[miniN - 1], m, MPI_FLOAT, rank - 1, 1, MPI_COMM_WORLD);
        }
        else
        {
            MPI_Sendrecv(T[0], m, MPI_FLOAT, rank + 1, 0, left_buffer, m, MPI_FLOAT, rank - 1, MPI_ANY_TAG, MPI_COMM_WORLD, &stat);
            MPI_Sendrecv(T[miniN - 1], m, MPI_FLOAT, rank - 1, 0, right_buffer, m, MPI_FLOAT, rank + 1, MPI_ANY_TAG, MPI_COMM_WORLD, &stat);
        }

        T_new = serial_solver(T, n, m, 1, param, rank, size, miniN, left_buffer, right_buffer);
        float** q = T;
        T = T_new;
        T_new = q;
        float* qq = t;
        t = t_new;
        t_new = qq;

        MPI_Barrier(MPI_COMM_WORLD);
    }


    float* t_res = nullptr;
    float** T_res = nullptr;
    if (rank == 0)
    {
        t_res = new float[n * m];
        T_res = new float* [n];
        for (int i = 0; i < n; i++)
            T_res[i] = t_res + i * n;
    }
    auto qq = MPI_Gather(t, miniN * m, MPI_FLOAT, t_res, miniN * m, MPI_FLOAT, 0, MPI_COMM_WORLD);

    MPI_Finalize();
    return T;
}