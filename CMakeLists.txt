cmake_minimum_required(VERSION 3.10)
project(MyApp)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Output directories for executables and libraries
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Include directories for header files
include_directories(include src)

# Find all source files (.cpp) recursively
file(GLOB_RECURSE SOURCES
    "src/*.cpp"
    "main.cpp"
)

# Create the executable from source files
add_executable(main ${SOURCES})

# Set default build type to Debug if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# Create the build and bin directories if they don't exist
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/build)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Run target after build
add_custom_target(run
    COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/main
    DEPENDS main
    COMMENT "Running the program"
)

# Set object files output directory
set(CMAKE_OBJECT_PATH_SUFFIX "/build")

# Display a message to confirm setup
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Executable output: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/main")
