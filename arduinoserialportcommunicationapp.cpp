#include "arduinoserialportcommunicationapp.h"
#include "arduinoserialportcommunicationwindow.h"

ArduinoSerialPortCommunicationApp::ArduinoSerialPortCommunicationApp()
    : Gtk::Application("org.gtkmm.arduinoserialportcommunicationapp",
                       Gio::Application::Flags::HANDLES_OPEN) {
  // Constructor implementation
}

Glib::RefPtr<ArduinoSerialPortCommunicationApp>
ArduinoSerialPortCommunicationApp::create() {
  return Glib::make_refptr_for_instance<ArduinoSerialPortCommunicationApp>(
      new ArduinoSerialPortCommunicationApp());
}

ArduinoSerialPortCommunicationWindow *
ArduinoSerialPortCommunicationApp::create_appwindow() {
  // Create and return a new instance of ArduinoSerialPortCommunicationWindow

  auto appwindow = new ArduinoSerialPortCommunicationWindow();

  add_window(*appwindow);

  // Connect the signal to delete the appwindow when it is closed
  appwindow->signal_hide().connect([appwindow]() { delete appwindow; });
  return appwindow;
}

void ArduinoSerialPortCommunicationApp::on_activate() {
  // Create a new app window when the application is activated
  auto appwindow = create_appwindow();
  appwindow->present();
}

void ArduinoSerialPortCommunicationApp::on_open(
    const Gio::Application::type_vec_files &files, const Glib::ustring &hint) {
  // Handle file opening
}
