#ifndef SPM_ABOUT_DIALOG_H
#define SPM_ABOUT_DIALOG_H

#include <gtkmm-4.0/gtkmm.h>


class SPMAboutDialog : public Gtk::Window {
 public:
  SPMAboutDialog(BaseObjectType *cobject,
                    const Glib::RefPtr<Gtk::Builder> &refBuilder);
  static SPMAboutDialog *create(Gtk::Window &parent);

 protected:
  Glib::RefPtr<Gtk::Builder> m_refBuilder;
};
#endif // SPM_ABOUT_DIALOG_H