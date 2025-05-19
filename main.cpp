#include "spmonitorapp.h"

int main(int argc, char* argv[]) {
  auto app = SPMonitorApp::create();
  // Initialize the application
  return app->run(argc, argv);
}