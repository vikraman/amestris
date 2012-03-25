#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main()
{
    FILE * fp;
    char kbd[80], mouse[80], qwskbd[80], qwsmouse[80];
    fp = popen("get_devices.sh keyboard", "r");
    fscanf(fp, "%s", kbd);
    pclose(fp);
    strcpy(qwskbd, "QWS_KEYBOARD=LinuxInput:");
    strcat(qwskbd, kbd);
    fp = popen("get_devices.sh Mouse", "r");
    fscanf(fp, "%s", mouse);
    pclose(fp);
    strcpy(qwsmouse, "QWS_MOUSE_PROTO=LinuxInput:");
    strcat(qwsmouse, mouse);
    char * file = "/home/vh4x0r/devel/amestris/HelloQt/HelloQt";
    char * const argv[] = {file, "-qws", NULL};
    char * const envp[] = {qwskbd, qwsmouse};
    char cmd[80];
    strcpy(cmd, "sudo chmod 666 ");
    strcat(cmd, kbd);
    system(cmd);
    strcpy(cmd, "sudo chmod 666 ");
    strcat(cmd, mouse);
    system(cmd);
    if (!fork())
        execve(file, argv, envp);
    return 0;
}
