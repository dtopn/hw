1. Build
====
Cross-compile
----
1. `source` the enviroment set-up script.
2. `cd` into the the `Lab1` directory.
3. Fire the `make` command.
4. Copy the binary to the target device.

Native-build
----
1. Copy the working directory to the Galileo board.
2. Fire `make local` command.
3. Wait for a while
4. Done.

2. Install Kernel Module
====
1. Call `insmod Squeue.ko`

3. Run tests
====
1. Run the test script `test.sh`, it may take a long long time.
	+ The script will run the same program 9 times and record the output into test#.log and count.log respectively.
2. Or you can just try `./Squeue_test` it will only perform 600 enqueue and dequeue operations.
	+ The log file contains `sort`ed result, as you can observe, _id_ numbers are sequential 
	+ and the *line count* number in _count.log_ is always _600_

* * *

Tao J  
July 14th, 2014