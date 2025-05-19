#include "spmonitorapp.h"

#include <iostream>

#include "spmonitorappwindow.h"

SPMonitorApp::SPMonitorApp()
    : Gtk::Application("org.gtkmm.spmonitor",
                       Gio::Application::Flags::HANDLES_OPEN) {
  // Constructor implementation
}

Glib::RefPtr<SPMonitorApp> SPMonitorApp::create() {
  return Glib::make_refptr_for_instance<SPMonitorApp>(new SPMonitorApp());
}

SPMonitorAppWindow *SPMonitorApp::create_appwindow() {
  // Create and return a new instance of SPMonitorAppWindow

  auto appwindow = SPMonitorAppWindow::create();

  add_window(*appwindow);

  // Connect the signal to delete the appwindow when it is closed
  appwindow->signal_hide().connect([appwindow]() { delete appwindow; });
  return appwindow;
}

void SPMonitorApp::on_activate() {
  try {
    // Create a new app window when the application is activated
    auto appwindow = create_appwindow();
    appwindow->present();
  } catch (const Glib::Error &ex) {
    std::cerr << "SPMonitor::on_activate(): " << ex.what() << std::endl;

  } catch (const std::exception &ex) {
    std::cerr << "SPMonitor::on_activate(): " << ex.what() << std::endl;
  }
}

void SPMonitorApp::on_open(const Gio::Application::type_vec_files &files,
                           const Glib::ustring &hint) {
  // Handle file opening
}
