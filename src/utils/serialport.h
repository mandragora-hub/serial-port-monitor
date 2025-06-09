#ifndef SERIAL_PORT_H
#define SERIAL_PORT_H

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

  static const std::vector<int> commons_bauds;

 private:
  struct sp_port* port = nullptr;
  unsigned int timeout = 1000;

  int check(enum sp_return result);
};

#endif  // SERIAL_PORT_H