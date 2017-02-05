#pragma once
#include <stdlib.h>
#include <sys/types.h>

#if _WIN32 || _WIN64
typedef ptrdiff_t ssize_t;
#endif

typedef int comparer(void *pLeft, void *pRight);

#define GenCompareFunc(type) int type##PtrComparer(type *pLeft, type *pRight) \
{\
	return *pLeft - *pRight;\
};

void shellSort(void *pArray, size_t iLen, size_t iItemSize, comparer fComparer);

void insertionSort(void *pArray, size_t iLen, size_t iItemSize, comparer fComparer);

ssize_t binarySearchMore(void *pArray, size_t iLen, size_t iItemSize, void *pValue, comparer fComparer);