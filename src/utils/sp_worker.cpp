#include "sp_worker.h"
#include <chrono>
#include <thread>


SPWorker::SPWorker(SerialPort* port) : m_serialport(port) {
  m_rx_buffer = Gtk::TextBuffer::create();
  m_tx_buffer = Gtk::TextBuffer::create();
}

SPWorker::~SPWorker() {}

void SPWorker::doWork() {
  Gtk::TextBuffer::iterator iter = m_rx_buffer->end();
  m_rx_buffer->insert(iter, "simulating sp entry.");

  std::this_thread::sleep_for(std::chrono::milliseconds(300));
}

void SPWorker::stop_work() {}

bool SPWorker::has_stopped() const { return false; }
