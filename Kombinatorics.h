#pragma once
#include <vector>

template <typename T>
extern inline size_t factorial (T n) // факториал
{
	size_t ans = 1;

	for (T i = 2; i <= n; i++)
		ans *= i;

	return ans;
}

template <typename T>
extern inline size_t C (T n, T k) // Сочетания
{
	return factorial(n) / factorial(k) / factorial(n - k);
}

template <typename T>
extern inline size_t A (T n, T k) // Размещения
{
	return factorial(n) / factorial(n - k);
}
