#include "spm_app_window.h"

#include <format>
#include <iostream>
#include <set>
#include <stdexcept>

#include "resources.c"
#include "spm_app.h"
#include "spm_open_serial_port_dialog.h"
#include "utils/utils.h"

SPMAppWindow::SPMAppWindow(BaseObjectType *cobject,
                           const Glib::RefPtr<Gtk::Builder> &refBuilder)
    : Gtk::ApplicationWindow(cobject), m_refBuilder(refBuilder) {
  // Get widgets from Gtk::Builder file
  m_stack = m_refBuilder->get_widget<Gtk::Stack>("stack");
  if (!m_stack) throw std::runtime_error("No \"stack\" object in window.ui");

  m_search = m_refBuilder->get_widget<Gtk::ToggleButton>("search");
  if (!m_search) throw std::runtime_error("No \"search\" object in window.ui");

  m_searchbar = m_refBuilder->get_widget<Gtk::SearchBar>("searchbar");
  if (!m_searchbar)
    throw std::runtime_error("No \"searchbar\" object in window.ui");

#if HAS_SEARCH_ENTRY2
  m_searchentry = m_refBuilder->get_widget<Gtk::SearchEntry2>("searchentry");
#else
  m_searchentry = m_refBuilder->get_widget<Gtk::SearchEntry>("searchentry");
#endif
  if (!m_searchentry)
    throw std::runtime_error("No \"searchentry\" object in window.ui");

  m_gears = m_refBuilder->get_widget<Gtk::MenuButton>("gears");
  if (!m_gears) throw std::runtime_error("No \"gears\" object in window.ui");

  m_sidebar = m_refBuilder->get_widget<Gtk::Revealer>("sidebar");
  if (!m_sidebar)
    throw std::runtime_error("No \"sidebar\" object in window.ui");

  m_words = m_refBuilder->get_widget<Gtk::ListBox>("words");
  if (!m_words) throw std::runtime_error("No \"words\" object in window.ui");

  m_lines = m_refBuilder->get_widget<Gtk::Label>("lines");
  if (!m_lines) throw std::runtime_error("No \"lines\" object in window.ui");

  m_lines_label = m_refBuilder->get_widget<Gtk::Label>("lines_label");
  if (!m_lines_label)
    throw std::runtime_error("No \"lines_label\" object in window.ui");

  // Bind settings
  m_settings = Gio::Settings::create("org.gtkmm.spmonitor");
  m_settings->bind("transition", m_stack->property_transition_type());
  m_settings->bind("show-words", m_sidebar->property_reveal_child());

  m_binding_lines_visible = Glib::Binding::bind_property(
      m_lines->property_visible(), m_lines_label->property_visible());
  m_settings->bind("show-lines", m_lines->property_visible());

  // Bind properties
  m_binding_search_enabled = Glib::Binding::bind_property(
      m_search->property_active(), m_searchbar->property_search_mode_enabled(),
      Glib::Binding::Flags::BIDIRECTIONAL);

  // Connect signal handler
  m_searchentry->signal_search_changed().connect(
      sigc::mem_fun(*this, &SPMAppWindow::on_search_text_changed));
  m_stack->property_visible_child().signal_changed().connect(
      sigc::mem_fun(*this, &SPMAppWindow::on_visible_child_changed));
  m_sidebar->property_reveal_child().signal_changed().connect(
      sigc::mem_fun(*this, &SPMAppWindow::on_reveal_child_changed));

  // Connect the menu to MenuButton.
  auto menu_builder = Gtk::Builder::create_from_resource(
      "/org/gtkmm/spmonitor/resources/gears_menu.ui");
  auto menu = menu_builder->get_object<Gio::MenuModel>("menu");
  if (!menu) throw std::runtime_error("No \"menu\" object in gears_menu.ui");

  m_gears->set_menu_model(menu);
  add_action("open-serial-port",
             sigc::mem_fun(*this, &SPMAppWindow::on_action_open_serial_port));
  add_action("export", sigc::mem_fun(*this, &SPMAppWindow::on_export));
  add_action("close-tab",
             sigc::mem_fun(*this, &SPMAppWindow::on_close_current_tab));
  add_action(m_settings->create_action("show-words"));
  add_action(m_settings->create_action("show-lines"));

  // Bind the "visible" property of m_lines to the win.show-lines action, to
  // the "Lines" menu item and to the "visible" property of m_lines_label.
  // add_action(
  // Gio::PropertyAction::create("show-lines", m_lines->property_visible()));

  // Set window icon
  Gtk::IconTheme::get_for_display(get_display())
      ->add_resource_path("/org/gtkmm/spmonitor/resources");
  set_icon_name("logo");

  // Create welcome message
  //   try {
  //     auto welcome_message = Gtk::make_managed<Gtk::Label>();
  //     auto welcome_box = Gtk::make_managed<Gtk::CenterBox>();

  //     Glib::RefPtr<Gio::File> msg_file = Gio::File::create_for_uri(
  //         "resource:///org/gtkmm/spmonitor/resources/welcome_msg.xml");
  //     char *contents = nullptr;
  //     gsize length = 0;

  //     msg_file->load_contents(contents, length);

  //     welcome_message->set_markup(Glib::ustring(contents));
  //     // welcome_message->set_wrap(true);
  //     welcome_message->set_justify(Gtk::Justification::CENTER);

  //     welcome_box->set_center_widget(*welcome_message);
  //     welcome_box->set_expand();

  //     m_stack->add(*welcome_box, "Welcome", "Welcome");

  //     g_free(contents);

  //   } catch (const Glib::Error &ex) {
  //     std::cerr << ex.what() << std::endl;
  //   }

  //   // Remove this line and create a action.
  //   // createView();
}

