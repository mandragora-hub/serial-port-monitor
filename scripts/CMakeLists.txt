# Copy run-dev-app.sh to build folder
file(REMOVE "${CMAKE_BINARY_DIR}/run-dev-app.sh")
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/run-dev-app.sh" DESTINATION "${CMAKE_BINARY_DIR}")

# Copy appimage-builder-wrapper.sh to build folder
file(REMOVE "${CMAKE_BINARY_DIR}/appimage-builder-wrapper.sh")
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/appimage-builder-wrapper.sh" DESTINATION "${CMAKE_BINARY_DIR}")

