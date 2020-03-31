cmake_minimum_required(VERSION 3.10)
project(qpoases-python CXX)

if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
   message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
   file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/master/conan.cmake"
                  "${CMAKE_BINARY_DIR}/conan.cmake")
endif()

include(${CMAKE_BINARY_DIR}/conan.cmake)

conan_cmake_run(REQUIRES 
                    eigen/3.3.7@conan/stable
                    pybind11/2.3.0@conan/stable
                BASIC_SETUP)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(pybind11 2.3.0 REQUIRED)
find_package(Eigen3 3.3 REQUIRED)

file(GLOB_RECURSE SOURCES src/*.cpp)

add_library(_qpoases MODULE ${SOURCES})

target_compile_definitions(_qpoases
                           PRIVATE
                           -D__NO_COPYRIGHT__
                           -D__SUPPRESSANYOUTPUT__
                           )

target_include_directories(_qpoases
    PRIVATE include
)

target_link_libraries(_qpoases
    PRIVATE 
            pybind11::module
            Eigen3::Eigen
)

set_target_properties(_qpoases
    PROPERTIES	
        PREFIX "${PYTHON_MODULE_PREFIX}"	
        SUFFIX "${PYTHON_MODULE_EXTENSION}"	
)

install(TARGETS _qpoases LIBRARY DESTINATION modules/_qpoases)
