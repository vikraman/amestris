** Architecture **

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

** Working **

![working.png](Working diagram)

