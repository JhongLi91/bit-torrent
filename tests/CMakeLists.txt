cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 20)

file(GLOB tests ${CMAKE_CURRENT_SOURCE_DIR}/test*.cpp)
foreach(src IN LISTS tests)
  get_filename_component(name ${src} NAME_WE)

  add_executable(${name} ${src})
  target_include_directories(${name} PRIVATE ${PROJECT_SOURCE_DIR})

  target_link_libraries(${name} PRIVATE core_lib)
  add_test(NAME ${name} COMMAND ${name})
endforeach()
