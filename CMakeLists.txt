cmake_minimum_required(VERSION 3.20)

project(job-engine
VERSION 1.0
LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(job_engine
 src/main.cpp
)
target_include_directories(job_engine PRIVATE
  include

)
add_executable(job_engine_test
tests/JobEngineTest.cpp
)

target_include_directories(job_engine_test PRIVATE
include
)
add_executable(job_server
examples/server.cpp
)
target_include_directories(job_server PRIVATE 
include)
target_link_libraries(job_server PRIVATE
ws2_32)
add_executable(job_client
examples/client.cpp
)
target_link_libraries(job_client PRIVATE 
ws2_32)