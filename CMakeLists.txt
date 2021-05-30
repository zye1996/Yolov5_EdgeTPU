cmake_minimum_required(VERSION 3.11)
set(ProjectName "Yolo_EdgeTPU")
project(${ProjectName})

set(CMAKE_C_FLAGS "-Wall -pthread")
set(CMAKE_C_FLAGS_DEBUG "-g -O0")
set(CMAKE_C_FLAGS_RELEASE "-O3")
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -std=c++17 -lstdc++")
set(CMAKE_CXX_FLAGS_DEBUG ${CMAKE_C_FLAGS_DEBUG})
set(CMAKE_CXX_FLAGS_RELEASE ${CMAKE_C_FLAGS_RELEASE})

# Must match with commit that libedgetpu.so was built from.
set(TENSORFLOW_COMMIT 85c8b2a817f95a3e979ecd1ed95bff1dc1335cff)

# GET CPU Architechture
if(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "x86_64")
    set(EDGE_ARCH "k8")
    set(TF_COMMAND make -j8 BUILD_WITH_NNAPI=false -C ${CMAKE_BINARY_DIR}/tensorflow/src/tf -f tensorflow/lite/tools/make/Makefile lib)
    set(TF_INSTALL_PREFIX "linux_x86_64")
elseif(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "aarch64")
    set(EDGE_ARCH "${CMAKE_SYSTEM_PROCESSOR}")
    set(TF_INSTALL_PREFIX "generic-aarch64_armv8-a")
    set(CROSS_PREFIX "aarch64-linux-gnu-")
    set(TF_COMMAND make -j4 TARGET=generic-aarch64 TARGET_ARCH=armv8-a -C ${CMAKE_BINARY_DIR}/tensorflow/src/tf -f tensorflow/lite/tools/make/Makefile CC=${CROSS_PREFIX}g++ CXX=${CROSS_PREFIX}g++ AR=${CROSS_PREFIX}ar CFLAGS=-fpermissive lib)
elseif(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "armv7l")
    message("We are going to assume that this build is for the rpi")
    set(EDGE_ARCH "${CMAKE_SYSTEM_PROCESSOR}")
    set(TF_INSTALL_PREFIX "rpi_armv7l")
    set(CROSS_PREFIX "arm-linux-gnueabihf-")
    set(TF_COMMAND make -j4 TARGET=rpi TARGET_ARCH=armv7l -C ${CMAKE_BINARY_DIR}/tensorflow/src/tf -f tensorflow/lite/tools/make/Makefile CC=${CROSS_PREFIX}g++ CXX=${CROSS_PREFIX}g++ AR=${CROSS_PREFIX}ar CFLAGS=-fpermissive lib)
else()
    message(FATAL_ERROR "Not implemented to build for: ${CMAKE_SYSTEM_PROCESSOR}")
endif()

message("System ARCH is ${CMAKE_SYSTEM_PROCESSOR} ${EDGE_ARCH}")

include(ExternalProject)
message("${TF_COMMAND}")
ExternalProject_Add(tf
        GIT_REPOSITORY https://github.com/tensorflow/tensorflow
        GIT_TAG ${TENSORFLOW_COMMIT}
        PREFIX "tensorflow"
        CONFIGURE_COMMAND ./tensorflow/lite/tools/make/download_dependencies.sh
        BUILD_IN_SOURCE 1
        BUILD_COMMAND ${TF_COMMAND}
        INSTALL_COMMAND cp -f ${CMAKE_BINARY_DIR}/tensorflow/src/tf/tensorflow/lite/tools/make/gen/${TF_INSTALL_PREFIX}/lib/libtensorflow-lite.a ${CMAKE_BINARY_DIR}/
        )

# libedgetpu has armv7l as armv7a.
if(${EDGE_ARCH} STREQUAL "armv7l")
    set(EDGE_ARCH "armv7a")
endif()

set(TF_ET_SRC_LIB "${CMAKE_SOURCE_DIR}/libedgetpu/direct/${EDGE_ARCH}/libedgetpu.so.1.0")
set(TF_LITE_LIB "${CMAKE_BINARY_DIR}/libtensorflow-lite.a")

find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${CMAKE_SOURCE_DIR}/)
include_directories(${CMAKE_SOURCE_DIR}/src/)
include_directories(${CMAKE_SOURCE_DIR}/include/)
include_directories(${CMAKE_SOURCE_DIR}/libedgetpu/)
include_directories(${CMAKE_BINARY_DIR}/tensorflow/src/tf/)
include_directories(${CMAKE_BINARY_DIR}/tensorflow/src/tf/tensorflow/lite/tools/make/downloads/flatbuffers/include/)
include_directories(${CMAKE_BINARY_DIR}/glog_install/include)
link_directories(${CMAKE_BINARY_DIR})

add_subdirectory(abseil-cpp)

# create output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/out/${EDGE_ARCH})
add_custom_target(build-time-make-directory ALL
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

add_library(utils
        src/image_utils.h src/image_utils.cpp
        src/model_utils.h src/model_utils.cpp
        src/tflite_utils.h src/tflite_utils.cpp
        src/label_utils.h src/label_utils.cpp)
target_link_libraries(utils absl::strings)

add_executable(${PROJECT_NAME}
        src/main.cpp
        src/YoloDecoder.cpp
        src/YoloDecoder.h
        )
target_link_libraries(${PROJECT_NAME} utils ${OpenCV_LIBS} ${TF_LITE_LIB} ${TF_ET_SRC_LIB} -ldl)
add_dependencies(${PROJECT_NAME} tf utils)
