/* 
 * Read keyboard events from physical keyboard device
 * and write them to the virtual uinput device
 *
 * Usage : virt_kbd <physical keyboard device> <uinput device> <virtual device name>
 * Eg : virt_kbd /dev/input/event5 /dev/input/uinput virt-kbd
 *
 * @author Vikraman
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <error.h>
#include <errno.h>
#include <linux/input.h>
#include <linux/uinput.h>

int main (int argc, char ** argv) {
  if (argc != 4) return 1;

  /* file descriptors */
  int kfd = -1, ufd = -1, i;

  if (-1 == (kfd = open (argv[1], O_RDONLY)))
    error (EXIT_FAILURE, errno, "Can't open device");
  if (-1 == (ufd = open (argv[2], O_WRONLY)))
    error (EXIT_FAILURE, errno, "Can't open device");

  /* enable keyboard events */
  ioctl (ufd, UI_SET_EVBIT, EV_KEY);
  /* all keycodes */
  for (i = 0; i < KEY_MAX; i++)
    ioctl (ufd, UI_SET_KEYBIT, i);

  /* init uinput device */
  struct uinput_user_dev uidev;
  memset (&uidev, 0, sizeof (struct uinput_user_dev));
  strncpy (uidev.name, argv[3], UINPUT_MAX_NAME_SIZE);
  uidev.id.bustype = BUS_USB;
  uidev.id.vendor = 0x1234;
  uidev.id.product = 0x1234;
  uidev.id.version = 1;
  write (ufd, &uidev, sizeof (uidev));

  /* create device */
  ioctl (ufd, UI_DEV_CREATE);

  /* read events and write to virtual device */
  struct input_event ev;
  ssize_t ret;
  while (1) {
    /* add code here to receive/send events from/to other computer */
    if ((ret = read (kfd, &ev, sizeof (struct input_event))) > 0)
      write (ufd, &ev, sizeof (struct input_event));
    else if (ret == -1) break;
  }

  /* destroy device */
  ioctl (ufd, UI_DEV_DESTROY);
  close (ufd);
  close (kfd);
  return EXIT_SUCCESS;
}
