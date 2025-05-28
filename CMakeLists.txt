cmake_minimum_required(VERSION 3.10.0)
project(arduino-serial-port-communication VERSION 0.1.0 LANGUAGES C CXX)

set (CMAKE_CXX_STANDARD 23)

# Installing GTKMM library
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTKMM REQUIRED gtkmm-4.0)

include_directories(${GTKMM_INCLUDE_DIRS})
link_directories(${GTKMM_LIBRARY_DIRS})
add_definitions(${GTKMM_CFLAGS_OTHER})

include(lib/external-dependencies.cmake)

# add_library(arduino-serial-port-communication-lib
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

add_executable(arduino-serial-port-communication src/main.cpp)
target_link_libraries(arduino-serial-port-communication resources_files sources_files ${GTKMM_LIBRARIES})

include(CTest)
enable_testing()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
