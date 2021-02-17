#include <iostream>
#include <fstream>
#include <CL/cl.hpp>

int main() {
	int n = 100;

	//get all platforms (drivers)
	std::vector<cl::Platform> all_platforms;
	cl::Platform::get(&all_platforms);
	if (all_platforms.size() == 0) {
		std::cout << " No platforms found. Check OpenCL installation!\n";
		exit(1);
	}
	cl::Platform default_platform = all_platforms[1];
	std::cout << "Using platform: " << default_platform.getInfo<CL_PLATFORM_NAME>() << "\n";

	//get default device of the default platform
	std::vector<cl::Device> all_devices;
	default_platform.getDevices(CL_DEVICE_TYPE_ALL, &all_devices);
	if (all_devices.size() == 0) {
		std::cout << " No devices found. Check OpenCL installation!\n";
		exit(1);
	}
	cl::Device default_device = all_devices[0];
	std::cout << "Using device: " << default_device.getInfo<CL_DEVICE_NAME>() << "\n";


	cl::Context context({ default_device });

	std::ifstream helloWorldFile("hello.cl");
	std::string src(std::istreambuf_iterator<char>(helloWorldFile), (std::istreambuf_iterator<char>()));

	cl::Program::Sources sources(1, std::make_pair(src.c_str(), src.length() + 1));

	cl::Program program(context, sources);
	if (program.build({ default_device }) != CL_SUCCESS) {
		std::cout << " Error building: " << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(default_device) << "\n";
		exit(1);
	}


	// create buffers on the device
	cl::Buffer buffer_A(context, CL_MEM_READ_WRITE, sizeof(int) * n);
	cl::Buffer buffer_B(context, CL_MEM_READ_WRITE, sizeof(int) * n);
	cl::Buffer buffer_C(context, CL_MEM_READ_WRITE, sizeof(int) * n);

	int* A = new int[n];
	int* B = new int[n];

	for (int i = 0; i < n; i++)
	{
		A[i] = i;
		B[i] = i * 1000;
	}

	//create queue to which we will push commands for the device.
	cl::CommandQueue queue(context, default_device);

	//write arrays A and B to the device
	queue.enqueueWriteBuffer(buffer_A, CL_TRUE, 0, sizeof(int) * n, A);
	queue.enqueueWriteBuffer(buffer_B, CL_TRUE, 0, sizeof(int) * n, B);


	//run the kernel
	cl::Kernel simple_add(program, "simple_add");
	simple_add.setArg(0, buffer_A);
	simple_add.setArg(1, buffer_B);
	simple_add.setArg(2, buffer_C);
	queue.enqueueNDRangeKernel(simple_add, cl::NullRange, cl::NDRange(n), cl::NullRange);
	queue.finish();


	int* C = new int[n];
	//read result C from the device to array C
	queue.enqueueReadBuffer(buffer_C, CL_TRUE, 0, sizeof(int) * n, C);

	std::cout << " result: \n";
	for (int i = 0; i < n; i++) {
		std::cout << C[i] << " ";
	}

	return 0;
}