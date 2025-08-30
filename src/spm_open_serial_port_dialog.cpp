#include "spm_open_serial_port_dialog.h"

#include <array>

#include "spm_app_window.h"
#include "utils/serialport.h"
#include "utils/utils.h"

// TODO: move this to serial port class
const std::array<int, 4> commons_databits = {8, 7, 6, 5};
const std::array<int, 2> commons_stopbits = {1, 2};

SPMOpenSerialPortDialog::SPMOpenSerialPortDialog(
    BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refBuilder)
    : Gtk::Window(cobject), m_refBuilder(refBuilder) {
  m_port = m_refBuilder->get_widget<Gtk::DropDown>("port");
  if (!m_port)
    throw std::runtime_error("No \"port\" object in open_serial_port.ui");
  on_refresh_serial_port_list();

  m_baudrate = m_refBuilder->get_widget<Gtk::DropDown>("baudrate");
  if (!m_baudrate)
    throw std::runtime_error("No \"baudrate\" object in open_serial_port.ui");

  auto baudrate_list = Gtk::StringList::create();
  for (const auto& rate : SerialPort::commons_bauds)
    baudrate_list->append(Glib::ustring(std::to_string(rate)));

  m_baudrate->set_model(baudrate_list);
  m_baudrate->set_selected(12);

  m_parity = m_refBuilder->get_widget<Gtk::DropDown>("parity");
  if (!m_parity)
    throw std::runtime_error("No \"parity\" object in open_serial_port.ui");

  auto parity_list = Gtk::StringList::create();
  for (auto it : SerialPort::parity_names) {
    if (it.first == SP_PARITY_INVALID) continue;
    parity_list->append(it.second);
  }
  m_parity->set_model(parity_list);

  m_databits = m_refBuilder->get_widget<Gtk::DropDown>("databits");
  if (!m_databits)
    throw std::runtime_error("No \"databits\" object in open_serial_port.ui");
  auto databits_list = Gtk::StringList::create();
  for (const auto& bits : commons_databits)
    databits_list->append(Glib::ustring::compose("%1", bits));
  m_databits->set_model(databits_list);

  m_stopbits = m_refBuilder->get_widget<Gtk::DropDown>("stopbits");
  if (!m_stopbits)
    throw std::runtime_error("No \"stopbits\" object in open_serial_port.ui");
  auto stopbits_list = Gtk::StringList::create();
  for (const auto& bits : commons_stopbits)
    stopbits_list->append(Glib::ustring::compose("%1", bits));
  m_stopbits->set_model(stopbits_list);

  m_flowcontrol = m_refBuilder->get_widget<Gtk::DropDown>("flowcontrol");
  if (!m_flowcontrol)
    throw std::runtime_error(
        "No \"flowcontrol\" object in open_serial_port.ui");
  auto flowcontrol_list = Gtk::StringList::create();
  for (auto it : SerialPort::flowcontrol_names)
    flowcontrol_list->append(it.second);
  m_flowcontrol->set_model(flowcontrol_list);

  m_read_checkbutton = m_refBuilder->get_widget<Gtk::CheckButton>("mode_read");
  if (!m_read_checkbutton)
    throw std::runtime_error("No \"mode_read\" object in open_serial_port.ui");

  m_write_checkbutton =
      m_refBuilder->get_widget<Gtk::CheckButton>("mode_write");
  if (!m_write_checkbutton)
    throw std::runtime_error("No \"mode_write\" object in open_serial_port.ui");

  m_read_write_checkbutton =
      m_refBuilder->get_widget<Gtk::CheckButton>("mode_readwrite");
  if (!m_read_write_checkbutton)
    throw std::runtime_error(
        "No \"mode_readwrite\" object in open_serial_port.ui");

  auto m_connect = m_refBuilder->get_widget<Gtk::Button>("connect");
  if (!m_connect)
    throw std::runtime_error("No \"connect\" object in open_serial_port.ui");

  m_connect->signal_clicked().connect([&]() { connect_dispatcher.emit(); });

  // m_connect->signal_clicked().connect(
  //     sigc::mem_fun(*this, &SPMOpenSerialPortDialog::on_connect));

  auto m_refresh_port_list =
      m_refBuilder->get_widget<Gtk::Button>("refresh_port_list");
  if (!m_refresh_port_list)
    throw std::runtime_error(
        "No \"refresh_port_list\" object in open_serial_port.ui");

  m_refresh_port_list->signal_clicked().connect(sigc::mem_fun(
      *this, &SPMOpenSerialPortDialog::on_refresh_serial_port_list));

  auto m_cancel = m_refBuilder->get_widget<Gtk::Button>("cancel");
  if (!m_cancel)
    throw std::runtime_error("No \"close\" object in open_serial_port.ui");

  m_cancel->signal_clicked().connect(
      sigc::mem_fun(*this, &SPMOpenSerialPortDialog::close));
}

SPMOpenSerialPortDialog* SPMOpenSerialPortDialog::create(Gtk::Window& parent) {
  // Load the builder file and instantiate its widgets.
  auto refBuilder = Gtk::Builder::create_from_resource(
      "/org/gtkmm/spmonitor/ui/open_serial_port.ui");

  auto dialog = Gtk::Builder::get_widget_derived<SPMOpenSerialPortDialog>(
      refBuilder, "open_serial_port_dialog");
  if (!dialog)
    throw std::runtime_error(
        "No \"open_serial_port_dialog\" object in prefs.ui");

  dialog->set_transient_for(parent);
  return dialog;
}

std::string SPMOpenSerialPortDialog::get_portname() {
  return Utils::get_string_from_object_base(m_port->get_selected_item());
}

sp_mode SPMOpenSerialPortDialog::get_serial_port_mode() {
  sp_mode mode = SP_MODE_READ;
  if (m_read_checkbutton->get_active()) mode = SP_MODE_READ;
  if (m_write_checkbutton->get_active()) mode = SP_MODE_WRITE;
  if (m_read_write_checkbutton->get_active()) mode = SP_MODE_READ_WRITE;
  return mode;
}

sp_port_config* SPMOpenSerialPortDialog::get_serial_port_config() {
  int baudrate =
      Utils::get_int_from_object_base(m_baudrate->get_selected_item());
  std::string parity_name =
      Utils::get_string_from_object_base(m_parity->get_selected_item());
  int databits =
      Utils::get_int_from_object_base(m_databits->get_selected_item());
  int stopbits =
      Utils::get_int_from_object_base(m_stopbits->get_selected_item());

  sp_port_config* config = SerialPort::create_config(
      baudrate, SerialPort::parity_from_name(parity_name), databits, stopbits);
  // config.flowcontrol = m_flowcontrol;
  return config;
}

void SPMOpenSerialPortDialog::on_refresh_serial_port_list() {
  auto ports_list = Gtk::StringList::create();
  for (const auto& portname : SerialPort::list_available_serial_ports())
    ports_list->append(portname);
  m_port->set_model(ports_list);
}
