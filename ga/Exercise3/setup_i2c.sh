echo -n "29" > /sys/class/gpio/export
echo -n "out" > /sys/class/gpio/gpio29/direction
echo -n "0" > /sys/class/gpio/gpio29/value
insmod /lib/modules/3.8.7-yocto-standard/kernel/drivers/i2c/i2c-dev.ko
