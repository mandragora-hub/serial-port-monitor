#include "spmonitorapp.h"

int main(int argc, char* argv[]) {
#if HAS_SEARCH_ENTRY2
  // A GtkSearchEntry (e.g from window.ui) shall  be wrapped in
  // a GtkSearchEntry2 and not in a deprecated Gtk::SearchEntry.
  Gtk::Application::wrap_in_search_entry2(true);
#endif
  // Since this example is running uninstalled, we have to help it find
  // its schema. This is *not* necessary in a properly installed
  // application.
  Glib::setenv("GSETTINGS_SCHEMA_DIR", ".", false);

  auto app = SPMonitorApp::create();
  // Initialize the application
  return app->run(argc, argv);
}