#!/bin/bash

shopt -s nocasematch

function search ()
{
    for i in /sys/class/input/event*
    do
        if [[ "$(cat "${i}/device/name")" =~ "$@" ]]
        then
            source "${i}/uevent"
            echo "/dev/${DEVNAME}"
            break
        fi
    done
}

search "$@"
