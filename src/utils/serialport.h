#ifndef SERIAL_PORT_H
#define SERIAL_PORT_H

#include <string>

#include "libserialport.h"

class SerialPort {
 public:
  SerialPort(std::string port_name, sp_mode flags);
  ~SerialPort();
  // static void list_available_serial_ports();

  // method
  // std::string getPortName();
  // std::string getDescription();

 private:
  struct sp_port* port;

  int check(enum sp_return result);
};

#endif  // SERIAL_PORT_H