// static
SPMAppWindow *SPMAppWindow::create() {
  auto refBuilder = Gtk::Builder::create_from_resource(
      "/org/gtkmm/spmonitor/resources/window.ui");

  auto window =
      Gtk::Builder::get_widget_derived<SPMAppWindow>(refBuilder, "app_window");
  if (!window)
    throw std::runtime_error("No \"app_window\" object in window.ui");

  return window;
}

void SPMAppWindow::open_file_view(const Glib::RefPtr<Gio::File> &file) {
  const Glib::ustring basename = file->get_basename();

  auto refBuilder = Gtk::Builder::create_from_resource(
      "/org/gtkmm/spmonitor/resources/view.ui");
  auto view_box = refBuilder->get_widget<Gtk::Box>("view-box");
  if (!view_box)
    throw std::runtime_error("No \"lines_label\" object in view.ui");

  auto view = refBuilder->get_widget<Gtk::TextView>("text-view");
  if (!view) throw std::runtime_error("No \"lines_label\" object in view.ui");

  auto clearOutputButton =
      refBuilder->get_widget<Gtk::Button>("clear-output-button");
  if (!clearOutputButton)
    throw std::runtime_error("No \"clear-output-button\" object in view.ui");

  auto input_entry = refBuilder->get_widget<Gtk::Entry>("input-entry");
  if (!input_entry)
    throw std::runtime_error("No \"input-entry\" object in view.ui");

  auto input_send_button =
      refBuilder->get_widget<Gtk::Button>("input-send-button");
  if (!input_send_button)
    throw std::runtime_error("No \"input-send-button\" object in view.ui");

  auto autoscroll_check_button =
      refBuilder->get_widget<Gtk::CheckButton>("autoscroll-check-button");
  if (!autoscroll_check_button)
    throw std::runtime_error(
        "No \"autoscroll-check-button\" object in view.ui");

  auto show_timestamp_check_button =
      refBuilder->get_widget<Gtk::CheckButton>("show-timestamp-check-button");
  if (!show_timestamp_check_button)
    throw std::runtime_error(
        "No \"show-timestamp-check-button\" object in view.ui");

  auto parity_dropdown =
      refBuilder->get_widget<Gtk::DropDown>("parity-dropdown");
  if (!parity_dropdown)
    throw std::runtime_error("No \"parity-dropdown\" object in view.ui");

  auto parity_list = Gtk::StringList::create();
  for (auto it : SerialPort::parity_names) {
    if (it.first == SP_PARITY_INVALID) continue;
    parity_list->append(it.second);
  }
  parity_dropdown->set_model(parity_list);

  auto bauds_dropdown = refBuilder->get_widget<Gtk::DropDown>("bauds-dropdown");
  if (!bauds_dropdown)
    throw std::runtime_error("No \"bauds-dropdown\" object in view.ui");

  auto bauds = Gtk::StringList::create();
  for (int rate : SerialPort::commons_bauds) {
    bauds->append(Glib::ustring::compose("%1 bauds", rate));
  }
  bauds_dropdown->set_model(bauds);

  // Set GUI element and serial port with ports settings
  std::string port_setting_path = std::format(
      "/org/gtkmm/spmonitor/ports/{}/", normalize_port_path(file->get_path()));
  Glib::RefPtr<Gio::Settings> port_settings =
      Gio::Settings::create("org.gtkmm.spmonitor.ports", port_setting_path);

  port_settings->bind("autoscroll", autoscroll_check_button->property_active());
  port_settings->bind("show-timestamp",
                      show_timestamp_check_button->property_active());

  int bauds_settings = port_settings->get_uint("bauds");
  int in = Utils::indexOf<int>(SerialPort::commons_bauds, bauds_settings);
  bauds_dropdown->set_selected(in);

  Glib::ustring parity_setting = port_settings->get_string("parity");
  int index_of_parity =
      Utils::find_index_in_string_list(parity_list, parity_setting);
  parity_dropdown->set_selected(index_of_parity);
  //  --------------------------

  // Try to create the serial ports
  SerialPort *sp =
      SerialPort::create(file->get_path(), SP_MODE_READ_WRITE, bauds_settings,
                         SerialPort::parity_from_name(parity_setting));
  if (!sp)
    throw std::runtime_error("Error connecting with port: " + file->get_path());

  m_stack->add(*view_box, basename, basename);

  auto worker = std::make_shared<SPWorker>(sp);
  worker->set_name(basename);
  worker->m_update_dispatcher.connect(
      sigc::bind(sigc::mem_fun(*this, &SPMAppWindow::on_worker_update), worker,
                 view, port_settings));
  worker->thread = new std::thread([this, worker] { worker->do_work(this); });

  sp_workers.push_back(worker);

  // Connect gui signal with slots
  Glib::Binding::bind_property(
      input_entry->property_text(), input_send_button->property_sensitive(),
      Glib::Binding::Flags::SYNC_CREATE, [](const Glib::ustring &text) -> bool {
        return !Utils::trim(text).empty();
      });
  Glib::SlotSpawnChildSetup send_entry_to_worker_slot = sigc::bind(
      sigc::mem_fun(*this,
                    &SPMAppWindow::on_activate_entry_and_clicked_send_button),
      input_entry, worker);
  input_entry->signal_activate().connect(send_entry_to_worker_slot);
  input_send_button->signal_clicked().connect(send_entry_to_worker_slot);
  clearOutputButton->signal_clicked().connect(sigc::bind(
      sigc::mem_fun(*this, &SPMAppWindow::on_clear_output), worker, view));
  bauds_dropdown->property_selected().signal_changed().connect(
      sigc::bind(sigc::mem_fun(*this, &SPMAppWindow::on_bauds_dropdown_changed),
                 worker, bauds_dropdown, port_settings));
  parity_dropdown->property_selected().signal_changed().connect(sigc::bind(
      sigc::mem_fun(*this, &SPMAppWindow::on_parity_dropdown_changed), worker,
      parity_dropdown, port_settings));
  view->get_buffer()->signal_changed().connect(
      sigc::bind(sigc::mem_fun(*this, &SPMAppWindow::on_text_view_changed),
                 view, port_settings));

  port_settings->signal_changed("show-timestamp")
      .connect(
          sigc::bind(sigc::mem_fun(*this, &SPMAppWindow::refresh_text_view),
                     worker, view, port_settings));
  m_settings->signal_changed("font").connect(
      sigc::bind(sigc::mem_fun(*this, &SPMAppWindow::on_font_changed), view));

  // m_search->set_sensitive(true);

  // update_words();
  update_lines();
}

