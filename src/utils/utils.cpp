#include "utils.h"

#include <bits/stdc++.h>

#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

int Utils::find_index_in_string_list(
    const Glib::RefPtr<Gtk::StringList>& string_list,
    const Glib::ustring& target_string) {
  // Check if the list is valid
  if (!string_list) return -1;

  // Iterate through each item in the list
  for (guint i = 0; i < string_list->get_n_items(); ++i) {
    // Get the item at the current index
    Glib::RefPtr<Glib::ObjectBase> item_base = string_list->get_object(i);

    // Attempt to cast the item to a Gtk::StringObject
    // Use dynamic_pointer_cast because Glib::ObjectBase is a virtual base
    Glib::RefPtr<Gtk::StringObject> string_object =
        std::dynamic_pointer_cast<Gtk::StringObject>(item_base);

    // If the cast was successful and the string matches, return the index
    if (string_object && string_object->get_string() == target_string) {
      return static_cast<int>(i);  // Found the string at this index
    }
  }

  // If the loop finishes, the string was not found in the list
  return -1;
}

Gtk::Widget* Utils::get_nth_child_of_box(Gtk::Box* parent_box, int n) {
  if (!parent_box) return nullptr;

  Gtk::Widget* current_child = parent_box->get_first_child();
  int count = 0;
  while (current_child) {
    if (count == n) {
      return current_child;
    }
    current_child = current_child->get_next_sibling();
    count++;
  }
  return nullptr;  // Child not found at this position
}

// locale-safe way to trim leading and trailing
Glib::ustring Utils::trim(const Glib::ustring& str) {
  auto start = std::find_if_not(str.begin(), str.end(), ::isspace);
  auto end = std::find_if_not(str.rbegin(), str.rend(), ::isspace).base();

  if (start >= end) return "";

  return Glib::ustring(start, end);
}

// default format: YYYY-MM-DD HH:MM:SS
std::string Utils::get_timestamp_string(
    std::chrono::system_clock::time_point timestamp =
        std::chrono::system_clock::now(),
    std::string fmt = "%Y-%m-%d %H:%M:%S") {
  time_t currentTime_t =
      std::chrono::system_clock::to_time_t(timestamp);  // Convert to time_t
  std::tm* localTime =
      std::localtime(&currentTime_t);  // Convert to local time structure

  // std::cout << "Current timestamp (time_t): " << currentTime_t << std::endl;
  std::ostringstream oss;
  oss << "[" << std::put_time(localTime, fmt.c_str()) << "]";
  return oss.str();
}
