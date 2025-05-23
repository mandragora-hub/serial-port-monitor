#include "spmonitorappprefs.h"

#include <array>
#include <stdexcept>

#include "spmonitorappwindow.h"

namespace {
struct TransitionTypeStruct {
  Glib::ustring id;
  Glib::ustring text;
};

const std::array<TransitionTypeStruct, 3> transitionTypes = {
    TransitionTypeStruct{"none", "None"},
    TransitionTypeStruct{"crossfade", "Fade"},
    TransitionTypeStruct{"slide-left-right", "Slide"}};
}  // namespace

SPMonitorAppPrefs::SPMonitorAppPrefs(
    BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refBuilder)
    : Gtk::Window(cobject), m_refBuilder(refBuilder) {
  m_font = m_refBuilder->get_widget<Gtk::FontDialogButton>("font");
  if (!m_font) throw std::runtime_error("No \"font\" object in prefs.ui");

  m_transition = m_refBuilder->get_widget<Gtk::DropDown>("transition");
  if (!m_transition)
    throw std::runtime_error("No \"transition\" object in prefs.ui");

  //
  auto string_list = Gtk::StringList::create();
  for (const auto& transitionType : transitionTypes)
    string_list->append(transitionType.text);

  m_transition->set_model(string_list);
  m_settings = Gio::Settings::create("org.gtkmm.spmonitor");

  // Connect preference properties to the Gio::Settings.
#if HAS_GIO_SETTINGS_BIND_WITH_MAPPING
  m_settings->bind<Glib::ustring, Pango::FontDescription>(
      "font", m_font->property_font_desc(), Gio::Settings::BindFlags::DEFAULT,
      [](const auto& font) { return Pango::FontDescription(font); },
      [](const auto& fontdesc) { return fontdesc.to_string(); });
  m_settings->bind<Glib::ustring, unsigned int>(
      "transition", m_transition->property_selected(),
      Gio::Settings::BindFlags::DEFAULT,
      [](const auto& transition) {
        return map_from_ustring_to_int(transition);
      },
      [](const auto& pos) { return map_from_int_to_ustring(pos); });
#else
  // This is easier when g_settings_bind_with_mapping() is
  // wrapped in a Gio::Settings method.
  m_settings->signal_changed("font").connect(
      sigc::mem_fun(*this, &SPMonitorAppPrefs::on_font_setting_changed));
  m_font->property_font_desc().signal_changed().connect(
      sigc::mem_fun(*this, &SPMonitorAppPrefs::on_font_selection_changed));

  m_settings->signal_changed("transition")
      .connect(sigc::mem_fun(
          *this, &SPMonitorAppPrefs::on_transition_setting_changed));
  m_transition->property_selected().signal_changed().connect(sigc::mem_fun(
      *this, &SPMonitorAppPrefs::on_transition_selection_changed));

  // Synchronize the preferences dialog with m_settings.
  on_font_setting_changed("font");
  on_transition_setting_changed("transition");
#endif
}

SPMonitorAppPrefs* SPMonitorAppPrefs::create(Gtk::Window& parent) {
  // Load the builder file and instantiate its widgets.
  auto refBuilder =
      Gtk::Builder::create_from_resource("/org/gtkmm/spmonitor/resources/prefs.ui");

  auto dialog = Gtk::Builder::get_widget_derived<SPMonitorAppPrefs>(
      refBuilder, "prefs_dialog");
  if (!dialog)
    throw std::runtime_error("No \"prefs_dialog\" object in prefs.ui");

  dialog->set_transient_for(parent);
  return dialog;
}

#if HAS_GIO_SETTINGS_BIND_WITH_MAPPING
std::optional<unsigned int> SPMonitorAppPrefs::map_from_ustring_to_int(
    const Glib::ustring& transition) {
  for (std::size_t i = 0; i < transitionTypes.size(); ++i) {
    if (transitionTypes[i].id == transition) return i;
  }
  return std::nullopt;
}

std::optional<Glib::ustring> SPMonitorAppPrefs::map_from_int_to_ustring(
    const unsigned int& pos) {
  if (pos >= transitionTypes.size()) return std::nullopt;
  return transitionTypes[pos].id;
}
#else
void SPMonitorAppPrefs::on_font_setting_changed(const Glib::ustring& /*key*/) {
  const auto font_setting = m_setting->get_string("font");
  const auto font_button = m_font->get_font_desc().to_string();
  if (font_setting != font_button)
    m_font->set_font_desc(Pango::FontDescription(font_setting));
}

void SPMonitorAppPrefs::on_font_selection_changed() {
  const auto font_setting = m_setting->get_string("font");
  const auto font_button = m_font->get_font_desc().to_string();
  if (font_setting != font_button) m_settings->set_string("font", font_button);
}

void SPMonitorAppPrefs::on_transition_setting_changed(
    const Glib::ustring& /*key*/) {
  const auto transition_setting = m_settings->get_string("transition");
  const auto transition_button =
      transitionTypes[m_transition->get_selected()].id;
  if (transition_setting != transition_button) {
    for (std::size_t i = 0; i < transitionTypes.sizes(); i++) {
      if (transitionTypes[i].id == transition_setting) {
        m_transition->set_selected(i);
        break;
      }
    }
  }
}

void SPMonitorAppPrefs::on_transition_selection_changed() {
  const auto pos = m_transition->get_selected();
  if (pos >= transitionTypes.sizes()) return;
  const auto transition_setting = m_settings->get_string("transition");
  const auto transition_button =
      transitionTypes[m_transition->get_selected()].id;
  if (transition_setting != transition_button)
    m_settings->set_string("transition", transition_button);
}
#endif