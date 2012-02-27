#!/bin/bash
#
# Script to initialize all virtual devices
# and launch a qt-embedded program
#
# @author Vikraman

# Searches for the first event device whose name contains the substring passed as arguments
function search ()
{
  for i in /sys/class/input/event*
  do
    if [[ "$(cat $i/device/name)" =~ "$@" ]]
    then
      echo /dev/input/$(basename $i)
      break
    fi
  done
}

# Exits gracefully
function die ()
{
  echo $@
  pgrep virt_kbd >/dev/null && killall virt_kbd
  pgrep virt_mouse >/dev/null && killall virt_mouse
  unset QWS_KEYBOARD
  unset QWS_MOUSE_PROTO
  exit
}

echo "Initializing .."

# Device name might need to be changed depending on the keyboard used
KBD_DEVICE=$(search "keyboard")
[ -z $KBD_DEVICE ] && die "Cannot find keyboard device"

# Device name might need to be changed depending on the mouse used
MOUSE_DEVICE=$(search "Optical Mouse")
[ -z $MOUSE_DEVICE ] && die "Cannot find mouse device"

# Requires uinput to be compiled as a module
(lsmod | grep uinput >/dev/null) || sudo modprobe uinput
UINPUT_DEVICE="/dev/input/uinput"
[ -c $UINPUT_DEVICE ] || die "Cannot find uinput device"

# In case sudo is not available, modify these statements
sudo chmod 644 $KBD_DEVICE $MOUSE_DEVICE
sudo chmod 666 $UINPUT_DEVICE

VIRT_KBD_NAME="virt-kbd"
VIRT_MOUSE_NAME="virt-mouse"

# Virtual keyboard and mouse programs are in current directory
echo -n "Press Enter to start virtual kbd program .. "
read
./virt_kbd $KBD_DEVICE $UINPUT_DEVICE $VIRT_KBD_NAME &
echo -n "Press Enter to start virtual mouse program .. "
read
./virt_mouse $MOUSE_DEVICE $UINPUT_DEVICE $VIRT_MOUSE_NAME &

# Wait for udev to create the device nodes
sleep 2

# Search virtual device nodes
VIRT_KBD_DEVICE=$(search $VIRT_KBD_NAME)
[ -z $VIRT_KBD_DEVICE ] && die "Cannot find virtual keyboard device"
VIRT_MOUSE_DEVICE=$(search $VIRT_MOUSE_NAME)
[ -z $VIRT_MOUSE_DEVICE ] && die "Cannot find virtual mouse device"

# rw permissions on virtual devices required for Qt
sudo chmod 666 $VIRT_KBD_DEVICE $VIRT_MOUSE_DEVICE

# Qt input device variables
export QWS_KEYBOARD="LinuxInput:${VIRT_KBD_DEVICE}"
export QWS_MOUSE_PROTO="LinuxInput:${VIRT_MOUSE_DEVICE}"

# Display devices
echo "Keyboard : " $KBD_DEVICE "-->" $UINPUT_DEVICE "-->" $VIRT_KBD_DEVICE
echo "Mouse : " $MOUSE_DEVICE "-->" $UINPUT_DEVICE "-->" $VIRT_MOUSE_DEVICE

# Start HelloQt program in the current directory
echo "Starting Qt embedded application .."
./HelloQt/HelloQt -qws
echo "Quitting .."

die "Success :)"
