#include <gtkmm-4.0/gtkmm.h>
#include "examplewindow.h"

int main(int argc, char* argv[]) {
  auto app = Gtk::Application::create("org.gtkmm.examples.base");

  return app->make_window_and_run<ExampleWindow>(argc, argv);
}