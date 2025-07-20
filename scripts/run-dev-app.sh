#!/bin/bash
# Wrapper to run the app in dev mode. Copied in the build stage and should be available in the build folder
# This is needed cause we need to set the settings schemes dir. 
#

export XDG_DATA_DIRS="${PWD}/share:${XDG_DATA_DIRS}"
./sp-monitor # Adjust this path to your executable if needed

# To test gsettings directly with your schema:
#gsettings --schemadir="${PWD}/share/glib-2.0/schemas" list-keys com.example.my-gtk-app
#gsettings --schemadir="${PWD}/share/glib-2.0/schemas" get com.example.my-gtk-app show-toolbar
#gsettings --schemadir="${PWD}/share/glib-2.0/schemas" set com.example.my-gtk-app show-toolbar false
