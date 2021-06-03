#include <iostream>
#include <fstream>
#include <CL/cl.hpp>

const char* getErrorString(cl_int error)
{
	switch (error) {
		// run-time and JIT compiler errors
	case 0: return "CL_SUCCESS";
	case -1: return "CL_DEVICE_NOT_FOUND";
	case -2: return "CL_DEVICE_NOT_AVAILABLE";
	case -3: return "CL_COMPILER_NOT_AVAILABLE";
	case -4: return "CL_MEM_OBJECT_ALLOCATION_FAILURE";
	case -5: return "CL_OUT_OF_RESOURCES";
	case -6: return "CL_OUT_OF_HOST_MEMORY";
	case -7: return "CL_PROFILING_INFO_NOT_AVAILABLE";
	case -8: return "CL_MEM_COPY_OVERLAP";
	case -9: return "CL_IMAGE_FORMAT_MISMATCH";
	case -10: return "CL_IMAGE_FORMAT_NOT_SUPPORTED";
	case -11: return "CL_BUILD_PROGRAM_FAILURE";
	case -12: return "CL_MAP_FAILURE";
	case -13: return "CL_MISALIGNED_SUB_BUFFER_OFFSET";
	case -14: return "CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST";
	case -15: return "CL_COMPILE_PROGRAM_FAILURE";
	case -16: return "CL_LINKER_NOT_AVAILABLE";
	case -17: return "CL_LINK_PROGRAM_FAILURE";
	case -18: return "CL_DEVICE_PARTITION_FAILED";
	case -19: return "CL_KERNEL_ARG_INFO_NOT_AVAILABLE";

		// compile-time errors
	case -30: return "CL_INVALID_VALUE";
	case -31: return "CL_INVALID_DEVICE_TYPE";
	case -32: return "CL_INVALID_PLATFORM";
	case -33: return "CL_INVALID_DEVICE";
	case -34: return "CL_INVALID_CONTEXT";
	case -35: return "CL_INVALID_QUEUE_PROPERTIES";
	case -36: return "CL_INVALID_COMMAND_QUEUE";
	case -37: return "CL_INVALID_HOST_PTR";
	case -38: return "CL_INVALID_MEM_OBJECT";
	case -39: return "CL_INVALID_IMAGE_FORMAT_DESCRIPTOR";
	case -40: return "CL_INVALID_IMAGE_SIZE";
	case -41: return "CL_INVALID_SAMPLER";
	case -42: return "CL_INVALID_BINARY";
	case -43: return "CL_INVALID_BUILD_OPTIONS";
	case -44: return "CL_INVALID_PROGRAM";
	case -45: return "CL_INVALID_PROGRAM_EXECUTABLE";
	case -46: return "CL_INVALID_KERNEL_NAME";
	case -47: return "CL_INVALID_KERNEL_DEFINITION";
	case -48: return "CL_INVALID_KERNEL";
	case -49: return "CL_INVALID_ARG_INDEX";
	case -50: return "CL_INVALID_ARG_VALUE";
	case -51: return "CL_INVALID_ARG_SIZE";
	case -52: return "CL_INVALID_KERNEL_ARGS";
	case -53: return "CL_INVALID_WORK_DIMENSION";
	case -54: return "CL_INVALID_WORK_GROUP_SIZE";
	case -55: return "CL_INVALID_WORK_ITEM_SIZE";
	case -56: return "CL_INVALID_GLOBAL_OFFSET";
	case -57: return "CL_INVALID_EVENT_WAIT_LIST";
	case -58: return "CL_INVALID_EVENT";
	case -59: return "CL_INVALID_OPERATION";
	case -60: return "CL_INVALID_GL_OBJECT";
	case -61: return "CL_INVALID_BUFFER_SIZE";
	case -62: return "CL_INVALID_MIP_LEVEL";
	case -63: return "CL_INVALID_GLOBAL_WORK_SIZE";
	case -64: return "CL_INVALID_PROPERTY";
	case -65: return "CL_INVALID_IMAGE_DESCRIPTOR";
	case -66: return "CL_INVALID_COMPILER_OPTIONS";
	case -67: return "CL_INVALID_LINKER_OPTIONS";
	case -68: return "CL_INVALID_DEVICE_PARTITION_COUNT";

		// extension errors
	case -1000: return "CL_INVALID_GL_SHAREGROUP_REFERENCE_KHR";
	case -1001: return "CL_PLATFORM_NOT_FOUND_KHR";
	case -1002: return "CL_INVALID_D3D10_DEVICE_KHR";
	case -1003: return "CL_INVALID_D3D10_RESOURCE_KHR";
	case -1004: return "CL_D3D10_RESOURCE_ALREADY_ACQUIRED_KHR";
	case -1005: return "CL_D3D10_RESOURCE_NOT_ACQUIRED_KHR";
	default: return "Unknown OpenCL error";
	}
}


