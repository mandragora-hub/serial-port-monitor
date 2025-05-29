#ifndef SP_WORKER_H
#define SP_WORKER_H

#include <gtkmm-4.0/gtkmm.h>

#include "serialport.h"

class SPMAppWindow;

class SPWorker {
 public:
  SPWorker(SerialPort *port);
  ~SPWorker();

  void doWork(SPMAppWindow *caller);
  void stop_work();
  bool has_stopped() const;

 private:
  SerialPort *m_serialport = {nullptr};

  Glib::RefPtr<Gtk::TextBuffer> m_rx_buffer;
  Glib::RefPtr<Gtk::TextBuffer> m_tx_buffer;

  bool m_shall_stop;
  bool m_has_stopped;
};

#endif  // SP_WORKER_H
