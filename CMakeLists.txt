cmake_minimum_required(VERSION 3.10)

project(PongGame LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# --- SDL3 Setup ---
# find SDL3, SDL3_ttf, SDL3_image using find_package
find_package(SDL3 REQUIRED)
find_package(SDL3_ttf REQUIRED)
find_package(SDL3_image REQUIRED)


# Add source files
aux_source_directory(src DIR_SRCS)

add_executable(PongGame ${DIR_SRCS})

# Link SDL3 libraries
target_link_libraries(PongGame PRIVATE SDL3::SDL3 SDL3_ttf::SDL3_ttf SDL3_image::SDL3_image)


# Define output directory for the executable
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out)