void SPMAppWindow::on_search_text_changed() {
  const auto text = m_searchentry->get_text();
  auto view = get_visible_text_view();
  if (!view) {
    std::cout << "SPMAppWindow::on_search_text_changed(): No visible text view."
              << std::endl;
    return;
  }

  // Very simple-minded search implementation
  auto buffer = view->get_buffer();
  Gtk::TextIter match_start;
  Gtk::TextIter match_end;
  if (buffer->begin().forward_search(text,
                                     Gtk::TextSearchFlags::CASE_INSENSITIVE,
                                     match_start, match_end)) {
    buffer->select_range(match_start, match_end);
    view->scroll_to(match_start);
  }
}

void SPMAppWindow::on_visible_child_changed() {
  m_searchbar->set_search_mode(false);
  update_words();
  update_lines();
}

void SPMAppWindow::on_find_word(const Gtk::Button *button) {
  m_searchentry->set_text(button->get_label());
}

void SPMAppWindow::on_reveal_child_changed() { update_words(); }

void SPMAppWindow::on_bauds_dropdown_changed(
    std::shared_ptr<SPWorker> worker, Gtk::DropDown *dropdown,
    Glib::RefPtr<Gio::Settings> port_settings) {
  const int bauds_rate = SerialPort::commons_bauds.at(dropdown->get_selected());
  port_settings->set_uint("bauds", bauds_rate);
  SerialPort *port = worker->get_port();
  if (port) port->set_bauds_rate(bauds_rate);
}

