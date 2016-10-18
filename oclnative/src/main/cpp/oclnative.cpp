
#include <stdlib.h>
#include "oclnative.h"

#define PLATFORM_INFO_SIZE 100
#define DEVICE_INFO_SIZE 4096

#define CHECK_CL_STATUS(cl_status, message) if (cl_status != CL_SUCCESS) { \
		fprintf(stderr, "Failed: %d (%s)\n%s\n", cl_status, getErrorString(cl_status), message); \
		exit(1); \
	}

const char *getErrorString(cl_int error)
{
switch(error){
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

std::vector<void *> OclNative::getPlatformIds() {
    cl_platform_id *platforms = NULL;
    cl_uint num_platforms;

    cl_int cl_status = clGetPlatformIDs(0, NULL, &num_platforms);
    CHECK_CL_STATUS(cl_status, "Failed to retrieve platform ids");

    platforms = (cl_platform_id *) malloc(sizeof(cl_platform_id) * num_platforms);
    cl_status = clGetPlatformIDs(num_platforms, platforms, NULL);
	CHECK_CL_STATUS(cl_status, "Failed to retrieve platforms.");

    std::vector<void *>* result = new std::vector<void *>(num_platforms);
    for(int i=0; i<num_platforms; i++) {
        (*result)[i] = platforms[i];
    }

    free(platforms);
    return *result;
}

std::vector<std::string>* OclNative::getPlatformInfo(void* platformId) {
	std::vector<std::string> *result = new std::vector<std::string>();
	char buffer[PLATFORM_INFO_SIZE];
	cl_int cl_status;

	cl_status = clGetPlatformInfo(reinterpret_cast<cl_platform_id>(platformId), CL_PLATFORM_NAME, sizeof(buffer),
    	&buffer, NULL);
    CHECK_CL_STATUS(cl_status, "Failed to retrieve platform info.");
	(*result).insert(result->end(), std::string("CL_PLATFORM_NAME:") + std::string(buffer));

	cl_status = clGetPlatformInfo(reinterpret_cast<cl_platform_id>(platformId), CL_PLATFORM_VENDOR, sizeof(buffer),
    	&buffer, NULL);
    CHECK_CL_STATUS(cl_status, "Failed to retrieve platform info.");
	(*result).insert(result->end(), std::string("CL_PLATFORM_VENDOR:") + std::string(buffer));

	cl_status = clGetPlatformInfo(reinterpret_cast<cl_platform_id>(platformId), CL_PLATFORM_VERSION, sizeof(buffer),
    	&buffer, NULL);
	CHECK_CL_STATUS(cl_status, "Failed to retrieve platform info.");
	(*result).insert(result->end(), std::string("CL_PLATFORM_VERSION:") + std::string(buffer));

	return result;
}

std::string OclNative::getPlatformExtensions(void* platformId) {
	char *ext_data;
	size_t ext_size;
	cl_int cl_status;

	cl_status = clGetPlatformInfo(reinterpret_cast<cl_platform_id>(platformId),
		CL_PLATFORM_EXTENSIONS, 0, NULL, &ext_size);
	CHECK_CL_STATUS(cl_status, "Failed to retrieve numer of platform extensions.");

	ext_data = (char*)malloc(ext_size);
	cl_status = clGetPlatformInfo(reinterpret_cast<cl_platform_id>(platformId),
		CL_PLATFORM_EXTENSIONS, ext_size, ext_data, NULL);
	CHECK_CL_STATUS(cl_status, "Failed to retrieve platform extensions.");

	return std::string(ext_data);
}

std::vector<void *> OclNative::getDeviceIds(void * platformId) {
	cl_device_id *cl_devices;
	cl_uint num_devices;
	cl_int cl_status;

	cl_status = clGetDeviceIDs(reinterpret_cast<cl_platform_id>(platformId),
		CL_DEVICE_TYPE_ALL, 5, NULL, &num_devices);
	CHECK_CL_STATUS(cl_status, "Failed to retrieve number of devices.");

	cl_devices = (cl_device_id *)malloc(sizeof(cl_device_id) * num_devices);
	cl_status = clGetDeviceIDs(reinterpret_cast<cl_platform_id>(platformId),
		CL_DEVICE_TYPE_ALL, num_devices, cl_devices, NULL);
	CHECK_CL_STATUS(cl_status, "Failed to retrieve device ids.");

   	std::vector<void *>* result = new std::vector<void *>(num_devices);
    for(int i=0; i<num_devices; i++) {
    	(*result)[i] = cl_devices[i];
    }

    free(cl_devices);
    return *result;
}

std::vector<std::string>* OclNative::getDeviceInfo(void* deviceId) {
	std::vector<std::string> *result = new std::vector<std::string>();
	char buffer[DEVICE_INFO_SIZE];
	size_t size_value;
	cl_uint uint_value;
	size_t *sizep_value;
	cl_int cl_status;

	cl_status = clGetDeviceInfo(reinterpret_cast<cl_device_id>(deviceId), CL_DEVICE_NAME, sizeof(buffer),
    	&buffer, NULL);
	CHECK_CL_STATUS(cl_status, "Failed to retrieve CL_DEVICE_NAME.");
	(*result).insert(result->end(), std::string("CL_DEVICE_NAME:") + std::string(buffer));

	cl_status = clGetDeviceInfo(reinterpret_cast<cl_device_id>(deviceId), CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(size_value),
    	&size_value, NULL);
	CHECK_CL_STATUS(cl_status, "Failed to retrieve CL_DEVICE_MAX_WORK_GROUP_SIZE.");
	sprintf(buffer, "%ld", size_value);
	(*result).insert(result->end(), std::string("CL_DEVICE_MAX_WORK_GROUP_SIZE:") + std::string(buffer));

	cl_status = clGetDeviceInfo(reinterpret_cast<cl_device_id>(deviceId), CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS, sizeof(uint_value),
    	&uint_value, NULL);
	CHECK_CL_STATUS(cl_status, "Failed to retrieve CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS.");
	sprintf(buffer, "%d", uint_value);
	(*result).insert(result->end(), std::string("CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS:") + std::string(buffer));

	sizep_value = (size_t *)malloc(sizeof(size_t) * uint_value);
	cl_status = clGetDeviceInfo(reinterpret_cast<cl_device_id>(deviceId), CL_DEVICE_MAX_WORK_ITEM_SIZES, sizeof(size_t) * uint_value,
    	sizep_value, NULL);
	CHECK_CL_STATUS(cl_status, "Failed to retrieve CL_DEVICE_MAX_WORK_ITEM_SIZES.");
	int i;
	fprintf(stderr, "\n-------\nNumber of DIMs: %ld\n", uint_value);
	for(i=0; i<uint_value; i++) {
		fprintf(stderr, "%ld\n", sizep_value[i]);
	}

	return result;
}

void* OclNative::createContext(void* deviceId) {
	cl_device_id devices[] = { reinterpret_cast<cl_device_id>(deviceId) };
	cl_context context;
	cl_int cl_status;

	context = clCreateContext(NULL, 1, devices, NULL, NULL, &cl_status);
	CHECK_CL_STATUS(cl_status, "Failed to create context.");

	return context;
}

void* OclNative::createProgram(void* contextId, std::string source) {
	cl_int cl_status;
	const char *program_buffer = source.c_str();
	size_t program_lengths[] = {source.length()};
	cl_program program;

	program = clCreateProgramWithSource(reinterpret_cast<cl_context>(contextId), 1, (const char**)&program_buffer,
		program_lengths, &cl_status);
	CHECK_CL_STATUS(cl_status, "Failed to create program.");

	return program;
}

void OclNative::buildProgram(void* deviceId, void* programId) {
	cl_device_id devices[] = { reinterpret_cast<cl_device_id>(deviceId) };
	cl_int cl_status;

	cl_status = clBuildProgram(reinterpret_cast<cl_program>(programId), 1, devices, NULL, NULL, NULL);
	CHECK_CL_STATUS(cl_status, "Failed to build program.");
}

void* OclNative::createKernel(void* programId, std::string kernelName) {
	cl_int cl_status;
	cl_kernel kernel;

	kernel = clCreateKernel(reinterpret_cast<cl_program>(programId), kernelName.c_str(), &cl_status);
	CHECK_CL_STATUS(cl_status, "Failed to create kernel");

	return kernel;
}

void* OclNative::createCommandQueue(void* deviceId, void* contextId) {
	cl_int cl_status;
	cl_command_queue queue;

	queue = clCreateCommandQueue(reinterpret_cast<cl_context>(contextId), reinterpret_cast<cl_device_id>(deviceId),
		0, &cl_status);
	CHECK_CL_STATUS(cl_status, "Failed to create command queue.");

	return queue;
}

std::vector<float> OclNative::executeMatrixMultiplication(void* context, void* commands, void* kernelId, std::string deviceName, int ncols, std::vector<float> matrix1, std::vector<float> matrix2) {
	cl_int cl_status;
	cl_uint mem_size_A = sizeof(float) * matrix1.size();
	cl_uint mem_size_B = sizeof(float) * matrix2.size();

 	float* h_A = &matrix1[0];
	float* h_B = &matrix2[0];

	cl_uint size_C = ncols * ncols;
	cl_uint mem_size_C = sizeof(float) * size_C;
	float* h_C = (float*)malloc(mem_size_C);

    // OpenCL device memory for matrices
   	cl_mem d_A;
   	cl_mem d_B;
   	cl_mem d_C;

	// Create the input and output arrays in device memory for our calculation
   	d_C = clCreateBuffer(reinterpret_cast<cl_context>(context), CL_MEM_READ_WRITE, mem_size_A, NULL, &cl_status);
   	CHECK_CL_STATUS(cl_status, "Failed to create buffer d_C");

   	d_A = clCreateBuffer(reinterpret_cast<cl_context>(context), CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, mem_size_A, h_A, &cl_status);
	CHECK_CL_STATUS(cl_status, "Failed to create buffer d_A");

   	d_B = clCreateBuffer(reinterpret_cast<cl_context>(context), CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, mem_size_B, h_B, &cl_status);
	CHECK_CL_STATUS(cl_status, "Failed to create buffer d_B");

   	if (!d_A || !d_B || !d_C) {
       perror("Error: Failed to allocate device memory!\n");
       exit(1);
   	}

   	//Launch OpenCL kernel
   	size_t localWorkSize[2], globalWorkSize[2];
   	int wA = ncols;
   	int wC = ncols;

   	cl_status = clSetKernelArg(reinterpret_cast<cl_kernel>(kernelId), 0, sizeof(cl_mem), (void *)&d_C);
   	cl_status |= clSetKernelArg(reinterpret_cast<cl_kernel>(kernelId), 1, sizeof(cl_mem), (void *)&d_A);
   	cl_status |= clSetKernelArg(reinterpret_cast<cl_kernel>(kernelId), 2, sizeof(cl_mem), (void *)&d_B);
   	cl_status |= clSetKernelArg(reinterpret_cast<cl_kernel>(kernelId), 3, sizeof(int), (void *)&wA);
   	cl_status |= clSetKernelArg(reinterpret_cast<cl_kernel>(kernelId), 4, sizeof(int), (void *)&wC);

   	if (cl_status != CL_SUCCESS) {
       printf("Error: Failed to set kernel arguments! %d\n", cl_status);
       exit(1);
   	}

	if (deviceName.find("Intel") != std::string::npos) {
		fprintf(stderr, "Setting dimensions for Intel\n");
		localWorkSize[0] = 16;
		localWorkSize[1] = 1;
		globalWorkSize[0] = 1024;
		globalWorkSize[1] = 1024;
  	} else if (deviceName.find("Iris") != std::string::npos) {
  		fprintf(stderr, "Setting dimensions for Iris\n");
		localWorkSize[0] = 16;
		localWorkSize[1] = 16;
		globalWorkSize[0] = 1024;
		globalWorkSize[1] = 1024;
  	} else {
  		fprintf(stderr, "Setting dimensions for nVidia\n");
		localWorkSize[0] = 16;
		localWorkSize[1] = 16;
		globalWorkSize[0] = 1024;
		globalWorkSize[1] = 1024;
  	}

	for(int i=0; i<10; i++) {
		cl_status = clEnqueueNDRangeKernel(reinterpret_cast<cl_command_queue>(commands), reinterpret_cast<cl_kernel>(kernelId),
			2, NULL, globalWorkSize, localWorkSize, 0, NULL, NULL);
		CHECK_CL_STATUS(cl_status, "Failed to execute kernel!");
	}

	//Retrieve result from device
	cl_status = clEnqueueReadBuffer(reinterpret_cast<cl_command_queue>(commands), d_C, CL_TRUE, 0, mem_size_C, h_C, 0, NULL, NULL);
	CHECK_CL_STATUS(cl_status, "Failed to read output array");

	return std::vector<float>(0.0f);
}




