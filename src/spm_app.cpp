#include "spm_app.h"

#include <iostream>

#include "spm_app_prefs_dialog.h"
#include "spm_app_window.h"

SPMApp::SPMApp()
    : Gtk::Application("org.gtkmm.spmonitor",
                       Gio::Application::Flags::HANDLES_OPEN) {
  // Constructor implementation
}

Glib::RefPtr<SPMApp> SPMApp::create() {
  return Glib::make_refptr_for_instance<SPMApp>(new SPMApp());
}

SPMAppWindow *SPMApp::create_appwindow() {
  // Create and return a new instance of SPMAppWindow

  auto appwindow = SPMAppWindow::create();

  add_window(*appwindow);

  // Connect the signal to delete the appwindow when it is closed
  appwindow->signal_hide().connect([appwindow]() { delete appwindow; });
  return appwindow;
}

void SPMApp::on_startup() {
  Gtk::Application::on_startup();

  add_action("preferences",
             sigc::mem_fun(*this, &SPMApp::on_action_preferences));
  add_action("quit", sigc::mem_fun(*this, &SPMApp::on_action_quit));
  set_accel_for_action("app.quit", "<Ctrl>Q");
}

void SPMApp::on_activate() {
  try {
    // Create a new app window when the application is activated
    auto appwindow = create_appwindow();
    appwindow->present();
  } catch (const Glib::Error &ex) {
    std::cerr << "SPM::on_activate(): " << ex.what() << std::endl;

  } catch (const std::exception &ex) {
    std::cerr << "SPM::on_activate(): " << ex.what() << std::endl;
  }
}

void SPMApp::on_open(const Gio::Application::type_vec_files &files,
                           const Glib::ustring &hint) {
  // The application has been asked to open some files,
  // so let's open a new view for each one.
  SPMAppWindow *appwindow = nullptr;
  auto windows = get_windows();
  if (windows.size() > 0)
    appwindow = dynamic_cast<SPMAppWindow *>(windows[0]);

  try {
    if (!appwindow) appwindow = create_appwindow();

    for (const auto &file : files) appwindow->open_file_view(file);

    appwindow->present();
  } catch (const Glib::Error &ex) {
    std::cerr << "SPM::on_open(): " << ex.what() << std::endl;
  } catch (const std::exception &ex) {
    std::cerr << "SPM::on_open(): " << ex.what() << std::endl;
  }
}

void SPMApp::on_action_preferences() {
  try {
    auto prefs_dialog = SPMAppPrefsDialog::create(*get_active_window());
    prefs_dialog->present();

    prefs_dialog->signal_hide().connect(
        [prefs_dialog]() { delete prefs_dialog; });
  } catch (const Glib::Error &ex) {
    std::cerr << "SPM::on_action_preferences(): " << ex.what()
              << std::endl;
  } catch (const std::exception &ex) {
    std::cerr << "SPM::on_action_preference(): " << ex.what()
              << std::endl;
  }
}

void SPMApp::on_action_quit() {
  auto windows = get_windows();
  for (auto window : windows) window->set_visible(false);

  quit();
}
