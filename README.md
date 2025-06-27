# Serial Port Monitor

<p align=center>
  <img src="./resources/logo.png">
</p>

> [!WARNING]
> This software is unfinished. Keep your expectations low.

Basic serial port monitor heavily inspired by the [Arduino IDE monitor](https://docs.arduino.cc/software/ide-v2/tutorials/ide-v2-serial-monitor/).

## Background

Well, I just not want to install Arduino IDE for simple use him monitor. Also were a perfect time to learn how to use Gtk (Or better say Gtkmm, I consider use C Gtk, but I dont remenber why decided to the C++ Gtkmm, perhaps for the readable). I use a lot Gtkmm book example, more specifically the [A trivial applicaction](https://gnome.pages.gitlab.gnome.org/gtkmm-documentation/chapter-building-applications.html) was like the boilerplate.

The serial port comunication was posible thanks to [libserialport](https://sigrok.org/wiki/Libserialport) by Sigrok.

*If you want to test with a virtual port communication, there was a script in script folder that configure [tty0tty](https://github.com/freemed/tty0tty)* temporarily.

** The project has a recreational purpose. Use with carefull. **

## Requirements

* [Gtkmm 4.10 or later](https://gtkmm.gnome.org/en/index.html)
* [CMake](https://cmake.org)

## Build from source

External dependecies:

- Gtkmm-4.0 must be installed in you system.
- Libserialport is configure and installed under the fly, in the cmake build process.

```bash
mkdir -p build && cd build
cmake ..
make
```

For running just type the follow command

```bash
sp-monitor [/dev/ACM1, ...]
```
