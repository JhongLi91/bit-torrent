cmake_minimum_required(VERSION 3.15)
project(BitTorrent VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(lib
    src/bencoding.cpp
)

target_include_directories(lib PUBLIC include)

add_executable(bit_torrent src/main.cpp)
target_link_libraries(bit_torrent PRIVATE lib)

enable_testing()
add_subdirectory(tests)
