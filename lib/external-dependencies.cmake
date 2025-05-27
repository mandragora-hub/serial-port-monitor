# find_program(MAKE_EXE NAMES make)

SET(EXTERNAL_PROJECT_PREFIX "${CMAKE_BINARY_DIR}/external_projects/libserialport")

include(ExternalProject)
ExternalProject_Add(libserialport
  GIT_REPOSITORY    git@github.com:mandragora-hub/libserialport.git
  GIT_TAG           origin/master
  PREFIX "${EXTERNAL_PROJECT_PREFIX}"
  CONFIGURE_COMMAND <SOURCE_DIR>/autogen.sh
                    COMMAND <SOURCE_DIR>/configure --prefix=<INSTALL_DIR>
)

