#ifndef SERIAL_PORT_H
#define SERIAL_PORT_H

#include <map>
#include <string>
#include <vector>

#include "libserialport.h"

class SerialPort {
 public:
  SerialPort(std::string port_name, sp_mode flags);
  ~SerialPort();

  static SerialPort* create(std::string port_name, sp_mode flags);
  // static void list_available_serial_ports();

  // method
  // std::string getPortName();
  // std::string getDescription();

  int wait(unsigned int timeout, sp_event mask);
  int input_waiting();
  int read_data(void* buf, size_t count);
  int send_data(const void* data, size_t count);

  // Helper functions (TODO: every should public or private or both)
  void show_config();
  static const std::vector<int> commons_bauds;
  static std::map<enum sp_parity, std::string> parity_names;
  const char* parity_name(enum sp_parity parity);
  int check(enum sp_return result);

 private:
  struct sp_port* port = nullptr;
  struct sp_port_config* config = nullptr;

  struct sp_port_config* create_default_settings();
  unsigned int timeout = 1000;
};

#endif  // SERIAL_PORT_H