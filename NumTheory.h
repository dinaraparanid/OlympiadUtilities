#pragma once
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <map>

template <typename T>
constexpr void nums_fast_swap(T& a, T& b) // для чисел использовать это (БЫСТРЕЕ, ЧЕМ std::swap())
{
	a ^= b;
	b ^= a;
	a ^= b;
}

template <typename T>
constexpr uint64_t lcm(const T a, const T b)  // НОК
{
	return a / std::__gcd(a, b) * b;
}

template <typename T>
constexpr T mod(const T a, const T b) // Математический остаток (>= 0)
{
	return ((a % b + b) % b);
}

template <typename T>
constexpr T gcd_ext(const T a, const T b, T& x, T& y) // расширенный алг. Эвклида (для диофантовых ур.)
{
	if (!b)
	{
		x = 1;
		y = 0;
		return a;
	}

	const T d = gcd_ext(b, a % b, x, y);
	x -= (a / b) * y;

	nums_fast_swap(x, y);
	return d;
}

template <typename N, typename S>
constexpr int64_t bin_pow(N a, S n) // быстрое возведение в степень O(log(n))
{                                   // ЮЗАТЬ ЭТО ВМЕСТО (std::pow()), ОНА БЫСТРЕЕ
	if (a == 2) return 1 << n;
	if (a == 1) return 1;
	if (a == 0) return 0;
	if (n == 1) return a;
	if (n == 0) return 1;
	if (a == -1 && n & 1) return -1;
	if (a == -1 && !(n & 1)) return 1;

	int64_t res = 1;

	while (n)
	{
		if (n & 1)
			res *= a;

		a *= a;
		n >>= 1;
	}

	return res;
}

template <typename T>
constexpr std::unordered_set<T> del(const T a) // делители числа (НЕ СОРТИРОВАНЫ)
{
	std::unordered_set<T> ans;

	for (T i = 1; i * i <= a; ++i)
	{
		if (a % i == 0)
		{
			if (i * i == a)
				ans.insert(i);
			else
			{
				ans.insert(i);
				ans.insert(a / i);
			}
		}
	}

	return ans;
}

template <typename T>
constexpr std::set<T> del_sort(const T a) // делители числа (СОРТИРОВАНЫ)
{
	std::set<T> ans;

	for (T i = 1; i * i <= a; ++i)
	{
		if (a % i == 0)
		{
			if (i * i == a)
				ans.insert(i);
			else
			{
				ans.insert(i);
				ans.insert(a / i);
			}
		}
	}

	return ans;
}

template <typename T>
constexpr std::unordered_map<T, int> st_del(T a) // разложение числа на простые (НЕ СОРТИРОВАННЫ)
{
	std::unordered_map<T, int> ans;
	const T remember = a;

	T i = 2;

	while (a != 1 && i * i <= remember && i <= a)
	{
		while (a % i == 0)
			++ans[i], a /= i;

		++i;
	}

	if (a != 1)
		ans[a] = 1;

	return ans;
}

template <typename T>
constexpr std::map<T, int> st_del_sort(T a) // разложение числа на простые (СОРТИРОВАНЫ)
{
	std::map<T, int> ans;
	const T remember = a;

	T i = 2;

	while (a != 1 && i * i <= remember && i <= a)
	{
		while (a % i == 0)
			++ans[i], a /= i;

		++i;
	}

	if (a != 1)
		ans[a] = 1;

	return ans;
}

template <typename T>
constexpr uint64_t amount_of_del(const T a) // кол-во делителей
{
	std::unordered_map<T, int> del(std::move(st_del(a)));

	uint64_t ans = 1;

	for (auto it : del)
		ans *= (it.second + 1);

	return ans;
}

template <typename T>
constexpr uint64_t sum_del(const T a) // сумма делителей
{
	std::unordered_map<T, int> del(std::move(st_del(a)));

	uint64_t ans = 1;

	for (auto it : del)
	{
		uint64_t res = 1 + it.first;

		for (int i = 2; i <= it.second; i++)
			res += binpow(it.first, i);

		ans *= res;
	}

	return ans;
}

template <typename T>
constexpr std::set<T> eratosfen(const T a) // все простые числа от 1 до a включительно
{
	std::set<T> ans;

	for (T i = 2; i <= a; ++i)
		ans.insert(i);

	for (T i = 2; i <= a; ++i)
		if (ans.find(i) != ans.end())
			for (T q = i * i; q <= a; q += i)
				ans.erase(q);

	return ans;
}

/**
 Функция Эйлера равна кол-ву чисел от 1 до n, которые gcd(i, n) == 1
 Теорема Эйлера: a^q(m) % m == 1 (q(m) - ф. Эйлера)
 Малая Теорема Ферма: a^(p - 1) % p == 1 (где p - простое)
 */

template <typename T>
constexpr uint64_t phi(T n)
{
	uint64_t res = n;

	for (T p = 2; p * p <= n; ++p)
	{
		if (n % p == 0)
			res -= res / p;

		while (n % p == 0)
			n /= p;
	}

	if (n > 1)
		res -= res / n;

	return res;
}

template <typename T>
constexpr uint64_t arif_prog_step1(const T a, const T b) // арифметическая прогрессия с шагом 1
{
	return (a + b) * (b - a + 1) / 2;
}

template <typename T>
constexpr int64_t arif_prog(const T a, const uint64_t n, const int64_t mov) // арифметическая прогрессия
{
	return (2ll * a + mov * (n - 1)) * n / 2;
}

template <typename T>
constexpr int64_t geom_prog(const T a, const uint64_t n, const int64_t mov)  // геом. прогрессия
{
	return mov == 1 ? a : a * (bin_pow(mov, n) - 1) / (mov - 1);
}
