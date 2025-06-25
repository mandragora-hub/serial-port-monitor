#include "spm_app.h"

int main(int argc, char* argv[]) {
  #if HAS_SEARCH_ENTRY2
  // https://gnome.pages.gitlab.gnome.org/gtkmm/classGtk_1_1SearchEntry2.html#details
  Gtk::Application::wrap_in_search_entry2(true);
  #endif

  // Since this example is running uninstalled, we have to help it find
  // its schema. This is *not* necessary in a properly installed
  // application.
  Glib::setenv("GSETTINGS_SCHEMA_DIR", ".", false);

  auto app = SPMApp::create();
  return app->run(argc, argv);
}