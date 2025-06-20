#include "sp_worker.h"

#include <chrono>
#include <iostream>
#include <thread>
#include "utils.h"

// TODO: This should be set in the settings
#define MILLISECOND_THREAD_SLEEP 400

SPWorker::SPWorker(SerialPort* port) : m_serialport(port) {
  // Unique id
  id = reinterpret_cast<uintptr_t>(port);

  m_rx_buffer = new DynamicBuffer();
  m_tx_buffer = new DynamicBuffer();
}

SPWorker::~SPWorker() {
  if (thread) {
    if (thread->joinable()) thread->join();
    delete thread;
    thread = nullptr;
  }
  delete m_serialport;
}

void SPWorker::do_work(SPMAppWindow* caller) {
  for (;;) {  // Run until break
    {
      std::lock_guard<std::mutex> lock(mutex);
      m_has_stopped = false;
    }

    // Send serial port data
    {
      if (m_tx_buffer->size() > 0) {
        std::lock_guard<std::mutex> lock(mutex);
        if (m_serialport->wait(100, SP_EVENT_TX_READY) >= 0) {
          m_serialport->send_data(m_tx_buffer->data(), m_tx_buffer->size());

          m_rx_buffer->append(m_tx_buffer->data(), m_tx_buffer->size());
          insert_entries(m_tx_buffer->data());
          m_tx_buffer->clear();
        }
      }
    }

    // Receive serial port data
    {
      std::lock_guard<std::mutex> lock(mutex);

      if (m_serialport->wait(100, SP_EVENT_RX_READY) >= 0) {
        // TODO:: get text from serial port, with can have a flag app for
        // activate
        // simulate mode maybe !!
        // std::string text = "simulate rx buffer entry.\n";

        int bytes_waiting = m_serialport->input_waiting();
        void* buf = malloc(bytes_waiting);
        if (!buf) continue;

        m_serialport->read_data(buf, bytes_waiting);
        std::string text(static_cast<char*>(buf), bytes_waiting);

        free(buf);
        m_rx_buffer->append(text.c_str(), text.size());
        insert_entries(text.c_str());
      }
    }

    if (m_shall_stop) {
      std::lock_guard<std::mutex> lock(mutex);
      m_has_stopped = true;
      break;
    }

    // Rest
    m_update_dispatcher.emit();
    std::this_thread::sleep_for(
        std::chrono::milliseconds(MILLISECOND_THREAD_SLEEP));
  }
}

void SPWorker::stop_work() {
  std::lock_guard<std::mutex> lock(mutex);
  m_shall_stop = true;
}

bool SPWorker::has_stopped() const {
  std::lock_guard<std::mutex> lock(mutex);
  return m_has_stopped;
}

void SPWorker::send_data(const char* data, size_t size) {
  std::lock_guard<std::mutex> lock(mutex);
  m_tx_buffer->append(data, size);
}

void SPWorker::clearRX() {
  std::lock_guard<std::mutex> lock(mutex);
  m_rx_buffer->clear();
}

std::vector<LogEntry> SPWorker::get_entries() {
  return log_entries;
}

void SPWorker::insert_entries(Glib::ustring text) {
  LogEntry entry;
  entry.timestamp = std::chrono::system_clock::now();
  entry.text = text;
  
  log_entries.push_back(entry);
}
