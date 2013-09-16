#include "searchAlgo.h"

int sequenRecur(int *array, int now, int size, int target) {
	if ( array[now] == target ) return now;
	if ( size == now - 1 ) return NOT_FOUND;
	return sequenRecur( array, now + 1, size, target);
}

int sequenItera(int *array, int size, int target) {
	int i;
	for (i = 0; i <= size; i++) {
		if (array[i] == target)
			return i;
	}
	return NOT_FOUND;
}

int binaryRecur(int *array, int start, int end, int target) {
	int midd = (start + end) / 2;
	if (start > end)
		return 	NOT_FOUND;
	if (array[midd] > target)
		return binaryRecur(array, start, midd - 1, target);
	if (array[midd] < target)
		return binaryRecur(array, midd + 1, end, target);
	if (array[midd] == target)
		return midd;
}

int binaryItera(int *array, int size, int target) {
	int start = 0;
	int end = size;
	while (start <= end) {
		int midd = (start + end) / 2;
		if (array[midd] > target)
			end = midd - 1;
		else if (array[midd] < target)
			start = midd + 1;
		else if (array[midd] == target)
			return target;
	}
	return NOT_FOUND;
}
