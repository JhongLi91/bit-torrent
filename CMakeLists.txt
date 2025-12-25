cmake_minimum_required(VERSION 3.16)
project(bit_torrent)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Common output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

add_library(core_lib
    src/bencoding/bencoding.cpp
)

target_include_directories(core_lib PUBLIC include)

add_executable(bit_torrent src/main.cpp)
target_link_libraries(bit_torrent PRIVATE core_lib)


# copy assets to build
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${PROJECT_SOURCE_DIR}/assets
    ${CMAKE_BINARY_DIR}/assets
    COMMENT "Copying assets to build directory..."
)

# tests
enable_testing()
add_subdirectory(tests)
