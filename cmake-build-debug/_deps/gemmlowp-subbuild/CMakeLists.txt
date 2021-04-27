# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.19.2)

# We name the project and the target for the ExternalProject_Add() call
# to something that will highlight to the user what we are working on if
# something goes wrong and an error message is produced.

project(gemmlowp-populate NONE)

include(ExternalProject)
ExternalProject_Add(gemmlowp-populate
                     "UPDATE_DISCONNECTED" "False" "GIT_REPOSITORY" "https://github.com/google/gemmlowp" "GIT_TAG" "fda83bdc38b118cc6b56753bd540caa49e570745" "GIT_PROGRESS" "TRUE"
                    SOURCE_DIR          "/home/yzy/CLionProjects/Yolo_EdgeTPU/cmake-build-debug/gemmlowp"
                    BINARY_DIR          "/home/yzy/CLionProjects/Yolo_EdgeTPU/cmake-build-debug/_deps/gemmlowp-build"
                    CONFIGURE_COMMAND   ""
                    BUILD_COMMAND       ""
                    INSTALL_COMMAND     ""
                    TEST_COMMAND        ""
                    USES_TERMINAL_DOWNLOAD  YES
                    USES_TERMINAL_UPDATE    YES
)


