cmake_minimum_required(VERSION 4.0.2)

set(PROJECT_VERSION_OVERRIDE "${PROJECT_VERSION}" CACHE STRING "Override project version")
set(PROJECT_VERSION "${PROJECT_VERSION_OVERRIDE}") # Apply the override

project(sp-monitor VERSION ${PROJECT_VERSION} LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

message(DEBUG "C++ compiler: ${CMAKE_CXX_COMPILER}")
message(DEBUG "C++ compiler version: ${CMAKE_CXX_COMPILER_VERSION}")


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

# Generated resources files
set(GLIB_COMPILE_RESOURCES_COMMAND "glib-compile-resources")
set(resources_srcs "${CMAKE_CURRENT_BINARY_DIR}/resources.c")
add_custom_command(
      OUTPUT "${resources_srcs}"
      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
      COMMAND ${GLIB_COMPILE_RESOURCES_COMMAND}
      ARGS 
        --target="${resources_srcs}"
        --generate-source "${CMAKE_CURRENT_SOURCE_DIR}/resources/gresource.xml"
      COMMENT "Compiling GResources"
)

# Generated schemas files
set(GLIB_COMPILE_SCHEMAS_COMMAND "glib-compile-schemas")
set(schemas_srcs "${CMAKE_CURRENT_BINARY_DIR}")
add_custom_command(
      OUTPUT "${schemas_srcs}/gschemas.compiled"
      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/resources
      COMMAND ${GLIB_COMPILE_SCHEMAS_COMMAND} . ## current working dir
      ARGS 
        --targetdir="${schemas_srcs}"
      COMMENT "Compiling GSchemas"
)
add_custom_target(
    setting_schemas 
    ALL
    DEPENDS "${schemas_srcs}/gschemas.compiled"
)

include_directories("${CMAKE_CURRENT_BINARY_DIR}")

add_library(resources_files STATIC ${resources_srcs})

# Use external dependencies => libserialport 
ExternalProject_Get_property(libserialport install_dir)
message(STATUS "Install directory of libserialport = ${install_dir}")
include_directories(${install_dir}/include)

# Add sources files
file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.h")
list(REMOVE_ITEM SOURCES "src/main.cpp")
add_library(sources_files STATIC ${SOURCES})

add_dependencies(sources_files libserialport)

target_link_libraries(sources_files PUBLIC ${install_dir}/lib/libserialport.a)

add_executable(sp-monitor src/main.cpp)
target_link_libraries(sp-monitor resources_files sources_files ${GTKMM_LIBRARIES})

include(CTest)
enable_testing()

# Packaging support
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
# set(CPACK_PACKAGE_VENDOR "Vendor name")
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
set(CPACK_DEB_COMPONENT_INSTALL YES)

include(CPack)
