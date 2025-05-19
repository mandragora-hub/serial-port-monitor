cmake_minimum_required(VERSION 3.10.0)
project(arduino-serial-port-communication VERSION 0.1.0 LANGUAGES C CXX)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTKMM REQUIRED gtkmm-4.0)

include_directories(${GTKMM_INCLUDE_DIRS})
link_directories(${GTKMM_LIBRARY_DIRS})
add_definitions(${GTKMM_CFLAGS_OTHER})

# add_library(arduino-serial-port-communication-lib
#     src/serial_port.cpp
#     src/serial_port.h
# )

add_library(sources_files STATIC
    arduinoserialportcommunicationapp.h
    arduinoserialportcommunicationapp.cpp
    arduinoserialportcommunicationwindow.h
    arduinoserialportcommunicationwindow.cpp
)


add_executable(arduino-serial-port-communication main.cpp)
target_link_libraries(arduino-serial-port-communication sources_files ${GTKMM_LIBRARIES})

include(CTest)
enable_testing()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
