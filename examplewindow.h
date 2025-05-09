#ifndef EXAMPLEWINDOW_H
#define EXAMPLEWINDOW_H

#include <gtkmm-4.0/gtkmm/window.h>
#include <gtkmm-4.0/gtkmm/button.h>

class ExampleWindow : public Gtk::Window {
 public:
  ExampleWindow();
  virtual ~ExampleWindow();

 protected:
  // Signal handlers:
  void on_button_clicked();

  // Member widgets:
  Gtk::Button m_Button;
};

#endif  // EXAMPLEWINDOW_H