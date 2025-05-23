#include "spmonitorapp.h"

int main(int argc, char* argv[]) {
  // Since this example is running uninstalled, we have to help it find its
  // schema. This is *not* necessary in a properly installed application.
  Glib::setenv("GSETTINGS_SCHEMA_DIR", ".", false);

  auto app = SPMonitorApp::create();
  // Initialize the application
  return app->run(argc, argv);
}