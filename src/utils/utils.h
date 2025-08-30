#pragma once

#include <gtkmm-4.0/gtkmm.h>

#include <algorithm>
#include <chrono>
#include <string>

class Utils {
 public:
  template <typename T, typename V>
  static int indexOf(T v, V el) {
    auto it = std::find(v.begin(), v.end(), el);
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
  static std::string get_timestamp_string(
      std::chrono::_V2::system_clock::time_point timestamp =
          std::chrono::system_clock::now(),
      std::string fmt = "%Y-%m-%d %H:%M:%S");

  static Glib::ustring get_string_from_object_base(
      const Glib::RefPtr<Glib::ObjectBase>& object);

  static int get_int_from_object_base(
      const Glib::RefPtr<Glib::ObjectBase>& object);

};  // namespace Utils
