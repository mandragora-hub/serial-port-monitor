#include "sp_worker.h"

#include <chrono>
#include <iostream>
#include <thread>

// TODO: This should be set in the settings
#define MILLISECOND_THREAD_SLEEP 1500

SPWorker::SPWorker(SerialPort* port) : m_serialport(port) {
  // Unique id
  id = reinterpret_cast<uintptr_t>(port);

  m_rx_buffer = new DynamicBuffer(0);
  m_tx_buffer = new DynamicBuffer(0);
}

SPWorker::SPWorker() {
  m_rx_buffer = new DynamicBuffer(0);
  m_tx_buffer = new DynamicBuffer(0);
}

SPWorker::~SPWorker() {}

void SPWorker::do_work(SPMAppWindow* caller) {
  for (;;) {
    {
      std::lock_guard<std::mutex> lock(mutex);

      // TODO:: get text from serial port, with can have a flag app for activate
      // simalete mode maybe !!
      std::string text = "simulate rx buffer entry.\n";

      m_rx_buffer->append(text.c_str(), text.size());
    }

    m_dispatcher.emit();
    std::this_thread::sleep_for(
        std::chrono::milliseconds(MILLISECOND_THREAD_SLEEP));
  }
}

void SPWorker::stop_work() {}

bool SPWorker::has_stopped() const { return false; }

void SPWorker::clearRX() {
  std::lock_guard<std::mutex> lock(mutex);
  m_rx_buffer->clear();
}
