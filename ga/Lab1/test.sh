#!/bin/sh

./Squeue_tester show
./Squeue_tester | tee test0.log | wc -l >  count.log
./Squeue_tester | tee test1.log | wc -l >> count.log
./Squeue_tester | tee test2.log | wc -l >> count.log
./Squeue_tester | tee test3.log | wc -l >> count.log
./Squeue_tester | tee test4.log | wc -l >> count.log
./Squeue_tester | tee test5.log | wc -l >> count.log
./Squeue_tester | tee test6.log | wc -l >> count.log
./Squeue_tester | tee test7.log | wc -l >> count.log
./Squeue_tester | tee test8.log | wc -l >> count.log
