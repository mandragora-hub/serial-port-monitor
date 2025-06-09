#include "serialport.h"

#include <iostream>

SerialPort::SerialPort(std::string port_name, sp_mode flags) {
  check(sp_get_port_by_name(port_name.c_str(), &port));
  check(sp_open(port, flags));

  // TODO, manually setting in gui
  std::cout << "Setting port to 9600 8N1, no flow control." << std::endl;
  check(sp_set_baudrate(port, 9600));
  check(sp_set_bits(port, 8));
  check(sp_set_parity(port, SP_PARITY_NONE));
  check(sp_set_stopbits(port, 1));
  check(sp_set_flowcontrol(port, SP_FLOWCONTROL_NONE));
}

SerialPort::~SerialPort() {
  std::cout << "Serial port destructor called" << std::endl;
  check(sp_close(port));
  sp_free_port(port);
}

SerialPort *SerialPort::create(std::string port_name, sp_mode flags) {
  try {
    SerialPort *sp = new SerialPort(port_name, flags);
    return sp;
  } catch (const std::exception &e) {
    std::cerr << e.what() << '\n';
  }

  return nullptr;
}

int SerialPort::input_waiting() {
  int bytes_waiting = check(sp_input_waiting(port));
  return bytes_waiting;
}

int SerialPort::send_data(const void *data, size_t count) {
  return check(sp_blocking_write(port, data, count, timeout));
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
      throw std::runtime_error("Error: Invalid argument.\n");
    case SP_ERR_FAIL:
      error_message = sp_last_error_message();
      std::cerr << "Error: Failed: " << error_message << std::endl;
      sp_free_error_message(error_message);
      throw std::runtime_error("");
    case SP_ERR_SUPP:
      throw std::runtime_error("Error: Not supported.\n");
    case SP_ERR_MEM:
      throw std::runtime_error("Error: Couldn't allocate memory.\n");
    case SP_OK:
    default:
      return result;
  }
}

const std::vector<int> SerialPort::commons_bauds = {
    50,    75,    110,    134,    150,    200,    300,   600,
    1200,  1800,  2400,   4800,   9600,   19200,  28800, 38400,
    57600, 76800, 115200, 230400, 460800, 576000, 921600};

// void SerialPort::list_available_serial_ports() {
//   struct sp_port **port_list;

//   enum sp_return result = sp_list_ports(&port_list);

//   if (result != SP_OK) {
//     std::cerr << "sp_list_ports() failed!" << std::endl;
//     return;
//   }

//   /* Iterate through the ports. When port_list[i] is NULL
//    * this indicates the end of the list. */
//   int i;
//   for (i = 0; port_list[i] != NULL; i++) {
//     struct sp_port *port = port_list[i];

//     /* Get the name of the port. */
//     char *port_name = sp_get_port_name(port);

//     std::cout << "Found port: " << port_name << std::endl;
//   }

//   std::cout << "Found " << i << " ports." << std::endl;

//   /* Free the array created by sp_list_ports(). */
//   sp_free_port_list(port_list);
// }
