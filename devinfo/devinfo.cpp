

#include <stdio.h>

#ifdef __APPLE__
#include <OpenCL/cl.h>
#else
#include <CL/cl.h>
#endif

int main(int argc, char *argv[]) {
  fprintf(stdout, "Hello World\n");
}
