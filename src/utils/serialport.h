#ifndef SERIAL_PORT_H
#define SERIAL_PORT_H

#include <map>
#include <string>
#include <vector>

#include "libserialport.h"

class SerialPort {
 public:
  // SerialPort(std::string port_name, sp_mode mode = SP_MODE_READ);
  SerialPort(std::string port_name, sp_mode mode = SP_MODE_READ,
             int baud_rate = 9600, sp_parity parity = SP_PARITY_NONE,
             int bits = 8, int stopbits = 1,
             sp_flowcontrol flowcontrol = SP_FLOWCONTROL_NONE);
  ~SerialPort();

  static SerialPort* create(std::string port_name, sp_mode mode = SP_MODE_READ);
  static SerialPort* create(std::string port_name, sp_mode mode = SP_MODE_READ,
                            int baud_rate = 9600,
                            sp_parity parity = SP_PARITY_NONE, int bits = 8,
                            int stopbits = 1,
                            sp_flowcontrol flowcontrol = SP_FLOWCONTROL_NONE);
  static std::vector<std::string> list_available_serial_ports();

  // method
  // std::string getPortName();
  // std::string getDescription();

  int wait(unsigned int timeout, sp_event mask);
  int input_waiting();
  int read_data(void* buf, size_t count);
  int send_data(const void* data, size_t count);

  void set_parity(enum sp_parity parity);
  sp_parity get_parity();

  void set_bauds_rate(unsigned int bauds_rate);
  unsigned int get_bauds_rate();

  // Helper functions (TODO: every should public or private or both)
  void print_config();
  static const std::vector<int> commons_bauds;
  static std::map<enum sp_parity, std::string> parity_names;
  const char* parity_name(enum sp_parity parity);
  static enum sp_parity parity_from_name(std::string parity_name);
  int check(enum sp_return result);

 private:
  struct sp_port* port = nullptr;
  struct sp_port_config* config = nullptr;

  struct sp_port_config* create_default_settings();
  unsigned int timeout = 1000;
};

#endif  // SERIAL_PORT_H