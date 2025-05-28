#ifndef SPM_OPEN_SERIAL_PORT_DIALOG_H
#define SPM_OPEN_SERIAL_PORT_DIALOG_H

#include <gtkmm-4.0/gtkmm.h>


class SPMOpenSerialPortDialog : public Gtk::Window {
 public:
  SPMOpenSerialPortDialog(BaseObjectType *cobject,
                    const Glib::RefPtr<Gtk::Builder> &refBuilder);
  static SPMOpenSerialPortDialog *create(Gtk::Window &parent);

 protected:
  // Signal handlers
  // void on_font_setting_changed(const Glib::ustring &key);
  // void on_font_selection_changed();
  // void on_transition_setting_changed(const Glib::ustring &key);
  // void on_transition_selection_changed();

  Glib::RefPtr<Gtk::Builder> m_refBuilder;
  Glib::RefPtr<Gio::Settings> m_settings;
  Gtk::FontDialogButton *m_font{nullptr};
  Gtk::DropDown *m_transition{nullptr};
};
#endif // SPM_OPEN_SERIAL_PORT_DIALOG_H