cmake_minimum_required(VERSION 3.20)
project(course_planner VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(course_planner_lib
    src/binary_search_tree.cpp
    src/course_loader.cpp)
target_include_directories(course_planner_lib PUBLIC include)

function(enable_project_warnings target)
  if(MSVC)
    target_compile_options(${target} PRIVATE /W4 /permissive-)
  else()
    target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic -Wconversion)
  endif()
endfunction()
enable_project_warnings(course_planner_lib)

add_executable(course_planner src/main.cpp)
target_link_libraries(course_planner PRIVATE course_planner_lib)
enable_project_warnings(course_planner)

include(CTest)
if(BUILD_TESTING)
  add_executable(course_planner_verification tests/course_planner_tests.cpp)
  target_link_libraries(course_planner_verification PRIVATE course_planner_lib)
  enable_project_warnings(course_planner_verification)
  add_test(NAME course_planner_verification COMMAND course_planner_verification)
endif()
