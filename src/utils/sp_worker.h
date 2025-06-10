#ifndef SP_WORKER_H
#define SP_WORKER_H

#include <gtkmm-4.0/gtkmm.h>

#include <thread>

#include "dynamic_buffer.h"
#include "serialport.h"

class SPMAppWindow;

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

  bool get_shall_stop() const { return m_shall_stop; }

  bool operator==(const SPWorker &other) const { return id == other.id; }

  uintptr_t getID() const { return id; }

  // TODO: saving thread references here. is really the best places for it?
  std::thread *thread = {nullptr};

  Glib::Dispatcher m_dispatcher;

  const DynamicBuffer *get_rx_buffer() const { return m_rx_buffer; }

 private:
  // Synchronizes access to member data.
  mutable std::mutex mutex;

  Glib::ustring name;
  uintptr_t id;

  SerialPort *m_serialport = {nullptr};

  DynamicBuffer *m_rx_buffer = {nullptr};
  DynamicBuffer *m_tx_buffer = {nullptr};

  bool m_shall_stop = false;
  bool m_has_stopped = false;
};

// provide a hash function for use with std::map
namespace std {
template <>
struct hash<SPWorker> {
  std::size_t operator()(const SPWorker &w) const noexcept {
    std::size_t h1 = std::hash<int>{}(w.getID());
    return h1;
  }
};
}  // namespace std

#endif  // SP_WORKER_H
