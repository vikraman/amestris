#include "qws.h"
#include "defs.h"
#include "helpers.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

void launch_qws(char * virt_devname)
{
    /* set to path of qws server */
    char * qws_server = "./HelloQt/HelloQt";

    /* use -qws argument to lauch default qws server automatically */
    char * qws_argv[] = {NULL, "-qws", NULL};

    /* initialize environment variables using value of virtual input device node */
    char qws_envp_kbd[MAX], qws_envp_mouse[MAX];
    char * qws_envp[] = {concat(qws_envp_kbd, "QWS_KEYBOARD=LinuxInput:", virt_devname), concat(qws_envp_mouse, "QWS_MOUSE_PROTO=LinuxInput:", virt_devname)};

    /* use this to launch qws server */
    //execve(qws_server, qws_argv, qws_envp);
    
    /* using this for launching HelloQt */
    char qws_shell[200];
    concat(qws_shell, "./HelloQt/HelloQt.sh ", virt_devname);
    puts(qws_shell);
    system(qws_shell);
}
