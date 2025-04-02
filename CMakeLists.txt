cmake_minimum_required(VERSION 3.10)
project(main LANGUAGES CXX)
enable_language(CXX)

# Set compiler flags for C and C++ (use CXX_FLAGS for C++ projects)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g")

# Enable compile commands for better IDE support
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Add your executable (use ${PROJECT_NAME} consistently)
add_executable(${PROJECT_NAME} src/RefCountedPtr.tpp src/main.cpp)

# Set output directory for the executable
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/out")

# Set C++ standard explicitly
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)