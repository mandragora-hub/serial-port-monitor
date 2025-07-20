#include "spm_about_dialog.h"

#include <array>
#include <stdexcept>

#include "spm_app_window.h"

SPMAboutDialog::SPMAboutDialog(
    BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refBuilder)
    : Gtk::Window(cobject), m_refBuilder(refBuilder) {
  
  auto content_label_text = m_refBuilder->get_widget<Gtk::Label>("content-text-label");
  if (!content_label_text) throw std::runtime_error("No \"content-text-label\" object in about_dialog.ui");


  // Create about text
  try {
    // auto about_text = Gtk::make_managed<Gtk::Label>();
    // auto welcome_box = Gtk::make_managed<Gtk::CenterBox>();

    Glib::RefPtr<Gio::File> about_text_file = Gio::File::create_for_uri(
        "resource:///org/gtkmm/spmonitor/ui/about_text.xml");
    char *contents = nullptr;
    gsize length = 0;

    about_text_file->load_contents(contents, length);

    content_label_text->set_markup(Glib::ustring(contents));
    // about_text->set_wrap(true);
    content_label_text->set_justify(Gtk::Justification::CENTER);

    // welcome_box->set_center_widget(*content_label_text);
    // welcome_box->set_expand();

    // m_stack->add(*welcome_box, "Welcome", "Welcome");

    g_free(contents);

  } catch (const Glib::Error &ex) {
    std::cerr << ex.what() << std::endl;
  }


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
  //     sigc::mem_fun(*this, &SPMAboutDialog::on_font_setting_changed));
  // m_font->property_font_desc().signal_changed().connect(
  //     sigc::mem_fun(*this, &SPMAboutDialog::on_font_selection_changed));

  // m_settings->signal_changed("transition")
  //     .connect(sigc::mem_fun(
  //         *this, &SPMAboutDialog::on_transition_setting_changed));
  // m_transition->property_selected().signal_changed().connect(sigc::mem_fun(
  //     *this, &SPMAboutDialog::on_transition_selection_changed));

  // // Synchronize the preferences dialog with m_settings.
  // on_font_setting_changed("font");
  // on_transition_setting_changed("transition");

}

SPMAboutDialog* SPMAboutDialog::create(Gtk::Window& parent) {
  // Load the builder file and instantiate its widgets.
  auto refBuilder =
      Gtk::Builder::create_from_resource("/org/gtkmm/spmonitor/ui/about_dialog.ui");

  auto dialog = Gtk::Builder::get_widget_derived<SPMAboutDialog>(
      refBuilder, "about_dialog");
  if (!dialog)
    throw std::runtime_error("No \"about_dialog\" object in prefs.ui");

  dialog->set_transient_for(parent);
  return dialog;
}

