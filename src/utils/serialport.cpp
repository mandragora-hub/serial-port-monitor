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
  check(sp_close(port));
  sp_free_port(port);
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
