#include "Tools.h"
#include <malloc.h>
#include <memory.h>

#if _WIN32 || _WIN64
#include <basetsd.h>
#endif

#define ItemPtr(array, index, itemSize) (&((char*)(array))[(index) * (itemSize)])

ptrdiff_t binarySearchMore(void *pArray, size_t iLen, size_t iItemSize, void *pValue, comparer fComparer)
{
	if (iLen <= 0)
		return -1;

	if (iLen <= 4)
	{
		for (size_t i = 0; i < iLen; i++)
		{
			if (fComparer(ItemPtr(pArray, i, iItemSize), pValue) > 0)
				return i;
		}
	}

	size_t start = 0;
	size_t end = iLen - 1;
	size_t index = start + ((end - start) >> 1);

	if (fComparer(ItemPtr(pArray, end, iItemSize), pValue) <= 0)
		return -1;

	if (fComparer(ItemPtr(pArray, 0, iItemSize), pValue) > 0)
		return 0;

	for (;;)
	{
		void *pItem = ItemPtr(pArray, index, iItemSize);
		int cmp = fComparer(pItem, pValue);

		if (end - start == 1)
		{
			if (cmp <= 0)
				index++;
			break;
		}

		if (cmp > 0)
		{
			end = index;
		}
		else if (cmp <= 0)
		{
			start = index;
		}

		index = start + ((end - start) >> 1);
	}

	return index;
}

void insertionSort(void *pArray, size_t iLen, size_t iItemSize, comparer fComparer)
{
	void *pTempBuf = malloc(iItemSize);

	for (size_t i = 1; i < iLen; i++)
	{
		int cmp = fComparer(ItemPtr(pArray, i - 1, iItemSize), ItemPtr(pArray, i, iItemSize));
		if (cmp > 0)
		{
			ssize_t newIndex = binarySearchMore(pArray, i, iItemSize, ItemPtr(pArray, i, iItemSize), fComparer);

			if (newIndex == -1)
				newIndex = i;

			memmove(pTempBuf, ItemPtr(pArray, i, iItemSize), iItemSize);
			memmove(
				ItemPtr(pArray, newIndex + 1, iItemSize),
				ItemPtr(pArray, newIndex, iItemSize),
				(i - newIndex) * iItemSize);
			memmove(ItemPtr(pArray, newIndex, iItemSize), pTempBuf, iItemSize);
		}
	}
	
	free(pTempBuf);
}

void shellSort(void *pArray, size_t iLen, size_t iItemSize, comparer fComparer)
{
	size_t d = iLen / 2;
	if (d > 1)
	{
		void *pTempBuf = malloc(iItemSize);

		do
		{
			for (size_t i = d; i < iLen; i++)
			{
				int cmp = fComparer(ItemPtr(pArray, i - d, iItemSize), ItemPtr(pArray, i, iItemSize));
				if (cmp > 0)
				{
					memmove(pTempBuf, ItemPtr(pArray, i, iItemSize), iItemSize);
					memmove(ItemPtr(pArray, i, iItemSize), ItemPtr(pArray, i - d, iItemSize), iItemSize);
					memmove(ItemPtr(pArray, i - d, iItemSize), pTempBuf, iItemSize);
				}
			}

			d = (d * 5) / 6;
		} while (d > 1);

		free(pTempBuf);
	}

	insertionSort(pArray, iLen, iItemSize, fComparer);
}
