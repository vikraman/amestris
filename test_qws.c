#include <stdio.h>
#include "qws.h"

int main()
{
    char devname[80];
    pipexec("./get_devices.sh keyboard", devname);
    puts(devname);
    launch_qws(devname);
    return 0;
}
