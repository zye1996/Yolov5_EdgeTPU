# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.19.2)

# We name the project and the target for the ExternalProject_Add() call
# to something that will highlight to the user what we are working on if
# something goes wrong and an error message is produced.

project(eigen-populate NONE)

include(ExternalProject)
ExternalProject_Add(eigen-populate
                     "UPDATE_DISCONNECTED" "False" "GIT_REPOSITORY" "https://gitlab.com/libeigen/eigen" "GIT_TAG" "d10b27fe37736d2944630ecd7557cefa95cf87c9" "GIT_PROGRESS" "TRUE" "PREFIX" "/home/yzy/CLionProjects/Yolo_EdgeTPU/cmake-build-debug"
                    SOURCE_DIR          "/home/yzy/CLionProjects/Yolo_EdgeTPU/cmake-build-debug/eigen"
                    BINARY_DIR          "/home/yzy/CLionProjects/Yolo_EdgeTPU/cmake-build-debug/_deps/eigen-build"
                    CONFIGURE_COMMAND   ""
                    BUILD_COMMAND       ""
                    INSTALL_COMMAND     ""
                    TEST_COMMAND        ""
                    USES_TERMINAL_DOWNLOAD  YES
                    USES_TERMINAL_UPDATE    YES
)


