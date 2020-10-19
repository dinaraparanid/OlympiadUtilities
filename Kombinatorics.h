#pragma once

template <typename T>
constexpr size_t factorial(const T n) // факториал
{
	size_t ans = 1;

	for (T i = 2; i <= n; ++i)
		ans *= i;

	return ans;
}

template <typename T>
constexpr size_t C(const T n, const T k) // Сочетания
{
	return factorial(n) / factorial(k) / factorial(n - k);
}

template <typename T>
constexpr size_t A(const T n, const T k) // Размещения
{
	return factorial(n) / factorial(n - k);
}
