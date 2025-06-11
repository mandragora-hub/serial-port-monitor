#include "serialport.h"

#include <string.h>

#include <format>
#include <iostream>

// SerialPort::SerialPort(std::string port_name, sp_mode mode) {
//   check(sp_get_port_by_name(port_name.c_str(), &port));
//   check(sp_open(port, mode));

//   // Default settings
//   std::cout << "Setting port " << port_name << " to 9600 8N1, no flow control."
//             << std::endl;
//   config = create_default_settings();

//   std::cout << "Applying new configuration" << std::endl;
//   check(sp_set_config(port, config));
// }

SerialPort::SerialPort(std::string port_name, sp_mode mode, int baud_rate,
                       sp_parity parity, int bits, int stopbits,
                       sp_flowcontrol flowcontrol) {
  check(sp_get_port_by_name(port_name.c_str(), &port));
  check(sp_open(port, mode));

  config = create_default_settings();
  check(sp_set_config_baudrate(config, baud_rate));
  check(sp_set_config_bits(config, bits));
  check(sp_set_config_parity(config, parity));
  check(sp_set_config_stopbits(config, stopbits));
  check(sp_set_config_flowcontrol(config, flowcontrol));
  check(sp_set_config(port, config));
}

// SerialPort::SerialPort(std::string port_name, sp_mode flags) {
//   check(sp_get_port_by_name(port_name.c_str(), &port));
//   check(sp_open(port, flags));

//     /* Allocate config. */
//   check(sp_new_config(&config));

//   int baudrate, bits, stopbits;
//   enum sp_parity parity;

//   struct sp_port_config *other_config;
//   check(sp_new_config(&other_config));
//   check(sp_set_config_baudrate(other_config, 9600));
//   check(sp_set_config_bits(other_config, 7));
//   check(sp_set_config_parity(other_config, SP_PARITY_EVEN));
//   check(sp_set_config_stopbits(other_config, 2));
//   check(sp_set_config_flowcontrol(other_config, SP_FLOWCONTROL_XONXOFF));

//   // TODO, manually setting in gui
//   std::cout << "Setting port to 9600 8N1, no flow control." << std::endl;
//   check(sp_set_baudrate(port, 9600));
//   check(sp_set_bits(port, 8));
//   check(sp_set_parity(port, SP_PARITY_NONE));
//   check(sp_set_stopbits(port, 1));
//   check(sp_set_flowcontrol(port, SP_FLOWCONTROL_NONE));
// }

SerialPort::~SerialPort() {
  check(sp_close(port));
  sp_free_port(port);
  sp_free_config(config);
}

// SerialPort *SerialPort::create(std::string port_name, sp_mode mode) {
//   try {
//     SerialPort *sp = new SerialPort(port_name, mode);
//     return sp;
//   } catch (const std::exception &e) {
//     std::cerr << e.what() << '\n';
//   }

//   return nullptr;
// }

SerialPort *SerialPort::create(std::string port_name, sp_mode mode,
                               int baud_rate, sp_parity parity, int bits,
                               int stopbits, sp_flowcontrol flowcontrol) {
  try {
    SerialPort *sp = new SerialPort(port_name, mode, baud_rate, parity, bits,
                                    stopbits, flowcontrol);
    return sp;
  } catch (const std::exception &e) {
    std::cerr << e.what() << '\n';
  }

  return nullptr;
}

std::vector<std::string> SerialPort::list_available_serial_ports() {
  struct sp_port **port_list;
  enum sp_return result = sp_list_ports(&port_list);
  if (result != SP_OK) {
    std::cerr << "sp_list_ports() failed!" << std::endl;
    return std::vector<std::string>();
  }

  /* Iterate through the ports. When port_list[i] is NULL
   * this indicates the end of the list. */
  std::vector<std::string> list;
  for (int i = 0; port_list[i] != NULL; i++) {
    struct sp_port *port = port_list[i];
    char *port_name = sp_get_port_name(port);
    list.push_back(port_name);
  }

  /* Free the array created by sp_list_ports(). */
  sp_free_port_list(port_list);
  return list;
}

int SerialPort::input_waiting() {
  int bytes_waiting = check(sp_input_waiting(port));
  return bytes_waiting;
}

int SerialPort::send_data(const void *data, size_t count) {
  return check(sp_blocking_write(port, data, count, timeout));
}

sp_parity SerialPort::get_parity() {
  enum sp_parity parity;
  check(sp_get_config_parity(config, &parity));
  return parity;
}

void SerialPort::set_parity(sp_parity parity) {
  struct sp_port_config *other_config;
  check(sp_new_config(&other_config));

  check(sp_get_config(port, other_config));
  check(sp_set_config_parity(other_config, parity));
  check(sp_set_config(port, other_config));

  sp_free_config(config);
  config = other_config;
}