void SPMAppWindow::on_parity_dropdown_changed(
    std::shared_ptr<SPWorker> worker, Gtk::DropDown *dropdown,
    Glib::RefPtr<Gio::Settings> port_settings) {
  Glib::RefPtr<Glib::ObjectBase> selected_item = dropdown->get_selected_item();

  if (!selected_item) {
    std::cerr << "Error: on_parity_dropdown_changed" << std::endl;
    return;
  }
  auto string_object =
      std::dynamic_pointer_cast<Gtk::StringObject>(selected_item);
  if (!string_object) {
    std::cerr << "Error: Selected item could not be dynamically cast to "
                 "Gtk::StringObject."
              << std::endl;
    return;
  }
  Glib::ustring value = string_object->get_string();
  port_settings->set_string("parity", value);

  SerialPort *port = worker->get_port();
  if (port) port->set_parity(SerialPort::parity_from_name(value));
}

void SPMAppWindow::on_clear_output(std::shared_ptr<SPWorker> worker,
                                   Gtk::TextView *textView) {
  textView->get_buffer()->set_text("");  // TODO; this is the correct clear way?
  worker->clear_entries();
}

void SPMAppWindow::on_activate_entry_and_clicked_send_button(
    Gtk::Entry *entry, std::shared_ptr<SPWorker> worker) {
  Glib::ustring msg = Utils::trim(entry->get_text());
  if (msg.empty()) return;
  msg.append("\n");
  worker->send_data(msg.c_str(), msg.size());
  entry->set_text("");
}

void SPMAppWindow::on_worker_update(std::shared_ptr<SPWorker> worker,
                                    Gtk::TextView *textView,
                                    Glib::RefPtr<Gio::Settings> port_settings) {
  if (worker->get_shall_stop()) return;  // Is this really needed?
  auto buffer = textView->get_buffer();
  auto tag = create_default_style_tag(textView);

  Glib::ustring entry = Glib::ustring(worker->get_rx_buffer()->data(),
                                      worker->get_rx_buffer()->size());
  entry = Utils::trim(entry);
  if (entry.empty()) return;

  bool showtimestamp = port_settings->get_boolean("show-timestamp");

  auto regex = Glib::Regex::create("\\n");
  std::vector<Glib::ustring> lines = regex->split(entry);
  for (const auto &line : lines) {
    auto timestamp = std::chrono::system_clock::now();
    worker->insert_entries(line, timestamp);

    std::ostringstream oss;
    if (showtimestamp) {
      oss << Utils::get_timestamp_string(timestamp) << ": ";
    }
    oss << line << std::endl;

    Gtk::TextBuffer::iterator iter = buffer->end();
    buffer->insert_with_tag(iter, oss.str(), tag);
  }
  worker->clearRX();
}

