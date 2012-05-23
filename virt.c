#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <error.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <linux/input.h>
#include <linux/uinput.h>

#include "helpers.h"
#include "defs.h"
#include "qws.h"

/* longest length of an ip address */
char peer[INET_ADDRSTRLEN];

void * init_server (void * sfd);
void * init_client (void * cfd);

int main (int argc, char ** argv)
{
    /* usage: virt <ip address of peer> */
    if (argc != 2) return 1;
    strcpy(peer, argv[1]);

    int i, kfd, mfd, ufd, cfd, sfd, fd_max;
    fd_set rfds, wfds;

    /* input event */
    struct input_event ev;

    /* to initialize uinput device */
    struct uinput_user_dev uidev;

    pthread_t serv_thread, cli_thread;
    char kbd_devname[MAX], mouse_devname[MAX], virt_devname[MAX];

    /* initialize file descriptors as invalid */
    kfd = mfd = ufd = cfd = sfd = -1;

    /* read keyboard device event node */
    pipexec("./get_devices.sh keyboard", kbd_devname);
    puts(kbd_devname);

    /* read mouse device event node */
    pipexec("./get_devices.sh mouse", mouse_devname);
    puts(mouse_devname);

    /* exit if devices cannot be opened */
    if (-1 == (kfd = open (kbd_devname, O_RDONLY)))
        error (EXIT_FAILURE, errno, "Can't open keyboard");
    if (-1 == (mfd = open (mouse_devname, O_RDONLY)))
        error (EXIT_FAILURE, errno, "Can't open mouse");
    if (-1 == (ufd = open ("/dev/uinput", O_WRONLY)))
        error (EXIT_FAILURE, errno, "Can't open virtual device");

    /* launch threads to create sockets */
    pthread_create (&cli_thread, NULL, init_client, &cfd);
    pthread_create (&serv_thread, NULL, init_server, &sfd);

    /* enable keyboard events */
    ioctl (ufd, UI_SET_EVBIT, EV_KEY);
    /* all keycodes */
    for (i = 0; i < KEY_MAX; i++)
        ioctl (ufd, UI_SET_KEYBIT, i);

    /* enable mouse events */
    ioctl (ufd, UI_SET_EVBIT, EV_KEY);
    ioctl (ufd, UI_SET_EVBIT, EV_REL);
    /* all mouse button keycodes */
    ioctl (ufd, UI_SET_KEYBIT, BTN_LEFT);
    ioctl (ufd, UI_SET_KEYBIT, BTN_MIDDLE);
    ioctl (ufd, UI_SET_KEYBIT, BTN_RIGHT);
    for (i = 0; i < REL_MAX; i++)
        ioctl (ufd, UI_SET_RELBIT, i);

    /* init uinput device */
    memset (&uidev, 0, sizeof (struct uinput_user_dev));
    /* the device is internally called virt */
    strncpy (uidev.name, "virt", UINPUT_MAX_NAME_SIZE);
    uidev.id.bustype = BUS_USB;
    /* use arbitrary vendor and product ids */
    uidev.id.vendor = 0x1234;
    uidev.id.product = 0x1234;
    uidev.id.version = 1;
    write (ufd, &uidev, sizeof (uidev));

    /* create virtual device */
    ioctl (ufd, UI_DEV_CREATE);

    /* TODO find a way to check whether udev has created a device node */
    sleep (5);

    /* read event device name */
    pipexec("./get_devices.sh virt", virt_devname);
    puts(virt_devname);

    /* join threads */
    pthread_join (cli_thread, NULL);
    pthread_join (serv_thread, NULL);

    puts("All init done");

    /* start qws server here */
    if (!fork())
        launch_qws(virt_devname);

    while (1)
        {
            /* monitor read on kfd, mfd and cfd */
            FD_ZERO (&rfds);
            FD_SET (kfd, &rfds);
            FD_SET (mfd, &rfds);
            FD_SET (cfd, &rfds);

            /* FIXME skipping this since it is unnecessary */
            /* monitor write on sfd and ufd */
            /* FD_ZERO (&wfds); */
            /* FD_SET (sfd, &wfds); */
            /* FD_SET (ufd, &wfds); */

            /* calculate maximum socket value to initialize select() */
            fd_max = kfd;
            if (fd_max < mfd) fd_max = mfd;
            if (fd_max < ufd) fd_max = ufd;
            if (fd_max < cfd) fd_max = cfd;
            if (fd_max < sfd) fd_max = sfd;

            select (fd_max + 1, &rfds, &wfds, NULL, NULL);

            if (FD_ISSET (kfd, &rfds))
                {
                    read (kfd, &ev, sizeof (struct input_event));
                    puts("Input from local keyboard");
                    /* FIXME this might not always be true, requires testing */
                    /* while (! (FD_ISSET (ufd, &wfds))); */
                    write (ufd, &ev, sizeof (struct input_event));
                    puts("Output to virtual device");
                    FD_SET (ufd, &wfds);
                    /* FIXME this might not always be true, requires testing */
                    /* while (! (FD_ISSET (sfd, &wfds))); */
                    write (sfd, &ev, sizeof (struct input_event));
                    puts("Output to server");
                    FD_SET (sfd, &wfds);
                }

            else if (FD_ISSET (mfd, &rfds))
                {
                    read (mfd, &ev, sizeof (struct input_event));
                    puts("Input from local mouse");
                    /* FIXME this might not always be true, requires testing */
                    /* while (! (FD_ISSET (ufd, &wfds))); */
                    write (ufd, &ev, sizeof (struct input_event));
                    puts("Output to virtual device");
                    FD_SET (ufd, &wfds);
                    /* FIXME this might not always be true, requires testing */
                    /* while (! (FD_ISSET (sfd, &wfds))); */
                    write (sfd, &ev, sizeof (struct input_event));
                    puts("Output to server");
                    FD_SET (sfd, &wfds);
                }

            else if (FD_ISSET (cfd, &rfds))
                {
                    read (cfd, &ev, sizeof (struct input_event));
                    puts("Incoming event from remote server");
                    /* FIXME skipping this since it is unnecessary */
                    /* while (! (FD_ISSET (ufd, &wfds))); */
                    write (ufd, &ev, sizeof (struct input_event));
                    puts("Output to virtual device");
                    FD_SET (ufd, &wfds);
                }
        }

    /* kill all devices and sockets */
    ioctl (ufd, UI_DEV_DESTROY);
    close (ufd);
    close (kfd);
    close (mfd);
    close (cfd);
    close (sfd);
    return EXIT_SUCCESS;
}

