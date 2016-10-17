
#include <stdlib.h>
#include "oclnative.h"

#define PLATFORM_INFO_SIZE 100
#define DEVICE_INFO_SIZE 4096

#define CHECK_CL_STATUS(cl_status, message) if (cl_status < 0) { \
		perror(message); \
		exit(1); \
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
	CHECK_CL_STATUS(cl_status, "Failed to retrieve platform extensions.");

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
	CHECK_CL_STATUS(cl_status, "Failed to retrieve device ids.");

	cl_devices = (cl_device_id *)malloc(sizeof(cl_device_id) * num_devices);
	cl_status = clGetDeviceIDs(reinterpret_cast<cl_platform_id>(platformId),
		CL_DEVICE_TYPE_ALL, 5, cl_devices, NULL);
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
	cl_int cl_status;

	cl_status = clGetDeviceInfo(reinterpret_cast<cl_device_id>(deviceId), CL_DEVICE_NAME, sizeof(buffer),
    	&buffer, NULL);
	CHECK_CL_STATUS(cl_status, "Failed to retrieve device info.");
	(*result).insert(result->end(), std::string("CL_DEVICE_NAME:") + std::string(buffer));

	return result;
}

void* OclNative::createContext(void* deviceId) {
	cl_context_properties context_props[] = { 0 };
	cl_device_id devices[1];
	cl_context context;
	cl_int cl_status;

	devices[0] = (cl_device_id)deviceId;

	context = clCreateContext(context_props, 1, devices, NULL, NULL, &cl_status);
	CHECK_CL_STATUS(cl_status, "Failed to create context.");

	return context;
}

void* OclNative::createProgram(void* contextId, std::string source) {
	cl_int cl_status;
	const char *program_buffer = source.c_str();
	size_t program_lengths[] = {source.length()};
	cl_program program;

	program = clCreateProgramWithSource(reinterpret_cast<cl_context>(contextId), 1, (const char**)program_buffer,
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

float OclNative::executeDotProduct(void* programId, std::vector<float> vector1, std::vector<float> vector2) {
	return 0.0f;
}
