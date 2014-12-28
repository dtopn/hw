Files for each assignments were indicated by the first two digits in the file name.


__Tested under Linux kernel__ `3.14.15`


#### preparation
precompile source, and make sure everthing goes well with clean kernel source
```bash
cd linux-3.14.15
make oldconfig
# just keep pressing enter, default vaules are fine
make localmodconfig
make menuconfig
make -j4

# root permission
make modules_install
make install
```

#### assignment 3
```bash
patch -p1 < 0301.patch

## fire these manually
# make -j4
# make modules_install
# make install
# reboot now
```
```bash
gcc 0302.c && ./a.out
```

#### assignment 4
```bash
# apply either 0400 or 0401
# bash 0400copy.sh
patch -p1 < 0401*.patch
patch -p1 < 0402*.patch

## fire these manually
# make -j4
# make modules_install
# make install
# reboot now
```
```bash
bash 0403*.sh
```