void * init_server (void * p_sfd)
{
    struct sockaddr_in servaddr;
    int sock, * sfd = p_sfd, on = 1;

    /* initialize tcp socket */
    sock = socket (AF_INET, SOCK_STREAM, 0);

    /* make socket reusable to drop broken connections immediately */
    setsockopt (sock, SOL_SOCKET, SO_REUSEADDR, &on, sizeof (on));

    /* ipv4 */
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = PORT;
    servaddr.sin_addr.s_addr = htonl (INADDR_ANY);

    /* TODO gracefully handle the error */
    while (-1 == bind (sock, (struct sockaddr *) &servaddr, sizeof (servaddr)))
        perror ("bind");

    /* TODO backlog value */
    while (-1 == listen (sock, 0))
        perror ("listen");

    /* this blocks till an incoming connection appears */
    *sfd = accept (sock, NULL, NULL);
    close (sock);

    puts ("server init complete");
    return NULL;
}

void * init_client (void * p_cfd)
{
    struct sockaddr_in saddr;
    int * cfd = p_cfd;

    /* initialize tcp socket */
    *cfd = socket (AF_INET, SOCK_STREAM, 0);

    /* ipv4 */
    saddr.sin_family = AF_INET;
    saddr.sin_port = PORT;
    saddr.sin_addr.s_addr = inet_addr (peer);

    /* TODO try sleeping between connect() attempts */
    while (-1 == connect (*cfd, (struct sockaddr*) &saddr, sizeof (struct sockaddr)));

    puts ("client init complete");
    return NULL;
}
