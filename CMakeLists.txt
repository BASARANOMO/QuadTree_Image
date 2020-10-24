cmake_minimum_required(VERSION 2.6)
file(TO_CMAKE_PATH "$ENV{IMAGINEPP_ROOT}/CMake" p)
list(APPEND CMAKE_MODULE_PATH "${p}") #For old Imagine++
list(APPEND CMAKE_SYSTEM_FRAMEWORK_PATH /Library/Frameworks) #Mac, why not auto?
find_package(Imagine REQUIRED)

set(CMAKE_PREFIX_PATH $ENV{HOME}/Qt/5.12.9/clang_64)

project(ZipImage)

add_executable(test_quadtree quadtree/example.cpp quadtree/quadtree.h)

add_executable(zipimage example.cpp quadtree/quadtree.h)
ImagineUseModules(zipimage Graphics)
