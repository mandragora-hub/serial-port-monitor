# Serial Port Monitor

<p align=center>
  <img src="./resources/sp-monitor.png">
</p>

> [!WARNING]
> This software is unfinished. Keep your expectations low.

Basic serial port monitor heavily inspired by the [Arduino IDE monitor](https://docs.arduino.cc/software/ide-v2/tutorials/ide-v2-serial-monitor/).

## Background

Well, I just don't want to install Arduino IDE only to use its monitor. Also was a perfect time to learn how to use Gtk (Or better say Gtkmm, I consider using C Gtk, but I don't remember why I decided to the C++ Gtkmm, perhaps for the readability). I use a lot Gtkmm book example, more specifically the [A trivial application](https://gnome.pages.gitlab.gnome.org/gtkmm-documentation/chapter-building-applications.html) was like the boilerplate.

The serial port communication was possible thanks to [libserialport](https://sigrok.org/wiki/Libserialport) by Sigrok.

*If you want to test with a virtual port communication, there was a script in the script folder that configures [tty0tty](https://github.com/freemed/tty0tty)* temporarily.

** The project has a recreational purpose. Use with care. **

## Requirements

* [Gtkmm 4.10 or later](https://gtkmm.gnome.org/en/index.html)
* [CMake](https://cmake.org)

## Build from source

External dependecies:

- Gtkmm-4.0 must be installed in your system.
- Libserialport is configured and installed under the fly, in the CMake build process.

```bash
mkdir -p build && cd build
cmake ..
make
```

To run just type the following command

```bash
sp-monitor [/dev/ACM1, ...]
```
