# find_program(MAKE_EXE NAMES make)

SET(EXTERNAL_PROJECT_PREFIX "${CMAKE_BINARY_DIR}/external_projects/libserialport")

include(ExternalProject)
ExternalProject_Add(libserialport
  GIT_REPOSITORY    https://github.com/mandragora-hub/libserialport.git
  GIT_TAG           origin/master
  PREFIX "${EXTERNAL_PROJECT_PREFIX}"
  # INSTALL_DIR "libserialport"
  # INSTALL_COMMAND "make install DESTDIR=./lsp"
  CONFIGURE_COMMAND <SOURCE_DIR>/autogen.sh
                    COMMAND <SOURCE_DIR>/configure --prefix=<INSTALL_DIR>
  UPDATE_DISCONNECTED 1      # Don’t re-fetch every time
  # BUILD_BYPRODUCTS ${LIBSERIALPORT_PREFIX}/lib/libserialport.a
)

