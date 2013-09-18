/*
C. Bonus 1 (P): Binary Search Tree

时间限制
50 ms
内存限制
32000 kB
代码长度限制
16000 B
判题程序
Standard
作者
CHEN, Yue
A binary search tree is uniquely determined by a given ordered insertions of a sequence of positive integers. On the other hand, a given binary search tree may correspond to several different insertion sequences. Now given several insertion sequences, it is your job to determine if they can generate the same binary search tree.

Input

Your program must read test cases from the standard input.

Input consists of several test cases. For each test case, the first line contains two positive integers N (<=10) and L, which are the total number of integers in an insertion sequence and the number of sequences to be tested, respectively. The second line contains N positive integers, separated by a space, which are the initially inserted numbers. Then L lines follow, each contains a sequence of N integers to be checked.

For convenience, it is guaranteed that each insertion sequence is a permutation of integers 1 to N -- that is, all the N numbers are distinct and no greater than N. The input ends with N being 0. That case must NOT be processed.

Output

For each test case, output to the standard output. Print in L lines the checking results: if the sequence given in the i-th line corresponds to the same binary search tree as the initial sequence, output to the i-th line "Yes"; else output "No".

Sample Input
4 2
3 1 4 2
3 4 1 2
3 2 4 1
2 1
2 1
1 2
0
Sample Output
Yes
No
No
 */
