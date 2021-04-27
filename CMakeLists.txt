cmake_minimum_required(VERSION 3.16)

set(ProjectName "Yolo_EdgeTPU")
project(${ProjectName})

set(CMAKE_CXX_STANDARD 11)
set(TENSORFLOW_SOURCE_DIR "/home/yzy/libs/tensorflow-2.4.1")

# Get CPU Architechture
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

# libedgetpu has armv7l as armv7a.
if(${EDGE_ARCH} STREQUAL "armv7l")
    set(EDGE_ARCH "armv7a")
endif()

# Include tflite
if(NOT TENSORFLOW_SOURCE_DIR)
    get_filename_component(TENSORFLOW_SOURCE_DIR
            "${CMAKE_CURRENT_LIST_DIR}/../../../../"
            ABSOLUTE)
endif()

add_subdirectory(
        "${TENSORFLOW_SOURCE_DIR}/tensorflow/lite"
        "${CMAKE_CURRENT_BINARY_DIR}/tensorflow-lite" EXCLUDE_FROM_ALL)


set(TF_LITE_LIB "${CMAKE_BINARY_DIR}/libtensorflow-lite.a")

include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${CMAKE_SOURCE_DIR}/)
include_directories(${CMAKE_SOURCE_DIR}/libedgetpu/)
include_directories(${CMAKE_SOURCE_DIR}/libedgetpu/tflite/public)


# Executable
add_executable(Yolo_EdgeTPU main.cpp YoloDetector.cpp YoloDetector.h utils/tflite_utils.h utils/tflite_utils.cpp utils/model_utils.h utils/model_utils.cpp YoloDecoder.cpp YoloDecoder.h anchor.h utils/label_utils.h utils/label_utils.cpp utils/image_utils.h utils/image_utils.cpp)

# For OpenCV
find_package(OpenCV REQUIRED)
target_include_directories(${ProjectName} PUBLIC ${OpenCV_INCLUDE_DIRS})
target_link_libraries(${ProjectName} ${OpenCV_LIBS})

# For Glog
find_package (glog 0.5.0 REQUIRED)
target_link_libraries(${ProjectName} glog::glog)

# For EdgeTPU
set(TF_ET_SRC_LIB "${CMAKE_SOURCE_DIR}/libedgetpu/out/direct/${EDGE_ARCH}/libedgetpu.so.1.0")
target_link_libraries(${ProjectName} ${TF_ET_SRC_LIB})

# For TFLITE
target_link_libraries(Yolo_EdgeTPU tensorflow-lite ${CMAKE_DL_LIBS})

# OUTPUT DIR
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/out/${EDGE_ARCH})
