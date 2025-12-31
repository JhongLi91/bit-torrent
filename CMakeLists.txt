cmake_minimum_required(VERSION 3.16)
project(bit_torrent)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Common output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

# fetch external libraries
include(FetchContent)
find_package(Boost REQUIRED thread)
find_package(spdlog QUIET)
if(NOT spdlog_FOUND)
  message(STATUS "Fetching spdlog")
  FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG v1.14.1)
  FetchContent_MakeAvailable(spdlog)
endif()

# add code to core_lib
add_library(core_lib
    # parsing
    src/parsing/bencoding.cpp
    src/parsing/torrent.cpp
    src/parsing/buffer.cpp

    # network
    src/network/http.cpp
    src/network/network_utils.cpp
    src/network/tcp.cpp
)
add_library(boost INTERFACE)

target_include_directories(boost INTERFACE ${Boost_INCLUDE_DIRS})
target_include_directories(core_lib PUBLIC include)

target_link_libraries(core_lib PUBLIC 
    spdlog::spdlog
    Boost::thread
)

# main executable
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
