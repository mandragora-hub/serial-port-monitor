#ifndef SP_WORKER_H
#define SP_WORKER_H

#include <gtkmm-4.0/gtkmm.h>

#include <map>
#include <thread>

#include "dynamic_buffer.h"
#include "serialport.h"

class SPMAppWindow;

// struct used for track text and timestamp
struct LogEntry {
  std::chrono::system_clock::time_point timestamp;
  Glib::ustring text;
};

class SPWorker {
 public:
  SPWorker(SerialPort *port);
  ~SPWorker();

  void do_work(SPMAppWindow *caller);
  void stop_work();
  bool has_stopped() const;

  void send_data(const char *data, size_t size);
  void clearRX();

  void set_name(Glib::ustring new_name) { name = new_name; }
  const Glib::ustring get_name() const { return name; }

  SerialPort *get_port() const { return m_serialport; }

  bool get_shall_stop() const { return m_shall_stop; }

  bool operator==(const SPWorker &other) const { return name == other.name; }

  // TODO: saving thread references here. is really the best places for it?
  std::thread *thread = {nullptr};

  Glib::Dispatcher m_update_dispatcher;

  const DynamicBuffer *get_rx_buffer() const { return m_rx_buffer; }

  const std::vector<LogEntry> get_entries();
  void clear_entries();
  void insert_entries(Glib::ustring text,
                      std::chrono::system_clock::time_point timestamp =
                          std::chrono::system_clock::now());

 private:
  // Synchronizes access to member data.
  mutable std::mutex mutex;
  Glib::ustring name;

  SerialPort *m_serialport = {nullptr};

  DynamicBuffer *m_rx_buffer = {nullptr};
  DynamicBuffer *m_tx_buffer = {nullptr};

  std::vector<LogEntry> log_entries;

  bool m_shall_stop = false;
  bool m_has_stopped = false;
};
// namespace std

#endif  // SP_WORKER_H
