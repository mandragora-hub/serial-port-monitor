#ifndef SERIAL_PORT_H
#define SERIAL_PORT_H

#include <array>
#include <map>
#include <string>
#include <vector>

#include "libserialport.h"

class SerialPort {
 public:
  SerialPort(std::string port_name, sp_mode mode = SP_MODE_READ);
  SerialPort(std::string port_name, sp_port_config* config,
             sp_mode mode = SP_MODE_READ);
  SerialPort(std::string port_name, sp_mode mode = SP_MODE_READ,
             int baud_rate = 9600, sp_parity parity = SP_PARITY_NONE,
             int bits = 8, int stopbits = 1,
             sp_flowcontrol flowcontrol = SP_FLOWCONTROL_NONE);
  ~SerialPort();

  static SerialPort* create(std::string port_name, sp_mode mode = SP_MODE_READ);
  static SerialPort* create(std::string port_name, sp_port_config* config,
                            sp_mode mode);
  static SerialPort* create(std::string port_name, sp_mode mode = SP_MODE_READ,
                            int baud_rate = 9600,
                            sp_parity parity = SP_PARITY_NONE, int bits = 8,
                            int stopbits = 1,
                            sp_flowcontrol flowcontrol = SP_FLOWCONTROL_NONE);
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

  sp_mode get_mode();

  // Helper functions (TODO: every should public or private or both)
  void print_config();
  static int check(enum sp_return result);
  static std::vector<std::string> list_available_serial_ports();

  static const std::array<int, 23> commons_bauds;

  static std::map<enum sp_parity, std::string> parity_names;
  const char* parity_name(enum sp_parity parity);
  static enum sp_parity parity_from_name(const std::string parity_name);

  static std::map<enum sp_flowcontrol, std::string> flowcontrol_names;
  const char* flowcontrol_name(enum sp_flowcontrol flowcontrol);
  static enum sp_flowcontrol flowcontrol_from_name(
      std::string flowcontrol_name);

  static std::map<enum sp_mode, std::string> mode_names;
  static const char* mode_name(sp_mode mode);

  static struct sp_port_config* create_config(
      int baud_rate = 9600, sp_parity parity = SP_PARITY_NONE, int bits = 8,
      int stopbits = 1, sp_flowcontrol flowcontrol = SP_FLOWCONTROL_NONE);
  //   --------------

 private:
  struct sp_port* port = nullptr;
  struct sp_port_config* config = nullptr;
  sp_mode mode;

  static struct sp_port_config* create_default_settings();
  unsigned int timeout = 1000;
};

#endif  // SERIAL_PORT_H