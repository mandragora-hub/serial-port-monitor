#include <gtkmm-4.0/gtkmm.h>
#include "arduinoserialportcommunicationapp.h"

int main(int argc, char* argv[]) {
  auto app = ArduinoSerialPortCommunicationApp::create();
  // Initialize the application
  return app->run(argc, argv);
}