#include "spmonitorappwindow.h"
#include "spmonitorapp.h"

#include "resources.c"

SPMonitorAppWindow::SPMonitorAppWindow(
    BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder)
    : Gtk::ApplicationWindow(cobject), m_refBuilder(refBuilder) {}

// static
SPMonitorAppWindow *SPMonitorAppWindow::create() {
  auto refBuilder =
      Gtk::Builder::create_from_resource("/org/gtkmm/spmonitor/resources/window.ui");

  auto window = Gtk::Builder::get_widget_derived<SPMonitorAppWindow>(
      refBuilder, "app_window");
  if (!window)
    throw std::runtime_error("No \"app_window\" object in window.ui");

  return window;
}

void SPMonitorAppWindow::open_file_view(const Glib::RefPtr<Gio::File> &file) {
  // Open the file view
  // Implement the logic to open the file view here
}