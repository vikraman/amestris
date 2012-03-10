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

/* because Pi */
#define PORT 31415
#define PEER "192.168.2.10"

void * init_server (void * sfd);
void * init_client (void * cfd);

int main (int argc, char ** argv)
{
    int i, kfd, mfd, ufd, cfd, sfd;
    fd_set rfds, wfds;
    struct input_event ev;
    struct uinput_user_dev uidev;
    pthread_t serv_thread, cli_thread;

    /*  TODO necessary ? */
    kfd = mfd = ufd = cfd = sfd = -1;

    /* TODO autodetect from dev name ? */
    if (argc != 4)
        {
            puts ("Need keyboard, mouse and uinput device nodes");
            exit (EXIT_FAILURE);
        }

    if (-1 == (kfd = open (argv[1], O_WRONLY)))
        error (EXIT_FAILURE, errno, "Can't open keyboard");
    if (-1 == (mfd = open (argv[2], O_RDONLY)))
        error (EXIT_FAILURE, errno, "Can't open mouse");
    if (-1 == (ufd = open (argv[3], O_RDONLY)))
        error (EXIT_FAILURE, errno, "Can't open virtual device");

    /* begin dirty socket work */
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
    /* all keycodes */
    ioctl (ufd, UI_SET_KEYBIT, BTN_LEFT);
    ioctl (ufd, UI_SET_KEYBIT, BTN_MIDDLE);
    ioctl (ufd, UI_SET_KEYBIT, BTN_RIGHT);
    for (i = 0; i < REL_MAX; i++)
        ioctl (ufd, UI_SET_RELBIT, i);

    /* init uinput device */
    memset (&uidev, 0, sizeof (struct uinput_user_dev));
    /* TODO get this name from elsewhere */
    strncpy (uidev.name, "virt", UINPUT_MAX_NAME_SIZE);
    uidev.id.bustype = BUS_USB;
    /* trolling */
    uidev.id.vendor = 0x1234;
    uidev.id.product = 0x1234;
    uidev.id.version = 1;
    write (ufd, &uidev, sizeof (uidev));

    /* create device */
    ioctl (ufd, UI_DEV_CREATE);

    /* since udev runs async */
    /* TODO udevadm trigger */
    sleep (5);

    /* TODO should I wait this late ? */
    pthread_join (cli_thread, NULL);
    pthread_join (serv_thread, NULL);

    while (1)
        {
            /* monitor read on kfd, mfd and cfd */
            FD_ZERO (&rfds);
            FD_SET (kfd, &rfds);
            FD_SET (mfd, &rfds);
            FD_SET (cfd, &rfds);

            /* monitor write on sfd and ufd */
            FD_ZERO (&wfds);
            FD_SET (sfd, &wfds);
            FD_SET (ufd, &wfds);

            select (FD_SETSIZE, &rfds, &wfds, NULL, NULL);

            if (FD_ISSET (kfd, &rfds))
                {
                    read (kfd, &ev, sizeof (struct input_event));
                    while (! (FD_ISSET (ufd, &wfds)));
                    write (ufd, &ev, sizeof (struct input_event));
                    FD_SET (ufd, &wfds);
                    /* TODO could this block ? */
                    while (! (FD_ISSET (sfd, &wfds)));
                    write (sfd, &ev, sizeof (struct input_event));
                    FD_SET (sfd, &wfds);
                }

            if (FD_ISSET (mfd, &rfds))
                {
                    read (mfd, &ev, sizeof (struct input_event));
                    while (! (FD_ISSET (ufd, &wfds)));
                    write (ufd, &ev, sizeof (struct input_event));
                    FD_SET (ufd, &wfds);
                    /* TODO could this block ? */
                    while (! (FD_ISSET (sfd, &wfds)));
                    write (sfd, &ev, sizeof (struct input_event));
                    FD_SET (sfd, &wfds);
                }

            if (FD_ISSET (cfd, &rfds))
                {
                    read (cfd, &ev, sizeof (struct input_event));
                    while (! (FD_ISSET (ufd, &wfds)));
                    write (ufd, &ev, sizeof (struct input_event));
                    FD_SET (ufd, &wfds);
                    /* TODO could this block ? */
                    while (! (FD_ISSET (sfd, &wfds)));
                    write (sfd, &ev, sizeof (struct input_event));
                    FD_SET (sfd, &wfds);
                }
        }

    /* kill the bad guys */
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

    sock = socket (AF_INET, SOCK_STREAM, 0);
    /* no TIME_WAIT for me */
    setsockopt (sock, SOL_SOCKET, SO_REUSEADDR, &on, sizeof (on));

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = PORT;
    servaddr.sin_addr.s_addr = htonl (INADDR_ANY);

    /* TODO fix this terrible fuckup */
    while (-1 == bind (sock, (struct sockaddr *) &servaddr, sizeof (servaddr)))
        perror ("bind");

    /* TODO test backlog */
    while (-1 == listen (sock, 0))
        perror ("listen");

    /* this blocks */
    *sfd = accept (sock, NULL, NULL);
    close (sock);

    puts ("server init complete");
    return NULL;
}

void * init_client (void * p_cfd)
{
    struct sockaddr_in saddr;
    int * cfd = p_cfd;

    *cfd = socket (AF_INET, SOCK_STREAM, 0);

    saddr.sin_family = AF_INET;
    saddr.sin_port = PORT;
    saddr.sin_addr.s_addr = inet_addr (PEER);

    /* TODO test this, need sleep ? */
    while (-1 == connect (*cfd, (struct sockaddr*) &saddr, sizeof (struct sockaddr)));

    puts ("client init complete");
    return NULL;
}
