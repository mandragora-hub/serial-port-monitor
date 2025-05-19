#ifndef ARDUINO_APP
#define ARDUINO_APP

#include <gtkmm-4.0/gtkmm.h>

class ArduinoSerialPortCommunicationWindow;

class ArduinoSerialPortCommunicationApp : public Gtk::Application {
 public:
  static Glib::RefPtr<ArduinoSerialPortCommunicationApp> create();

 protected:
  ArduinoSerialPortCommunicationApp();
  // override default signal handlers:
  void on_activate() override;
  void on_open(const Gio::Application::type_vec_files &files,
               const Glib::ustring &hint) override;

 private:
  ArduinoSerialPortCommunicationWindow *create_appwindow();
};

#endif  // ARDUINO_APP