#pragma once
#include <map>
#include <vector>

// Для qsort() из C

template <typename T>
constexpr int qsort_cmp(const void* a, const void* b)
{
	const T arg1 = *static_cast<const T*>(a);
	const T arg2 = *static_cast<const T*>(b);

	if (arg1 < arg2) return -1;
	if (arg1 > arg2) return 1;
	return 0;
}

// Для map и set (деревянных)

template <typename T> class Cmp
{
public:
	constexpr bool operator() (const T a, const T b) const
	{
		return false;
		// код для сравнения
	}
};

// Сортировка подсчётом

template <typename T>
constexpr void count_sort(std::vector<T>& arr)
{
	std::map<T, size_t> cnt;

	for (auto i : arr)
		++cnt[i];

	size_t ind = 0;

	for (auto it : cnt)
		while (it.second--)
			arr[ind++] = it.first;
}
