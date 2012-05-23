#!/bin/bash

# ignore cases
shopt -s nocasematch

function search ()
{
    # linear search on all event devices
    for i in /sys/class/input/event*
    do
        # check names
        if [[ "$(cat "${i}/device/name")" =~ "$@" ]]
        then
            source "${i}/uevent"
            # DEVNAME is the variable provided in uevent
            echo "/dev/${DEVNAME}"
            break
        fi
    done
}

# search for the device name specified in the argument
search "$@"
