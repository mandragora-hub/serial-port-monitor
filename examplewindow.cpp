#include "examplewindow.h"
#include <iostream>

ExampleWindow::ExampleWindow() : m_Button("Hello World") {
  // Set the title of the window
  set_title("Basic application");
  // Set the default size of the window
  set_default_size(400, 600);

  // Add the button to the window
  set_child(m_Button);

  // Connect the button's clicked signal to the handler
  m_Button.signal_clicked().connect(
      sigc::mem_fun(*this, &ExampleWindow::on_button_clicked));
}

ExampleWindow::~ExampleWindow() {
  // Destructor
}
void ExampleWindow::on_button_clicked() {
  std::cout << "Button clicked!" << std::endl;
}