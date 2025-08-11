cmake_minimum_required(VERSION 4.0.2)

if(NOT DEFINED PROJECT_VERSION)
set(PROJECT_VERSION "1.0.0")
else()
set(PROJECT_VERSION "${PROJECT_VERSION}" CACHE STRING "Override project version")
endif()

project(sp-monitor VERSION ${PROJECT_VERSION} LANGUAGES C CXX)
message(STATUS "Building project version: ${PROJECT_VERSION}")


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

message(DEBUG "C++ compiler: ${CMAKE_CXX_COMPILER}")
message(DEBUG "C++ compiler version: ${CMAKE_CXX_COMPILER_VERSION}")

include(GNUInstallDirs)

# Installing GTKMM library
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTKMM REQUIRED gtkmm-4.0)

include_directories(${GTKMM_INCLUDE_DIRS})
link_directories(${GTKMM_LIBRARY_DIRS})
add_definitions(${GTKMM_CFLAGS_OTHER})

include(lib/external-dependencies.cmake)

# add_library(sp-monitor-lib
#     src/serial_port.cpp
#     src/serial_port.h
# )

add_subdirectory(resources)
include_directories("${CMAKE_CURRENT_BINARY_DIR}/resources") ## It allow to #include "resources.c"

add_subdirectory(scripts)

# Use external dependencies => libserialport 
ExternalProject_Get_property(libserialport INSTALL_DIR)
message(STATUS "Install directory of libserialport = ${INSTALL_DIR}")
include_directories(${INSTALL_DIR}/include)

# Add sources files
file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.h")
list(REMOVE_ITEM SOURCES "src/main.cpp")
add_library(sources_files STATIC ${SOURCES})
add_dependencies(sources_files libserialport)
target_link_libraries(sources_files PUBLIC resources_files ${INSTALL_DIR}/lib/libserialport.a)

# Main executable
add_executable(sp-monitor src/main.cpp)
target_link_libraries(sp-monitor resources_files sources_files ${GTKMM_LIBRARIES})
add_dependencies(sp-monitor compile_schemas) 
install(TARGETS sp-monitor DESTINATION bin)

# Enable CTest
include(CTest)
enable_testing()

# Packaging support
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
# set(CPACK_PACKAGE_VENDOR "Vendor name")

set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "A basic serial port monitor that lets you view and log data from serial devices in real time.")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
set(CPACK_SOURCE_GENERATOR "TGZ;ZIP")
set(CPACK_SOURCE_IGNORE_FILES
    /.git 
    /.vscode
    /.*build.*
    /\\\\.DS_Store
)

set(CPACK_PACKAGE_CONTACT "royer01gomez@gmail.com")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Royer Gomez")
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libgtkmm-4.0-0 (>= 4.10)")
set(CPACK_DEBIAN_PACKAGE_HOMEPAGE "https://github.com/mandragora-hub/serial-port-monitor")
set(CPACK_DEBIAN_PACKAGE_LICENSE "MIT")
# set(CPACK_DEBIAN_FILE_NAME DEB-DEFAULT)
# set(CPACK_COMPONENTS_GROUPING ALL_COMPONENTS_IN_ONE)#ONE_PER_GROUP)
# set(CPACK_DEB_COMPONENT_INSTALL YES)

include(CPack)