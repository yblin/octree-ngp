# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

# compile CUDA with /usr/bin/nvcc
CUDA_DEFINES = -DGLFW_INCLUDE_VULKAN -DNGP_GUI -DNGP_OPTIX -DNGP_VERSION=\"1.0dev\" -DNGP_VULKAN -DTCNN_MIN_GPU_ARCH=86

CUDA_INCLUDES = -I/home/yblin/Documents/GitHub/instant-ngp/dependencies/dlss/include -I/home/yblin/Documents/GitHub/instant-ngp/dependencies/glfw/include -I/home/yblin/Documents/GitHub/instant-ngp/dependencies/imgui -I/home/yblin/Documents/GitHub/instant-ngp/dependencies -I/home/yblin/Documents/GitHub/instant-ngp/dependencies/filesystem -I/home/yblin/Documents/GitHub/instant-ngp/dependencies/glm -I/home/yblin/Documents/GitHub/instant-ngp/dependencies/nanovdb -I/home/yblin/Documents/GitHub/instant-ngp/dependencies/NaturalSort -I/home/yblin/Documents/GitHub/instant-ngp/dependencies/tinylogger -I/home/yblin/Documents/GitHub/instant-ngp/dependencies/codelibrary -I/home/yblin/Documents/GitHub/instant-ngp/dependencies/optix -I/home/yblin/Documents/GitHub/instant-ngp/include -I/home/yblin/Documents/GitHub/instant-ngp/build -I/home/yblin/Documents/GitHub/instant-ngp/dependencies/zstr/src -I/home/yblin/Documents/GitHub/instant-ngp/dependencies/tiny-cuda-nn/include -I/home/yblin/Documents/GitHub/instant-ngp/dependencies/tiny-cuda-nn/dependencies -I/home/yblin/Documents/GitHub/instant-ngp/dependencies/tiny-cuda-nn/dependencies/cutlass/include -I/home/yblin/Documents/GitHub/instant-ngp/dependencies/tiny-cuda-nn/dependencies/cutlass/tools/util/include -I/home/yblin/Documents/GitHub/instant-ngp/dependencies/tiny-cuda-nn/dependencies/fmt/include

CUDA_FLAGS = -O3 -DNDEBUG --generate-code=arch=compute_86,code=[compute_86,sm_86] -Xcompiler=-Wno-float-conversion -Xcompiler=-fno-strict-aliasing -Xcompiler=-fms-extensions -Xcompiler=-fPIC --extended-lambda --expt-relaxed-constexpr --use_fast_math -Xcudafe=--diag_suppress=unrecognized_gcc_pragma -std=c++14