void SPMAppWindow::refresh_text_view(
    Glib::ustring key, std::shared_ptr<SPWorker> worker,
    Gtk::TextView *textView, Glib::RefPtr<Gio::Settings> port_settings) {
  bool showtimestamp = port_settings->get_boolean("show-timestamp");
  auto tag = create_default_style_tag(textView);
  auto buffer = textView->get_buffer();
  buffer->set_text("");

  auto entries = worker->get_entries();
  for (const auto &entry : entries) {
    std::ostringstream oss;
    if (showtimestamp) {
      oss << Utils::get_timestamp_string(entry.timestamp) << ": ";
    }
    oss << entry.text << std::endl;

    Gtk::TextBuffer::iterator iter = buffer->end();
    buffer->insert_with_tag(iter, oss.str(), tag);
  }
}

void SPMAppWindow::on_close_current_tab() {
  Gtk::Widget *current = m_stack->get_visible_child();
  if (!current) return;

  // Stop worker
  const Glib::ustring basename = m_stack->get_visible_child_name();
  for (auto worker : sp_workers) {
    if (basename == worker->get_name()) {
      worker->stop_work();

      auto it = std::find(sp_workers.begin(), sp_workers.end(), worker);
      std::size_t index = std::distance(sp_workers.begin(), it);
      sp_workers.erase(sp_workers.begin() + index);
      break;
    }
  }

  // ----

  std::vector<Gtk::Widget *> children;
  if (Gtk::Widget *child = m_stack->get_first_child()) {
    do {
      children.push_back(child);
      child = child->get_next_sibling();
    } while (child);
  }

  auto it = std::find(children.begin(), children.end(), current);
  std::size_t index = std::distance(children.begin(), it);

  children.erase(children.begin() + index);
  m_stack->remove(*current);

  if (children.size() <= 0) return;
  size_t next_child_index = index <= 0 ? index : index - 1;

  auto next_child = children.at(next_child_index);
  if (next_child) m_stack->set_visible_child(*next_child);
}

void SPMAppWindow::on_text_view_changed(
    Gtk::TextView *textView, Glib::RefPtr<Gio::Settings> port_settings) {
  update_lines();

  if (!port_settings->get_boolean("autoscroll")) return;
  auto buffer = textView->get_buffer();
  Gtk::TextIter end_iter = buffer->end();
  textView->scroll_to(end_iter);
}

void SPMAppWindow::on_export() {
  auto dialog = Gtk::FileDialog::create();
  dialog->set_initial_name(
      "export_filename");  // TODO: Use the current tab name

  auto filters = Gio::ListStore<Gtk::FileFilter>::create();

  auto filter_text = Gtk::FileFilter::create();
  filter_text->set_name("Text files");
  filter_text->add_mime_type("text/plain");
  filters->append(filter_text);

  auto filter_any = Gtk::FileFilter::create();
  filter_any->set_name("Any files");
  filter_any->add_pattern("*");
  filters->append(filter_any);

  dialog->set_filters(filters);

  sigc::slot<void(const Glib::RefPtr<Gio::AsyncResult> &)> slot =
      [this, dialog](const Glib::RefPtr<Gio::AsyncResult> &result) {
        try {
          auto file = dialog->save_finish(result);
          if (file) {
            auto view = get_visible_text_view();
            if (!view) {
              std::cout << "SPMAppWindow::on_export(): No visible "
                           "text view."
                        << std::endl;
              return;
            }

            auto buffer = view->get_buffer();
            Glib::ustring text = buffer->get_text();
            const std::string etag = {};
            std::string new_etag;

            file->replace_contents(text, etag, new_etag);
          }
        } catch (const Glib::Error &ex) {
          std::cerr << "Error: " << ex.what() << std::endl;
        }
      };

  dialog->save(slot);
}

