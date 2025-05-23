#ifndef SP_MONITOR_APP_H
#define SP_MONITOR_APP_H

#include <gtkmm-4.0/gtkmm.h>

class SPMonitorAppWindow;

class SPMonitorApp : public Gtk::Application {
 public:
  static Glib::RefPtr<SPMonitorApp> create();

 protected:
  SPMonitorApp();
  // override default signal handlers:
  void on_startup() override;
  void on_activate() override;
  void on_open(const Gio::Application::type_vec_files &files,
               const Glib::ustring &hint) override;

 private:
  SPMonitorAppWindow *create_appwindow();
  
  // Menu actions
  void on_action_preferences();
  void on_action_quit();
};

#endif  // SP_MONITOR_APP_H