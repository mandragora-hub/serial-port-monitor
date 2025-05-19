#ifndef ARDUINO_SERIAL_PORT_COMMUNICATION_WINDOW_H
#define ARDUINO_SERIAL_PORT_COMMUNICATION_WINDOW_H

#include <gtkmm-4.0/gtkmm.h>

class ArduinoSerialPortCommunicationWindow : public Gtk::ApplicationWindow {
 public:
  ArduinoSerialPortCommunicationWindow();
  void open_file_view(const Glib::RefPtr<Gio::File> &file);
};

#endif  // ARDUINO_SERIAL_PORT_COMMUNICATION_WINDOW_H