void SPMAppWindow::update_words() {
  // auto tab = dynamic_cast<Gtk::ScrolledWindow
  // *>(m_stack->get_visible_child()); if (!tab) return;

  // auto view = dynamic_cast<Gtk::TextView *>(tab->get_child());
  // if (!view) {
  //   std::cout << "SPMAppWindow::update_words(): No visible text view"
  //             << std::endl;
  //   return;
  // }

  // auto buffer = view->get_buffer();
  // // Find all words in the text buffer
  // std::set<Glib::ustring> words;
  // auto start = buffer->begin();
  // Gtk::TextIter end;
  // while (start) {
  //   while (start && !start.starts_word()) start++;
  //   if (!start) break;

  //   end = start;
  //   end.forward_word_end();
  //   if (start == end) break;

  //   auto word = buffer->get_text(start, end, false);
  //   words.insert(word.lowercase());
  //   start = end;
  // }
  // while (auto child = m_words->get_first_child()) m_words->remove(*child);

  // for (const auto &word : words) {
  //   auto row = Gtk::make_managed<Gtk::Button>(word);
  //   row->signal_clicked().connect(
  //       sigc::bind(sigc::mem_fun(*this, &SPMAppWindow::on_find_word), row));
  //   m_words->append(*row);
  // }
}

void SPMAppWindow::update_lines() {
  const auto text = m_searchentry->get_text();
  auto view = get_visible_text_view();
  if (!view) {
    std::cout << "SPMAppWindow::on_search_text_changed(): No visible text view."
              << std::endl;
    return;
  }

  auto buffer = view->get_buffer();

  int count = 0;
  auto iter = buffer->begin();
  while (iter) {
    ++count;
    if (!iter.forward_line()) break;
  }
  m_lines->set_text(Glib::ustring::format(count));
}

void SPMAppWindow::on_action_open_serial_port() {
  try {
    auto window = dynamic_cast<Gtk::Window *>(this);
    auto open_serial_port_dialog = SPMOpenSerialPortDialog::create(*window);
    open_serial_port_dialog->present();

    open_serial_port_dialog->signal_hide().connect(
        [open_serial_port_dialog]() { delete open_serial_port_dialog; });
  } catch (const Glib::Error &ex) {
    std::cerr << "SPM::on_action_open_serial_port(): " << ex.what()
              << std::endl;
  } catch (const std::exception &ex) {
    std::cerr << "SPM::on_action_open_serial_port(): " << ex.what()
              << std::endl;
  }
}

void SPMAppWindow::on_font_changed(Glib::ustring key, Gtk::TextView *textView) {
  auto buffer = textView->get_buffer();
  auto tag = create_default_style_tag(textView);
  buffer->apply_tag(tag, buffer->begin(), buffer->end());
}

std::string SPMAppWindow::normalize_port_path(std::string port_path) {
  size_t pos = port_path.rfind("/");
  std::string port_name =
      (pos == std::string::npos) ? port_path : port_path.substr(pos + 1);

  return port_name;
}

Gtk::TextView *SPMAppWindow::get_visible_text_view() {
  auto view_box = dynamic_cast<Gtk::Box *>(m_stack->get_visible_child());
  if (!view_box) {
    std::cout << "SPMAppWindow::on_search_text_changed(): No visible "
                 "child."
              << std::endl;
    return nullptr;
  }

  auto tab = dynamic_cast<Gtk::ScrolledWindow *>(
      Utils::get_nth_child_of_box(view_box, 2));
  if (!tab) {
    std::cout << "SPMAppWindow::on_search_text_changed(): no visible "
                 "scrolled-window in view box in 2nd position."
              << std::endl;
    return nullptr;
  }

  auto view = dynamic_cast<Gtk::TextView *>(tab->get_child());
  if (!view) {
    std::cout << "SPMAppWindow::on_search_text_changed(): No visible text view."
              << std::endl;
    return nullptr;
  }
  return view;
}

Glib::RefPtr<Gtk::TextTag> SPMAppWindow::create_default_style_tag(
    Gtk::TextView *textView) {
  auto buffer = textView->get_buffer();
  auto tag = buffer->create_tag();

  const Glib::ustring fontValue = m_settings->get_string("font");
  tag->property_font() = fontValue;
  // tag->property_pixels_above_lines() = 0;
  // tag->property_letter_spacing() = 0;
  // tag->property_pixels_below_lines() = 0;

  return tag;
}
