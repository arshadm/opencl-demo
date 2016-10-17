
#ifndef OCLNATIVE_H
#define OCLNATIVE_H

#ifdef __APPLE__
#include <OpenCL/cl.h>
#else
#include <CL/cl.h>
#endif

#include <string>
#include <vector>

typedef std::vector<std::vector<float>> FloatVectorVector;

class OclNative {
public:
    std::vector<void *> getPlatformIds();

    std::vector<std::string>* getPlatformInfo(void* platformId);

    std::string getPlatformExtensions(void* platformId);

    std::vector<void *> getDeviceIds(void * platformId);

    std::vector<std::string>* getDeviceInfo(void* deviceId);

    void* createContext(void* deviceId);

    void* createProgram(void* contextId, std::string source);

	void buildProgram(void* deviceId, void* programId);

	void* createKernel(void* programId, std::string kernelName);

    void* createCommandQueue(void* deviceId, void* contextId);

    float executeDotProduct(void* programId, std::vector<float> vector1, std::vector<float> vector2);
};

#endif /*! OCLNATIVE_H */
