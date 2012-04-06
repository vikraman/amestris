#include "qws.h"
#include "defs.h"
#include "helpers.h"
#include <unistd.h>
#include <stdlib.h>

void launch_qws(char * virt_devname)
{
    char * qws_server = "./HelloQt/HelloQt";
    char * qws_argv[] = {NULL, "-qws", NULL};
    char qws_envp_kbd[MAX], qws_envp_mouse[MAX];
    char * qws_envp[] = {concat(qws_envp_kbd, "QWS_KEYBOARD=LinuxInput:", virt_devname), concat(qws_envp_mouse, "QWS_MOUSE_PROTO=LinuxInput:", virt_devname)};
    //execve(qws_server, qws_argv, qws_envp);
    char qws_shell[200];
    concat(qws_shell, "./HelloQt/HelloQt.sh ", virt_devname);
    puts(qws_shell);
    system(qws_shell);
}
