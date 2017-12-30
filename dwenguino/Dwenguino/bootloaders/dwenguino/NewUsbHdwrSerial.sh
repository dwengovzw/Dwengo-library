#!/usr/bin/bash

# use whatever hash algorithm you prefer here:
# possibilities are md5sum (128 bits), sha<n>sum where <n> is 224,256,384,512
# there are probably others too.
hash=sha256sum

# change this "salt" to something else...anything else!
salt='Salt of the Earth'

sum=`( od -N 16 -t x /dev/random ; date ; ls -l | shuf ; echo "$salt" ) 2>&1 | $hash`

serial=${sum:0:20}
serial=`echo $serial | tr [:lower:] [:upper:]`

echo \#pragma once >NewUsbHdwrSerial.h
echo \#define USB_HDWR_SERIAL \"$serial\" >> NewUsbHdwrSerial.h

echo New serial number is $serial
echo Header file NewUsbHdwrSerial.h has been created.
echo This must be renamed to UsbHdwrSerial.h before it will take effect.
