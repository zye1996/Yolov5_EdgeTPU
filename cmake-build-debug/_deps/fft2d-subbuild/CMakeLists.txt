# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.19.2)

# We name the project and the target for the ExternalProject_Add() call
# to something that will highlight to the user what we are working on if
# something goes wrong and an error message is produced.

project(fft2d-populate NONE)

include(ExternalProject)
ExternalProject_Add(fft2d-populate
                     "UPDATE_DISCONNECTED" "False" "URL" "https://storage.googleapis.com/mirror.tensorflow.org/www.kurims.kyoto-u.ac.jp/~ooura/fft2d.tgz" "URL_HASH" "SHA256=ada7e99087c4ed477bfdf11413f2ba8db8a840ba9bbf8ac94f4f3972e2a7cec9"
                    SOURCE_DIR          "/home/yzy/CLionProjects/Yolo_EdgeTPU/cmake-build-debug/fft2d"
                    BINARY_DIR          "/home/yzy/CLionProjects/Yolo_EdgeTPU/cmake-build-debug/_deps/fft2d-build"
                    CONFIGURE_COMMAND   ""
                    BUILD_COMMAND       ""
                    INSTALL_COMMAND     ""
                    TEST_COMMAND        ""
                    USES_TERMINAL_DOWNLOAD  YES
                    USES_TERMINAL_UPDATE    YES
)


