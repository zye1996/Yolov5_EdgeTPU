
if(NOT "/home/yzy/CLionProjects/Yolo_EdgeTPU/cmake-build-debug/_deps/neon2sse-subbuild/neon2sse-populate-prefix/src/neon2sse-populate-stamp/neon2sse-populate-gitinfo.txt" IS_NEWER_THAN "/home/yzy/CLionProjects/Yolo_EdgeTPU/cmake-build-debug/_deps/neon2sse-subbuild/neon2sse-populate-prefix/src/neon2sse-populate-stamp/neon2sse-populate-gitclone-lastrun.txt")
  message(STATUS "Avoiding repeated git clone, stamp file is up to date: '/home/yzy/CLionProjects/Yolo_EdgeTPU/cmake-build-debug/_deps/neon2sse-subbuild/neon2sse-populate-prefix/src/neon2sse-populate-stamp/neon2sse-populate-gitclone-lastrun.txt'")
  return()
endif()

execute_process(
  COMMAND ${CMAKE_COMMAND} -E rm -rf "/home/yzy/CLionProjects/Yolo_EdgeTPU/cmake-build-debug/neon2sse"
  RESULT_VARIABLE error_code
  )
if(error_code)
  message(FATAL_ERROR "Failed to remove directory: '/home/yzy/CLionProjects/Yolo_EdgeTPU/cmake-build-debug/neon2sse'")
endif()

# try the clone 3 times in case there is an odd git clone issue
set(error_code 1)
set(number_of_tries 0)
while(error_code AND number_of_tries LESS 3)
  execute_process(
    COMMAND "/usr/bin/git"  clone --no-checkout --depth 1 --no-single-branch --progress "https://github.com/intel/ARM_NEON_2_x86_SSE" "neon2sse"
    WORKING_DIRECTORY "/home/yzy/CLionProjects/Yolo_EdgeTPU/cmake-build-debug"
    RESULT_VARIABLE error_code
    )
  math(EXPR number_of_tries "${number_of_tries} + 1")
endwhile()
if(number_of_tries GREATER 1)
  message(STATUS "Had to git clone more than once:
          ${number_of_tries} times.")
endif()
if(error_code)
  message(FATAL_ERROR "Failed to clone repository: 'https://github.com/intel/ARM_NEON_2_x86_SSE'")
endif()

execute_process(
  COMMAND "/usr/bin/git"  checkout master --
  WORKING_DIRECTORY "/home/yzy/CLionProjects/Yolo_EdgeTPU/cmake-build-debug/neon2sse"
  RESULT_VARIABLE error_code
  )
if(error_code)
  message(FATAL_ERROR "Failed to checkout tag: 'master'")
endif()

set(init_submodules TRUE)
if(init_submodules)
  execute_process(
    COMMAND "/usr/bin/git"  submodule update --recursive --init 
    WORKING_DIRECTORY "/home/yzy/CLionProjects/Yolo_EdgeTPU/cmake-build-debug/neon2sse"
    RESULT_VARIABLE error_code
    )
endif()
if(error_code)
  message(FATAL_ERROR "Failed to update submodules in: '/home/yzy/CLionProjects/Yolo_EdgeTPU/cmake-build-debug/neon2sse'")
endif()

# Complete success, update the script-last-run stamp file:
#
execute_process(
  COMMAND ${CMAKE_COMMAND} -E copy
    "/home/yzy/CLionProjects/Yolo_EdgeTPU/cmake-build-debug/_deps/neon2sse-subbuild/neon2sse-populate-prefix/src/neon2sse-populate-stamp/neon2sse-populate-gitinfo.txt"
    "/home/yzy/CLionProjects/Yolo_EdgeTPU/cmake-build-debug/_deps/neon2sse-subbuild/neon2sse-populate-prefix/src/neon2sse-populate-stamp/neon2sse-populate-gitclone-lastrun.txt"
  RESULT_VARIABLE error_code
  )
if(error_code)
  message(FATAL_ERROR "Failed to copy script-last-run stamp file: '/home/yzy/CLionProjects/Yolo_EdgeTPU/cmake-build-debug/_deps/neon2sse-subbuild/neon2sse-populate-prefix/src/neon2sse-populate-stamp/neon2sse-populate-gitclone-lastrun.txt'")
endif()

