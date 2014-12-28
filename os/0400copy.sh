#!/bin/sh
SRCROOT=`pwd`

# make a copy of ext2 to yzzext2
cd fs
rm -rf yzzext2
cp -R ext2 yzzext2
mv yzzext2/ext2.h yzzext2/yzzext2.h

cd ../include/linux
cp ext2_fs.h yzzext2_fs.h
# cp ext2_fs_sb.h yzzext2_fs_sb.h

cd $SRCROOT/include/asm-generic/bitops/
cp ext2-atomic.h yzzext2-atomic.h
cp ext2-atomic-setbit.h yzzext2-atomic-setbit.h
asm_files=`ls $PWD/yzzext*`

# back to src root
cd $SRCROOT
cd fs/yzzext2
files=`ls`
files="$asm_files $files $SRCROOT/include/linux/yzzext2_fs.h"
# subsitute ext2 to yzzext2
for f in $files
do

    # FIXME: check if $f exists

    echo -n "prepending yzz/YZZ in $f.."
    sed 's/ext2/yzzext2/g' < $f         | \
    sed "s/Second/Yuanzhang\'s Second/g"   | \
    sed 's/Ext2/yzzExt2/g' > .subtmp && \
    sed 's/EXT2/YZZEXT2/g' < .subtmp > $f && \
    rm .subtmp
    echo -e " \tparfait"

done
