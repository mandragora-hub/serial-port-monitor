#include "spm_open_serial_port_dialog.h"

#include <array>
#include <stdexcept>

#include "spm_app_window.h"

// namespace {
// struct TransitionTypeStruct {
//   Glib::ustring id;
//   Glib::ustring text;
// };

// const std::array<TransitionTypeStruct, 3> transitionTypes = {
//     TransitionTypeStruct{"none", "None"},
//     TransitionTypeStruct{"crossfade", "Fade"},
//     TransitionTypeStruct{"slide-left-right", "Slide"}};
// }  // namespace

SPMOpenSerialPortDialog::SPMOpenSerialPortDialog(
    BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refBuilder)
    : Gtk::Window(cobject), m_refBuilder(refBuilder) {
  // m_font = m_refBuilder->get_widget<Gtk::FontDialogButton>("font");
  // if (!m_font) throw std::runtime_error("No \"font\" object in prefs.ui");

  // m_transition = m_refBuilder->get_widget<Gtk::DropDown>("transition");
  // if (!m_transition)
  //   throw std::runtime_error("No \"transition\" object in prefs.ui");

  //
  // auto string_list = Gtk::StringList::create();
  // for (const auto& transitionType : transitionTypes)
  //   string_list->append(transitionType.text);

  // m_transition->set_model(string_list);
  // m_settings = Gio::Settings::create("org.gtkmm.spmonitor");

  // Connect preference properties to the Gio::Settings.

  // This is easier when g_settings_bind_with_mapping() is
  // wrapped in a Gio::Settings method.
  // m_settings->signal_changed("font").connect(
  //     sigc::mem_fun(*this, &SPMOpenSerialPortDialog::on_font_setting_changed));
  // m_font->property_font_desc().signal_changed().connect(
  //     sigc::mem_fun(*this, &SPMOpenSerialPortDialog::on_font_selection_changed));

  // m_settings->signal_changed("transition")
  //     .connect(sigc::mem_fun(
  //         *this, &SPMOpenSerialPortDialog::on_transition_setting_changed));
  // m_transition->property_selected().signal_changed().connect(sigc::mem_fun(
  //     *this, &SPMOpenSerialPortDialog::on_transition_selection_changed));

  // // Synchronize the preferences dialog with m_settings.
  // on_font_setting_changed("font");
  // on_transition_setting_changed("transition");

}

SPMOpenSerialPortDialog* SPMOpenSerialPortDialog::create(Gtk::Window& parent) {
  // Load the builder file and instantiate its widgets.
  auto refBuilder =
      Gtk::Builder::create_from_resource("/org/gtkmm/spmonitor/resources/prefs.ui");

  auto dialog = Gtk::Builder::get_widget_derived<SPMOpenSerialPortDialog>(
      refBuilder, "prefs_dialog");
  if (!dialog)
    throw std::runtime_error("No \"prefs_dialog\" object in prefs.ui");

  dialog->set_transient_for(parent);
  return dialog;
}

