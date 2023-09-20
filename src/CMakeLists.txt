cmake_minimum_required(VERSION 3.12)
project(tuner)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(THREADS_PREFER_PTHREAD_FLAG ON)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g -fsanitize=address -D_GLIBCXX_DEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

find_package(Threads REQUIRED)

add_compile_options("-O3 -Wall -Wextra")
# add_compile_options("-Wall -Wextra -fsanitize=address -D_GLIBCXX_DEBUG") # debug

add_executable(tuner "main.cpp" "tuner.cpp" "threadpool.cpp" "engines/toy.cpp" "engines/toy_tapered.cpp" "engines/fourku.cpp")

target_link_libraries(tuner PRIVATE Threads::Threads)
