#include "searchAlgo.h"

int sequenRecur(int *array, int now, int size, int target) {
	if ( array[now] == target ) return now; /* found item, return iterator */
	if ( size == now - 1 ) return NOT_FOUND; /*  reached the end of the array, then not found */
	return sequenRecur( array, now + 1, size, target); /* continue searching for item, check if the next item is the target */
}

int sequenItera(int *array, int size, int target) {
	int i;
	for (i = 0; i <= size; i++) { /* traverse the whole array */
		if (array[i] == target) /* if this item is the target then return the iterator */
			return i;
	}
	return NOT_FOUND; /* reached the end of the array, then not found */
}

int binaryRecur(int *array, int start, int end, int target) {
	int midd = (start + end) / 2;
	if (start > end) /* if start == end == mid after comparison and if not found then start must < end, then the search routine ends */
		return 	NOT_FOUND;
	if (array[midd] > target)
		return binaryRecur(array, start, midd - 1, target); /* continue search in latter half of the list */
	if (array[midd] < target)
		return binaryRecur(array, midd + 1, end, target); /* continue search in former half of the list */

	if (array[midd] == target) /* item found */
		return midd;
}

int binaryItera(int *array, int size, int target) {
	int start = 0;
	int end = size;
	while (start <= end) { /* decide wether the comparison routine ends*/
		int midd = (start + end) / 2; /* the same as above */
		if (array[midd] > target)
			end = midd - 1;
		else if (array[midd] < target)
			start = midd + 1;
		else if (array[midd] == target)
			return target;
	}
	return NOT_FOUND;
}
