#!/bin/sh

function search ()
{
    for i in /sys/class/input/event*
    do
        if [[ "$(cat $i/device/name)" =~ "$@" ]]
        then
            echo /dev/input/$(basename $i)
            break
        fi
    done
}

search "$1"
