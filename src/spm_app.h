#ifndef SPM_APP_H
#define SPM_APP_H

#include <gtkmm-4.0/gtkmm.h>

class SPMAppWindow;

class SPMApp : public Gtk::Application {
 public:
  static Glib::RefPtr<SPMApp> create();

 protected:
  SPMApp();
  // override default signal handlers:
  void on_startup() override;
  void on_activate() override;
  void on_open(const Gio::Application::type_vec_files &files,
               const Glib::ustring &hint) override;
  int on_command_line(
      const Glib::RefPtr<Gio::ApplicationCommandLine> &command_line) override;
  // int on_handle_local_options(
  // const Glib::RefPtr<Glib::VariantDict> &options) override;

 private:
  SPMAppWindow *create_appwindow();

  // Menu actions
  void on_action_preferences();
  void on_action_quit();

  // If this is true the gui will not show
  bool command_line_mode = false;
};

#endif  // SMR_APP_H