#ifndef ARDUINO_SERIAL_PORT_COMMUNICATION_WINDOW_H
#define ARDUINO_SERIAL_PORT_COMMUNICATION_WINDOW_H

#include <gtkmm-4.0/gtkmm.h>

#define HAS_SEARCH_ENTRY2 GTKMM_CHECK_VERSION(4, 13, 2)

class SPMonitorAppWindow : public Gtk::ApplicationWindow {
 public:
  SPMonitorAppWindow(BaseObjectType *cobject,
                     const Glib::RefPtr<Gtk::Builder> &refBuilder);

  static SPMonitorAppWindow *create();

  void open_file_view(const Glib::RefPtr<Gio::File> &file);

 protected:
  // signal handlers:
  void on_search_text_changed();
  void on_visible_child_changed();
  void on_find_word(const Gtk::Button *button);
  void on_reveal_child_changed();

  void update_words();
  void update_lines();

  Glib::RefPtr<Gtk::Builder> m_refBuilder;
  Glib::RefPtr<Gio::Settings> m_settings;
  Gtk::Stack *m_stack{nullptr};
  Gtk::MenuButton *m_gears{nullptr};
  Gtk::ToggleButton *m_search{nullptr};
  Gtk::SearchBar *m_searchbar{nullptr};
#if HAS_SEARCH_ENTRY2
  Gtk::SearchEntry2 *m_searchentry{nullptr};
#else
  Gtk::SearchEntry *m_searchentry{nullptr};
#endif
  Gtk::Revealer *m_sidebar{nullptr};
  Gtk::ListBox *m_words{nullptr};
  Gtk::Label *m_lines{nullptr};
  Gtk::Label *m_lines_label{nullptr};
  Glib::RefPtr<Glib::Binding> m_binding_search_enabled;
  Glib::RefPtr<Glib::Binding> m_binding_lines_visible;
};

#endif  // ARDUINO_SERIAL_PORT_COMMUNICATION_WINDOW_H