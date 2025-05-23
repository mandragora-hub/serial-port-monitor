#ifndef ARDUINO_SERIAL_PORT_COMMUNICATION_WINDOW_H
#define ARDUINO_SERIAL_PORT_COMMUNICATION_WINDOW_H

#include <gtkmm-4.0/gtkmm.h>

class SPMonitorAppWindow : public Gtk::ApplicationWindow {
 public:
  SPMonitorAppWindow(BaseObjectType *cobject,
                     const Glib::RefPtr<Gtk::Builder> &refBuilder);

  static SPMonitorAppWindow *create();

  void open_file_view(const Glib::RefPtr<Gio::File> &file);

 protected:
  Glib::RefPtr<Gtk::Builder> m_refBuilder;
  Glib::RefPtr<Gio::Settings> m_settings;
  Gtk::Stack *m_stack {nullptr};
  Gtk::MenuButton *m_gears {nullptr};
};

#endif  // ARDUINO_SERIAL_PORT_COMMUNICATION_WINDOW_H