unsigned int SerialPort::get_bauds_rate() {
  int bauds_rate;
  check(sp_get_config_baudrate(config, &bauds_rate));
  return bauds_rate;
}

void SerialPort::set_bauds_rate(unsigned int bauds_rate) {
  struct sp_port_config *other_config;
  check(sp_new_config(&other_config));

  check(sp_get_config(port, other_config));
  check(sp_set_config_baudrate(other_config, bauds_rate));
  check(sp_set_config(port, other_config));

  sp_free_config(config);
  config = other_config;
}

void SerialPort::print_config() {
  std::string port_name = sp_get_port_name(port);

  int baudrate, bits, stopbits;
  enum sp_parity parity;

  check(sp_get_config_baudrate(config, &baudrate));
  check(sp_get_config_bits(config, &bits));
  check(sp_get_config_stopbits(config, &stopbits));
  check(sp_get_config_parity(config, &parity));

  std::cout << std::format(
                   "Port name: {}, baudrate: {}, data bits: {}, parity: {}, "
                   "stop bits: {}",
                   port_name, baudrate, bits, parity_name(parity), stopbits)
            << std::endl;
}

sp_port_config *SerialPort::create_default_settings() {
  struct sp_port_config *initial_config;
  check(sp_new_config(&initial_config));

  check(sp_set_config_baudrate(initial_config, 9600));
  check(sp_set_config_bits(initial_config, 8));
  check(sp_set_config_parity(initial_config, SP_PARITY_NONE));
  check(sp_set_config_stopbits(initial_config, 1));
  check(sp_set_config_flowcontrol(initial_config, SP_FLOWCONTROL_NONE));

  return initial_config;
}

int SerialPort::wait(unsigned int timeout, sp_event mask) {
  struct sp_event_set *event_set;
  sp_new_event_set(&event_set);
  sp_add_port_events(event_set, port, mask);
  int result = check(sp_wait(event_set, timeout));

  // I have no idea of how works this function, i dont understand de docs, I
  // know it will wait for a ready state for sp_event, but alway return 0, i
  // mean always is ready
  std::cout << "debug: wait will return " << result << " for mask " << mask
            << std::endl;

  /* free resources. */
  sp_free_event_set(event_set);
  return result;
}

int SerialPort::read_data(void *buf, size_t count) {
  int result = check(sp_blocking_read(port, buf, count, 1000));
  return result;
}

int SerialPort::check(sp_return result) {
  char *error_message;

  switch (result) {
    case SP_ERR_ARG:
      throw std::runtime_error("Error: Invalid argument.");
    case SP_ERR_FAIL:
      error_message = sp_last_error_message();
      std::cerr << "Error: Failed: " << error_message << std::endl;
      sp_free_error_message(error_message);
      throw std::runtime_error("");
    case SP_ERR_SUPP:
      throw std::runtime_error("Error: Not supported.");
    case SP_ERR_MEM:
      throw std::runtime_error("Error: Couldn't allocate memory.");
    case SP_OK:
    default:
      return result;
  }
}

const std::vector<int> SerialPort::commons_bauds = {
    50,    75,    110,    134,    150,    200,    300,   600,
    1200,  1800,  2400,   4800,   9600,   19200,  28800, 38400,
    57600, 76800, 115200, 230400, 460800, 576000, 921600};

std::map<enum sp_parity, std::string> SerialPort::parity_names = {
    {SP_PARITY_INVALID, "(Invalid)"}, {SP_PARITY_NONE, "None"},
    {SP_PARITY_ODD, "Odd"},           {SP_PARITY_EVEN, "Even"},
    {SP_PARITY_MARK, "Mark"},         {SP_PARITY_SPACE, "Space"}};

/* Helper function to give a name for each parity mode. */
const char *SerialPort::parity_name(enum sp_parity parity) {
  switch (parity) {
    case SP_PARITY_INVALID:
      return "(Invalid)";
    case SP_PARITY_NONE:
      return "None";
    case SP_PARITY_ODD:
      return "Odd";
    case SP_PARITY_EVEN:
      return "Even";
    case SP_PARITY_MARK:
      return "Mark";
    case SP_PARITY_SPACE:
      return "Space";
    default:
      return NULL;
  }
}

sp_parity SerialPort::parity_from_name(std::string parity_name) {
  for (auto it : SerialPort::parity_names) {
    if (strcmp(it.second.c_str(), parity_name.c_str()) == 0) return it.first;
  }

  return SP_PARITY_INVALID;
}
