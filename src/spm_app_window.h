#ifndef SPM_APP_WINDOW_H
#define SPM_APP_WINDOW_H

#include <gtkmm-4.0/gtkmm.h>

#include <thread>
#include <unordered_map>

#include "utils/serialport.h"
#include "utils/sp_worker.h"

#define HAS_SEARCH_ENTRY2 GTKMM_CHECK_VERSION(4, 13, 2)

namespace {
// The only use of this class is to store refs of all component used in the stack view, Due to
// trought GtkStack is very hard to get all objects in the view.
class UIView : public Glib::Object {
 public:
  UIView() {}
  virtual ~UIView() {}
  Glib::RefPtr<Gtk::Builder> refBuilder = nullptr;
  Gtk::Box *view_box = nullptr;
  Gtk::TextView *view = nullptr;
  Gtk::Button *clear_output_button = nullptr;
  Gtk::Entry *input_entry = nullptr;
  Gtk::Button *input_send_button = nullptr;
  Gtk::CheckButton *autoscroll_check_button = nullptr;
  Gtk::CheckButton *show_timestamp_check_button = nullptr;
  Gtk::DropDown *parity_dropdown = nullptr;
  Gtk::DropDown *bauds_dropdown = nullptr;
};
std::vector<Glib::RefPtr<UIView>> stacks;
}  // namespace

class SPMAppWindow : public Gtk::ApplicationWindow {
 public:
  SPMAppWindow(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder);

  static SPMAppWindow *create();

  void open_file_view(const Glib::RefPtr<Gio::File> &file);
  void open_file_view(const Glib::ustring portname, sp_port_config *config, sp_mode mode);

 protected:
  // Signal handlers:
  void on_action_open_serial_port();
  void on_search_text_changed();
  void on_visible_child_changed();
  void on_find_word(const Gtk::Button *button);
  void on_reveal_child_changed();
  void on_worker_update(std::shared_ptr<SPWorker> worker, Gtk::TextView *textView,
                        Glib::RefPtr<Gio::Settings> port_settings);
  void refresh_text_view(Glib::ustring key, std::shared_ptr<SPWorker> worker,
                         Gtk::TextView *textView, Glib::RefPtr<Gio::Settings> port_settings);
  void on_clear_output(std::shared_ptr<SPWorker> worker, Gtk::TextView *textView);
  void on_activate_entry_and_clicked_send_button(Gtk::Entry *entry,
                                                 std::shared_ptr<SPWorker> worker);
  void on_text_view_changed(Gtk::TextView *textView, Glib::RefPtr<Gio::Settings> port_settings);
  void on_export();
  void on_close_current_tab();
  // UI style
  void on_font_changed(Glib::ustring key, Gtk::TextView *textView);
  // Port configuration
  void on_bauds_dropdown_changed(std::shared_ptr<SPWorker> worker, Gtk::DropDown *dropdown,
                                 Glib::RefPtr<Gio::Settings> port_settings);
  void on_parity_dropdown_changed(std::shared_ptr<SPWorker> worker, Gtk::DropDown *dropdown,
                                  Glib::RefPtr<Gio::Settings> port_settings);

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
  std::vector<std::shared_ptr<SPWorker>> sp_workers;

  std::string normalize_port_path(std::string port_path);
  Gtk::TextView *get_visible_text_view();
  Glib::RefPtr<Gtk::TextTag> create_default_style_tag(Gtk::TextView *textView);

  void initialize_ui_view(const std::shared_ptr<SPWorker> &worker,
                          const Glib::RefPtr<UIView> &ui_view,
                          const Glib::RefPtr<Gio::Settings> &port_settings);
};

#endif  // SPM_APP_WINDOW_H