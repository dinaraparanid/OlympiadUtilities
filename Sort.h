#pragma once

#include <vector>
#include <map>

// Для qsort() из C

template <typename T>
extern inline int qsortCmp (const void* a, const void* b)
{
	const T arg1 = *(const T*)a;
	const T arg2 = *(const T*)b;

	if (arg1 < arg2) return -1;
	if (arg1 > arg2) return 1;
	return 0;
}

// Для map и set (деревянных)

template <typename T> class Cmp
{
 public:
	__forceinline bool operator() (T a, T b) const
	{
		return false;
		// код для сравнения
	}
};

// Сортировка подсчётом

template <typename T>
extern inline void countSort(std::vector<T>& arr)
{
	std::map<T, size_t> cnt;

	for (auto i : arr)
		cnt[i]++;

	size_t ind = 0;

	for (auto it : cnt)
		while (it.second--)
			arr[ind++] = it.first;
}
