#!/bin/sh
# This shell script will let The port on Galileo board number 2 and 3 blink
# Not Like this now(Which will make use of internal pull-up resister on this two ports

# Setup class files
echo -n "27" > /sys/class/gpio/export 
# Turn them on
echo "out" > /sys/class/gpio/gpio27/direction 

while true
do
	echo "0" > /sys/class/gpio/gpio27/value
	sleep 1
	echo "1" > /sys/class/gpio/gpio27/value
	sleep 1
done

# Unregister class files
echo -n "27" > /sys/class/gpio/unexport 

return 0
