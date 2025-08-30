#ifndef SPM_OPEN_SERIAL_PORT_DIALOG_H
#define SPM_OPEN_SERIAL_PORT_DIALOG_H

#include <gtkmm-4.0/gtkmm.h>
#include "utils/serialport.h"

class SPMOpenSerialPortDialog : public Gtk::Window {
 public:
  SPMOpenSerialPortDialog(BaseObjectType *cobject,
                          const Glib::RefPtr<Gtk::Builder> &refBuilder);
  static SPMOpenSerialPortDialog *create(Gtk::Window &parent);

  Glib::Dispatcher connect_dispatcher;

  std::string get_portname();
  enum sp_mode get_serial_port_mode();
  struct sp_port_config *get_serial_port_config();

 protected:
  // Signal handlers
  void on_refresh_serial_port_list();

  Glib::RefPtr<Gtk::Builder> m_refBuilder;
  Gtk::DropDown *m_port{nullptr};
  Gtk::DropDown *m_baudrate{nullptr};
  Gtk::DropDown *m_parity{nullptr};
  Gtk::DropDown *m_databits{nullptr};
  Gtk::DropDown *m_stopbits{nullptr};
  Gtk::DropDown *m_flowcontrol{nullptr};
  Gtk::CheckButton *m_read_checkbutton{nullptr};
  Gtk::CheckButton *m_write_checkbutton{nullptr};
  Gtk::CheckButton *m_read_write_checkbutton{nullptr};
};
#endif  // SPM_OPEN_SERIAL_PORT_DIALOG_H