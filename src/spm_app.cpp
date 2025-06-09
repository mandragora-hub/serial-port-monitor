#include "spm_app.h"

#include <iostream>

#include "spm_about_dialog.h"
#include "spm_app_prefs_dialog.h"
#include "spm_app_window.h"
#include "utils/serialport.h"

SPMApp::SPMApp()
    : Gtk::Application("org.gtkmm.spmonitor",
                       Gio::Application::Flags::HANDLES_OPEN) {
  // Constructor implementation
  add_main_option_entry(Gio::Application::OptionType::BOOL, "list", 'l',
                        "Shows the list of availables serial port");
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
  add_action("show-about", sigc::mem_fun(*this, &SPMApp::on_action_about));
  add_action("quit", sigc::mem_fun(*this, &SPMApp::on_action_quit));
  set_accel_for_action("app.quit", "<Ctrl>Q");
  set_accel_for_action("win.close-tab", "<Ctrl>W");

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
  if (windows.size() > 0) appwindow = dynamic_cast<SPMAppWindow *>(windows[0]);

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

template <typename T_ArgType>
static bool get_arg_value(const Glib::RefPtr<Glib::VariantDict> &options,
                          const Glib::ustring &arg_name, T_ArgType &arg_value) {
  arg_value = T_ArgType();
  if (options->lookup_value(arg_name, arg_value)) {
    return true;
  } else {
    auto gvariant =
        g_variant_dict_lookup_value(options->gobj(), arg_name.c_str(), nullptr);
    if (!gvariant) {
      // std::cerr << "the \"" << arg_name
      //           << "\" value was not in the options VariantDict." <<
      //           std::endl;
    } else {
      std::cerr << "the \"" << arg_name << "\" value was of type "
                << g_variant_get_type_string(gvariant) << " instead of "
                << Glib::Variant<T_ArgType>::variant_type().get_string()
                << std::endl;
    }
    return false;
  }
}

int SPMApp::on_command_line(
    const Glib::RefPtr<Gio::ApplicationCommandLine> &command_line) {
  const auto options = command_line->get_options_dict();
  if (!options) std::cerr << G_STRFUNC << ": options is null." << std::endl;

  bool list_serialport_value = false;
  command_line_mode = get_arg_value(options, "list", list_serialport_value);

  if (list_serialport_value) {
    // SerialPort::list_available_serial_ports();
    return EXIT_SUCCESS;
  }

  if (!command_line_mode) {
    activate();
    return EXIT_SUCCESS;
  }

  return EXIT_SUCCESS;
}

void SPMApp::on_action_preferences() {
  try {
    auto prefs_dialog = SPMAppPrefsDialog::create(*get_active_window());
    prefs_dialog->present();

    prefs_dialog->signal_hide().connect(
        [prefs_dialog]() { delete prefs_dialog; });
  } catch (const Glib::Error &ex) {
    std::cerr << "SPM::on_action_preferences(): " << ex.what() << std::endl;
  } catch (const std::exception &ex) {
    std::cerr << "SPM::on_action_preference(): " << ex.what() << std::endl;
  }
}

void SPMApp::on_action_about() {
  try {
    auto about_dialog = SPMAboutDialog::create(*get_active_window());
    about_dialog->present();

    about_dialog->signal_hide().connect(
        [about_dialog]() { delete about_dialog; });
  } catch (const Glib::Error &ex) {
    std::cerr << "SPM::on_action_about(): " << ex.what() << std::endl;
  } catch (const std::exception &ex) {
    std::cerr << "SPM::on_action_about(): " << ex.what() << std::endl;
  }
}

void SPMApp::on_action_quit() {
  auto windows = get_windows();
  for (auto window : windows) window->set_visible(false);

  quit();
}
