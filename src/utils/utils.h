#pragma once

#include <gtkmm-4.0/gtkmm.h>

#include <string>

class Utils {
 public:
  // TODO: move template t in a way no only works with std::vector but every
  // type could be using with find
  template <typename T>
  static int indexOf(std::vector<T> v, T el) {
    auto it = find(v.begin(), v.end(), el);
    int index = it - v.begin();
    return index;
  }

  static int find_index_in_string_list(
      const Glib::RefPtr<Gtk::StringList>& string_list,
      const Glib::ustring& target_string);

  // Helper function to find a child widget by its type and relative position
  // within a parent Gtk::Box, using GTK4 traversal methods.
  // I dont like look for child widget in that way. Replace if we catch some way
  // better
  static Gtk::Widget* get_nth_child_of_box(Gtk::Box* parent_box, int n);

  // locale-safe way to trim leading and trailing
 static Glib::ustring trim(const Glib::ustring& str);

  // default format: YYYY-MM-DD HH:MM:SS
  static std::string generate_current_timestamp(std::string fmt);

};  // namespace Utils
