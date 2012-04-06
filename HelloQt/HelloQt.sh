#!/bin/bash

export QWS_KEYBOARD="LinuxInput:$@"
export QWS_MOUSE_PROTO="LinuxInput:$@"
echo $QWS_KEYBOARD
echo $QWS_MOUSE_PROTO
./HelloQt/HelloQt -qws 