int main() {
	for (int bifFuckingN = 50; bifFuckingN < 10000; bifFuckingN += 25) {
		clock_t start = clock();
		
		unsigned id_platform = 0;
		unsigned id_device = 0;
		//get all platforms (drivers)
		std::vector<cl::Platform> all_platforms;
		cl::Platform::get(&all_platforms);
		if (all_platforms.size() == 0) {
			//std::cout << " No platforms found. Check OpenCL installation!\n";
			exit(1);
		}
		else
		{
			//std::cout << "Chose platform" << std::endl;
			for (int i = 0; i < all_platforms.size(); i++)
			{
				//std::cout << i << ": " << all_platforms[i].getInfo<CL_PLATFORM_NAME>() << std::endl;
			}
			//std::cout << ">> ";
			//std::cin >> id_platform;
			id_platform = 0; //костыль
		}
		cl::Platform default_platform = all_platforms[id_platform];
		//std::cout << "Using platform: " << default_platform.getInfo<CL_PLATFORM_NAME>() << std::endl;

		//get default device of the default platform
		std::vector<cl::Device> all_devices;
		default_platform.getDevices(CL_DEVICE_TYPE_ALL, &all_devices);
		if (all_devices.size() == 0) {
			//std::cout << " No devices found. Check OpenCL installation!\n";
			exit(1);
		}
		else
		{
			//std::cout << "Chose device" << std::endl;
			for (int i = 0; i < all_devices.size(); i++)
			{
				//std::cout << i << ": " << all_devices[i].getInfo<CL_DEVICE_NAME>() << std::endl;
			}
			//std::cout << ">> ";
			//std::cin >> id_device;
			id_device = 0; // еще костыль

		}
		cl::Device default_device = all_devices[id_device];
		//std::cout << "Using device: " << default_device.getInfo<CL_DEVICE_NAME>() << "\n";


		cl::Context context({ default_device });

		std::ifstream helloWorldFile("kernel.cl");
		std::string src(std::istreambuf_iterator<char>(helloWorldFile), (std::istreambuf_iterator<char>()));

		cl::Program::Sources sources(1, std::make_pair(src.c_str(), src.length() + 1));

		cl::Program program(context, sources);
		if (program.build({ default_device }) != CL_SUCCESS) {
			std::cout << " Error building: " << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(default_device) << "\n";
			exit(1);
		}



	
		// int n = 210;
		// int m = 210;
		int n = bifFuckingN;
		int m = bifFuckingN;
		//int n = 210;
		//int m = 210;

		float h = 0.1;
		float k = 50;

		float dt = 1;
		float dz = 1;
		float Tb = 240;
		float T0 = 0;

		float kL = 1;
		float kR = 0;
		float kU = 10;
		float kD = 10;

		auto size = n * m;

		float* T = new float[size];
		for (int i = 0; i < n * m; i++)
		{
			T[i] = 0;
		}

		float* T_new = new float[size];
		for (int i = 0; i < n; i++)
			T_new[i] = 0;

		
		
		cl::Buffer dev_T(context, CL_MEM_READ_WRITE, sizeof(float) * n * m);
		cl::Buffer dev_T_new(context, CL_MEM_READ_WRITE, sizeof(float) * n * m);

		cl::CommandQueue queue(context, default_device);


		queue.enqueueWriteBuffer(dev_T, CL_TRUE, 0, sizeof(int) * n, T);
		queue.enqueueWriteBuffer(dev_T_new, CL_TRUE, 0, sizeof(int) * n, T_new);
		cl::Buffer dev_n(context, CL_MEM_READ_WRITE, sizeof(int));
		queue.enqueueWriteBuffer(dev_n, CL_TRUE, 0, sizeof(int), &n);

		cl::Buffer dev_m(context, CL_MEM_READ_WRITE, sizeof(int));
		queue.enqueueWriteBuffer(dev_m, CL_TRUE, 0, sizeof(int), &m);

		cl::Buffer dev_h(context, CL_MEM_READ_WRITE, sizeof(float));
		queue.enqueueWriteBuffer(dev_h, CL_TRUE, 0, sizeof(float), &h);

		cl::Buffer dev_k(context, CL_MEM_READ_WRITE, sizeof(float));
		queue.enqueueWriteBuffer(dev_k, CL_TRUE, 0, sizeof(float), &k);

		cl::Buffer dev_dt(context, CL_MEM_READ_WRITE, sizeof(float));
		queue.enqueueWriteBuffer(dev_dt, CL_TRUE, 0, sizeof(float), &dt);

		cl::Buffer dev_dz(context, CL_MEM_READ_WRITE, sizeof(float));
		queue.enqueueWriteBuffer(dev_dz, CL_TRUE, 0, sizeof(float), &dz);

		cl::Buffer dev_Tb(context, CL_MEM_READ_WRITE, sizeof(float));
		queue.enqueueWriteBuffer(dev_Tb, CL_TRUE, 0, sizeof(float), &Tb);

		cl::Buffer dev_T0(context, CL_MEM_READ_WRITE, sizeof(float));
		queue.enqueueWriteBuffer(dev_T0, CL_TRUE, 0, sizeof(float), &T0);

		cl::Buffer dev_kL(context, CL_MEM_READ_WRITE, sizeof(float));
		queue.enqueueWriteBuffer(dev_kL, CL_TRUE, 0, sizeof(float), &kL);

		cl::Buffer dev_kR(context, CL_MEM_READ_WRITE, sizeof(float));
		queue.enqueueWriteBuffer(dev_kR, CL_TRUE, 0, sizeof(float), &kR);

		cl::Buffer dev_kU(context, CL_MEM_READ_WRITE, sizeof(float));
		queue.enqueueWriteBuffer(dev_kU, CL_TRUE, 0, sizeof(float), &kU);

		cl::Buffer dev_kD(context, CL_MEM_READ_WRITE, sizeof(float));
		queue.enqueueWriteBuffer(dev_kD, CL_TRUE, 0, sizeof(float), &kD);

		for (int i = 0; i < 500; i++)
		{
			cl::Kernel simple_add(program, "solverKernel");
			simple_add.setArg(0, dev_T);
			simple_add.setArg(1, dev_T_new);
			simple_add.setArg(2, dev_n);
			simple_add.setArg(3, dev_m);
			simple_add.setArg(4, dev_h);
			simple_add.setArg(5, dev_k);
			simple_add.setArg(6, dev_dt);
			simple_add.setArg(7, dev_dz);
			simple_add.setArg(8, dev_Tb);
			simple_add.setArg(9, dev_T0);
			simple_add.setArg(10, dev_kL);
			simple_add.setArg(11, dev_kR);
			simple_add.setArg(12, dev_kU);
			simple_add.setArg(13, dev_kD);
			queue.enqueueNDRangeKernel(simple_add, cl::NullRange, cl::NDRange(n, m), cl::NullRange);

			auto a = dev_T;
			dev_T = dev_T_new;
			dev_T_new = a;
		}
		queue.finish();

		float* C = new float[size];
		//read result C from the device to array C
		//std::cout << "res";
		//std::cout << getErrorString(queue.enqueueReadBuffer(dev_T, CL_TRUE, 0, sizeof(float) * n * m, C)) << std::endl;
		clock_t end = clock();
		double seconds = (double)(end - start) / CLOCKS_PER_SEC;
		/*for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < m; j++)
				printf("%.2f ", C[i * m + j]);
			printf("\n");
		}*/
		printf("%f, ", seconds);
	}

	return 0;
}