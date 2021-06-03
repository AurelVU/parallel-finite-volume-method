#include <iostream>

#include "../UniversalSolution/ThermalConductivityFVM.h"
#include "../Utils/utils.h"

int main(int argc, char* argv[])
{
	char сtype;
	std::cout << "Write type of solver. S - Serial, O - OpenCL, C - CUDA : ";
	std::cin >> сtype;
	char type = SERIAL_VERSION;

	switch (сtype)
	{
	case 'C':
		type = CUDA_VERSION;
		break;
	case 'O':
		type = OPENCL_VERSION;
		break;
	case 'S':
		type = SERIAL_VERSION;
		break;
	}

	int n = 10;
	int m = 10;
    float** T = allocate2DArray<float>(n, m);
	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++)
			T[i][j] = 0;

	params param;
	param.h = 0.1;
	param.k = 50;

	param.dt = 1;
	param.dz = 1;
	param.Tb = 240;
	param.T0 = 0;

	param.kL = 1;
	param.kR = 0;
	param.kU = 10;
	param.kD = 10;
	
	float** res = run_simulation(T, n, m, 500, param, type);

	//float** res = run_simulation_with_MPI(T, n, m, 500, param, argc, argv);
	
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
			std::cout << res[i][j] << ' ';
		std::cout << std::endl;
	}

	system("pause");
}
