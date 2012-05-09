**Architecture**

1. Qt Windowing Server, which accepts requests from client programs to draw windows, widgets, etc.
2. Keyboard event device (/dev/input/eventX, file descriptor kfd), which can be used to read input events from the keyboard device, using the linux kernel event
input interface.
3. Mouse event device (/dev/input/eventY, file descriptor mfd), which can be used to read input events from the mouse device, using the linux kernel event
input interface.
4. Main thread launched when we execute virt.c
5. Virtual input device (/dev/uinput, file descriptor ufd) provided by the linux kernel module uinput, to manually
inject events after receiving them from the other computer etc.
6. Framebuffer device (/dev/fb0) which is used as the output device by the QWS application. It is provided the
appropriate framebuffer driver in the kernel (uvesafb, intelfb, nvidiafb etc.).

**Working**

![working.png](Working diagram)

**Implementation**

1. Read physical keyboard and mouse device names from sysfs.
2. Create client and server threads.
3. Enable injecting keyboard keypress events, and mouse relative and buttonpress events into virtual uinput device.
4. Create uinput device.
4. Join client and server threads.
5. Wait for udev to setup uinput event device node.
6. Read uinput device name from sysfs.
7. Fork and launch Qt windowing server.
8. Create read and write file descriptor sets. kfd, mfd and cfd are monitored for read events. sfd and ufd are monitored
for write events.
9. Call select() to multiplex file descriptor sets.
10. If a read event is encountered on kfd, read a struct input_event from kfd, write to ufd and sfd.
11. If a read event is encountered on mfd, read a struct input_event from mfd, write to ufd and sfd.
12. If a read event is encountered on cfd, read a struct input_event from cfd, write to ufd.
13. Loop steps 8 to 12.
14. Close all file descriptors and sockets, and exit successfully.
