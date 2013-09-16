#!/bin/sh
exec=a
K=40000

if [ -f stats.log ]
then
	rm stats.log
fi

for func in 1 3 2 4
do
for var1 in 100 500 1000 2000 4000 6000 8000 10000
do
	echo -n "size:$var1, target:$((var1 + 1)), K:$K, func:"
	case $func in
		1) echo "binaryRecur";;
		2) echo "sequenRecur";;
		3) echo "binaryItera";;
		4) echo "sequenItera";;
	esac
	$exec $var1 $((var1 + 1)) $K $func | tee -a stats.log
done
echo "-+-+-+-" | tee -a stats.log
done
