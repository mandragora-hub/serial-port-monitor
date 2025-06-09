#/bin/bash

# tty0tty - linux null modem emulator v1.2 

Run this script when you want to text with this virtual serial port.

echo "Moving to tmp folder"
cd /tmp # Keep everything temp

echo "Installation:"
git clone https://github.com/freemed/tty0tty

echo "Build the kernel module from provided source"
cd tty0tty/module
make

echo "Install the new kernel module into the kernel modules directory"
sudo make modules_install

echo "Load the module"
sudo udevadm control --reload-rules
sudo depmod
sudo modprobe tty0tty


echo "Finish."