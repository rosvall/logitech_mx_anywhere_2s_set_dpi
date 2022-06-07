#!/bin/sh -e

# Stupid hack to get the name of the corresponding /dev/hidrawN path
cd "$1"
cd device
cd hidraw
DEVNAME=`ls`

exec /usr/local/bin/logitech_mx_anywhere_2s_set_dpi /dev/"$DEVNAME" "$2"
