# Minimum CMake version required
cmake_minimum_required(VERSION 3.10)

# Project name and version
project(Lab2 VERSION 1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "-g")

# Add source files
file(GLOB SOURCES ${PROJECT_SOURCE_DIR}/main.cpp)

#find OPENMP
find_package(OpenMP)

# Add the executable
add_executable(Lab2 ${SOURCES})

include_directories(${PROJECT_SOURCE_DIR}/../SFML/include)

link_directories(${PROJECT_SOURCE_DIR}/../SFML/lib)

# Link the executable to the libraries in the lib directory
target_link_libraries(Lab2 PUBLIC sfml-graphics sfml-system sfml-window)
if(OpenMP_CXX_FOUND)
    target_link_libraries(Lab2 PUBLIC OpenMP::OpenMP_CXX)
endif()
