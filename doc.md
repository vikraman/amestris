**Qt4 based fault tolerant GUI server**

**Architecture**

1. Qt Windowing Server, which accepts requests from client programs to draw windows, widgets, etc.
2. Keyboard event device (`/dev/input/eventX`, file descriptor `kfd`), which can be used to read input events from the keyboard device, using the linux kernel event input interface.
3. Mouse event device (`/dev/input/eventY`, file descriptor `mfd`), which can be used to read input events from the mouse device, using the linux kernel event input interface.
4. Main thread launched when we execute `virt.c`.
5. Virtual input device (`/dev/uinput`, file descriptor `ufd`) provided by the linux kernel module uinput, to manually inject events after receiving them from the other computer etc.
6. Framebuffer device (`/dev/fbX`) which is used as the output device by the `QWS` application. It is provided the appropriate framebuffer driver in the kernel (uvesafb, intelfb, nvidiafb etc.).

**Implementation**

![working.png](Working diagram)

1. Read physical keyboard and mouse device names from `sysfs`.
2. Create client and server threads.
3. Enable injecting keyboard keypress events, and mouse relative and buttonpress events into virtual uinput device.
4. Create virtual input device.
4. Join client and server threads.
5. Wait for udev to setup virtual input event device node.
6. Read virtual input device name from `sysfs`.
7. Fork and launch Qt windowing server.
8. Create read and write file descriptor sets. `kfd`, `mfd` and `cfd` are monitored for read events. `sfd` and `ufd` are monitored for write events.
9. Call `select()` to multiplex file descriptor sets.
10. If a read event is encountered on `kfd`, read a `struct input_event` from `kfd`, write to `ufd` and `sfd`.
11. If a read event is encountered on `mfd`, read a `struct input_event` from `mfd`, write to `ufd` and `sfd`.
12. If a read event is encountered on `cfd`, read a `struct input_event` from `cfd`, write to `ufd`.
13. Loop steps 8 to 12.
14. Close all file descriptors and sockets, and exit successfully.

**Installation**

*Prerequisites*

1. Linux kernel 2.6 or higher
2. Event input interface `(CONFIG_EVENT ?)` and `sysfs` `(CONFIG_SYSFS ?)` enabled in kernel config.
3. `CONFIG_UINPUT` set to module (m) or yes (y) for virtual input device.
4. Framebuffer support in kernel (or initramfs) with an appropriate framebuffer driver (vesafb/uvesafb, intelfb, nvidiafb etc.). Recent kernels (2.6.39+) support kernel mode setting (KMS), which enables framebuffer automatically if the gpu is whitelisted. The user running the gui server should have write access to the framebuffer device (`/dev/fbX`). Adding the user to the `video` group works on most distributions.
5. User running the gui server should have write access to all input devices and the uinput device. An udev rules file can be used to have udev automatically assign permissions. A sample rules file (99-input.rules) is provided which assigns the appropriate access rights to the group input. The group name (input) can be modified accordingly, and the file placed in `/etc/udev/rules.d`.
6. `Port 31415` should be open on both the computers, so that the servers can listen on this port. The port can be changed by altering the value of the macro `PORT` in `defs.h`. Both the computers should be assigned valid IPv4 addresses, which are to be supplied while running the gui server.

To run the gui server, execute the `virt` program, with the IPv4 address (or hostname) of the peer (fault tolerant computer) as an argument, and wait until an "initialization successful" message is printed. Note that, this should be done on both the computers.

Once the windowing server has started, gui clients can be executed. The clients must be started on both the computers. Events entered on one computer will be synced between both conputers, as long as the network connection holds up.
