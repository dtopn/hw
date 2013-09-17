#ifndef SEARCHALGO_H
#define SEARCHALGO_H

#define NOT_FOUND -1
extern int sequenRecur(int *array, int now, int size, int target);
/* recursive version of sequential search */
extern int sequenItera(int *array, int size, int target); 
/* iterative version of sequential search */
extern int binaryRecur(int *array, int start, int end, int target);
/* recursive version of binary search */
extern int binaryItera(int *array, int size, int target);
/* iterative version of binary search */

#endif //SEARCHALGO_H
