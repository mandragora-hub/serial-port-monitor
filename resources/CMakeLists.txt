# Generated resources files
set(GLIB_COMPILE_RESOURCES_COMMAND "glib-compile-resources")
set(resources_srcs "${CMAKE_CURRENT_BINARY_DIR}/resources.c")
add_custom_command(
      OUTPUT "${resources_srcs}"
      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
      COMMAND ${GLIB_COMPILE_RESOURCES_COMMAND}
      ARGS 
        --target="${resources_srcs}"
        --generate-source "${CMAKE_CURRENT_SOURCE_DIR}/gresource.xml"
        COMMENT "Compiling GResources"
)
add_library(resources_files STATIC ${resources_srcs})

# Generated schemas files
find_program(GLIB_COMPILE_SCHEMAS_COMMAND NAMES glib-compile-schemas REQUIRED)
set(APP_SCHEMA_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(COMPILED_SCHEMA_OUTPUT_DIR "${CMAKE_BINARY_DIR}/share/glib-2.0/schemas")
file(MAKE_DIRECTORY "${COMPILED_SCHEMA_OUTPUT_DIR}") # Create the output directory

add_custom_command(
      OUTPUT "${COMPILED_SCHEMA_OUTPUT_DIR}/gschemas.compiled"
      WORKING_DIRECTORY ${APP_SCHEMA_DIR}
      COMMAND ${GLIB_COMPILE_SCHEMAS_COMMAND} --targetdir="${COMPILED_SCHEMA_OUTPUT_DIR}" ${APP_SCHEMA_DIR} 
      DEPENDS "${APP_SCHEMA_DIR}/org.gtkmm.spmonitor.gschema.xml"
      # ARGS   
      COMMENT "Compiling GSchemas"
)
add_custom_target(
    compile_schemas 
    ALL
    DEPENDS "${COMPILED_SCHEMA_OUTPUT_DIR}/gschemas.compiled"
)
install(FILES "${COMPILED_SCHEMA_OUTPUT_DIR}/gschemas.compiled"
        DESTINATION "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_DATADIR}/glib-2.0/schemas"
)

configure_file(sp-monitor.desktop.in sp-monitor.desktop @ONLY)
configure_file(AppImageBuilder.yml.in AppImageBuilder.yml @ONLY)

# 
install(FILES ${CMAKE_BINARY_DIR}/resources/sp-monitor.desktop DESTINATION share/applications/)
install(FILES sp-monitor.png DESTINATION share/icons/hicolor/256x256/apps)

# Copy Appimage recipe
# file(REMOVE "${CMAKE_BINARY_DIR}/AppImageBuilder.yml")
# file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/AppImageBuilder.yml" DESTINATION "${CMAKE_BINARY_DIR}")