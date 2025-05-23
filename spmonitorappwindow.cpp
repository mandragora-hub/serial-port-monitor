#include "spmonitorappwindow.h"

#include <iostream>
#include <stdexcept>

#include "resources.c"
#include "spmonitorapp.h"

SPMonitorAppWindow::SPMonitorAppWindow(
    BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder)
    : Gtk::ApplicationWindow(cobject), m_refBuilder(refBuilder) {
  // Get widgets from Gtk::Builder file
  m_stack = m_refBuilder->get_widget<Gtk::Stack>("stack");
  if (!m_stack) throw std::runtime_error("No \"stack\" object in window.ui");

  m_gears = m_refBuilder->get_widget<Gtk::MenuButton>("gears");
  if (!m_gears) throw std::runtime_error("No \"gears\" object in window.ui");

  // Bind settings
  m_settings = Gio::Settings::create("org.gtkmm.spmonitor");
  m_settings->bind("transition", m_stack->property_transition_type());

  // Connect the menu to MenuButton.
  auto menu_builder = Gtk::Builder::create_from_resource(
      "/org/gtkmm/spmonitor/resources/gears_menu.ui");
  auto menu = menu_builder->get_object<Gio::MenuModel>("menu");
  if (!menu) throw std::runtime_error("No \"menu\" object in gears_menu.ui");

  m_gears->set_menu_model(menu);
}

// static
SPMonitorAppWindow *SPMonitorAppWindow::create() {
  auto refBuilder = Gtk::Builder::create_from_resource(
      "/org/gtkmm/spmonitor/resources/window.ui");

  auto window = Gtk::Builder::get_widget_derived<SPMonitorAppWindow>(
      refBuilder, "app_window");
  if (!window)
    throw std::runtime_error("No \"app_window\" object in window.ui");

  return window;
}

void SPMonitorAppWindow::open_file_view(const Glib::RefPtr<Gio::File> &file) {
  const Glib::ustring basename = file->get_basename();

  auto scrolled = Gtk::make_managed<Gtk::ScrolledWindow>();
  scrolled->set_expand(true);
  auto view = Gtk::make_managed<Gtk::TextView>();
  view->set_editable(false);
  view->set_cursor_visible(false);
  scrolled->set_child(*view);
  m_stack->add(*scrolled, basename, basename);

  try {
    char *contents = nullptr;
    gsize length = 0;

    file->load_contents(contents, length);
    view->get_buffer()->set_text(contents, contents + length);
    g_free(contents);
  } catch (const Glib::Error &ex) {
    std::cout << "ExampleAppWindow::open_file_view(\"" << file->get_parse_name()
              << "\"):\n  " << ex.what() << std::endl;
  }
}