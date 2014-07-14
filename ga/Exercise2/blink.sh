#!/bin/sh
# This shell script will let The port on Galileo board number 2 and 3 blink
# Not Like this now(Which will make use of internal pull-up resister on this two ports

# Setup class files
# 2
echo -n "31" > /sys/class/gpio/export 
echo -n "14" > /sys/class/gpio/export 
echo -n "0" > /sys/class/gpio/export 
# 3
echo -n "30" > /sys/class/gpio/export 
echo -n "15" > /sys/class/gpio/export 
echo -n "1" > /sys/class/gpio/export 
# Turn them on
echo "out" > /sys/class/gpio/gpio0/direction 
echo "out" > /sys/class/gpio/gpio1/direction 
echo "out" > /sys/class/gpio/gpio14/direction 
echo "out" > /sys/class/gpio/gpio15/direction 
echo "out" > /sys/class/gpio/gpio30/direction 
echo "out" > /sys/class/gpio/gpio31/direction 

echo "0" > /sys/class/gpio/gpio31/value
echo "0" > /sys/class/gpio/gpio30/value

# Select mux for output
# Port 2
echo "0" > /sys/class/gpio/gpio0/value # Set to 1 to pull-up to 3.3v
echo "1" > /sys/class/gpio/gpio14/value

# Port 3
echo "0" > /sys/class/gpio/gpio1/value
echo "1" > /sys/class/gpio/gpio15/value

read

# Unregister class files
# 2
echo -n "31" > /sys/class/gpio/unexport 
echo -n "14" > /sys/class/gpio/unexport 
echo -n "0" > /sys/class/gpio/unexport 
# 3
echo -n "30" > /sys/class/gpio/unexport 
echo -n "15" > /sys/class/gpio/unexport 
echo -n "1" > /sys/class/gpio/unexport 
