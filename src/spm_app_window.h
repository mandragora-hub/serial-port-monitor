#ifndef SPM_APP_WINDOW_H
#define SPM_APP_WINDOW_H

#include <gtkmm-4.0/gtkmm.h>

#include <thread>
#include <unordered_map>

#include "utils/serialport.h"
#include "utils/sp_worker.h"

#define HAS_SEARCH_ENTRY2 GTKMM_CHECK_VERSION(4, 13, 2)

class SPMAppWindow : public Gtk::ApplicationWindow {
 public:
  SPMAppWindow(BaseObjectType *cobject,
               const Glib::RefPtr<Gtk::Builder> &refBuilder);

  static SPMAppWindow *create();

  void open_file_view(const Glib::RefPtr<Gio::File> &file);

 protected:
  // Signal handlers:
  void on_search_text_changed();
  void on_visible_child_changed();
  void on_find_word(const Gtk::Button *button);
  void on_reveal_child_changed();
  void on_bauds_dropdown_changed(std::shared_ptr<SPWorker> worker,
                                 Gtk::DropDown *dropdown,
                                 Glib::RefPtr<Gio::Settings> port_settings);
  void on_parity_dropdown_changed(std::shared_ptr<SPWorker> worker,
                                  Gtk::DropDown *dropdown,
                                  Glib::RefPtr<Gio::Settings> port_settings);
  void on_clear_output(Gtk::TextView *textView);
  void on_activate_entry_and_clicked_send_button(
      Gtk::Entry *entry, std::shared_ptr<SPWorker> worker);
  void on_worker_update(std::shared_ptr<SPWorker> worker,
                        Gtk::TextView *textView);
  void on_close_current_tab();
  void on_text_view_changed(Gtk::TextView *textView,
                            Glib::RefPtr<Gio::Settings> port_settings);
  void on_export();
  void on_action_open_serial_port();

  // actions
  // TODO: maybe all actions should be move it to private scope
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

 private:
  Gtk::TextView *get_text_view();
  void createView();

  // SerialPort *serialPort{nullptr};
  // SPWorker m_Worker;

  // std::unordered_map<std::string, SPWorker> m_WorkerTable;
  std::vector<std::shared_ptr<SPWorker>> sp_workers;

  Glib::Dispatcher m_Dispatcher;

  std::string normalize_port_path(std::string port_path);
  Gtk::TextView *get_visible_text_view();
};

#endif  // SPM_APP_WINDOW_H