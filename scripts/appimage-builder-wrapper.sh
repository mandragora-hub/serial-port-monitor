#!/bin/bash
# Just a wrapper for run appimage-builder docker version
# Ex:
#   Generate recipe
#   ./appimage-builder-wrapper.sh --generate
#   
#   Create appimage based on recipe
#   ./appimage-builder-wrapper.sh --skip-tests --recipe resources/AppImageBuilder.yml

appimage_builder="docker run --rm -it -v "$PWD":/work -w /work --user $(id -u):$(id -g) appimagecrafters/appimage-builder:latest appimage-builder"
${appimage_builder} $@

