#ifndef _NUMTHEORY_H_
#define _NUMTHEORY_H_

#include <algorithm>
#include <unordered_map>
#include <set>
#include <map>

#define HASH_MAP std::unordered_map

template <typename T>
extern inline T lcm(T a, T b)  // НОК
{
	return a * b / std::__gcd(a, b);
}

template <typename T>
extern inline T mod(T a, T b) // Математический остаток (> 0)
{
	return ((a % b + b) % b);
}

template <typename T>
extern inline T gcd_ext(T a, T b, T& x, T& y) // расширенный алг. Эвклида (для диофантовых ур.)
{
	if (b == 0)
	{
		x = 1;
		y = 0;
		return a;
	}

	const T d = gcd_ext(b, a % b, x, y);
	x -= (a / b) * y;

	std::swap(x, y);
	return d;
}

template <typename T>
extern inline T binpow (T a, T n) // быстрое возведение в степень O(log(n))
{
	T res = 1;

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
extern inline std::set<T> del(T a) // делители числа
{
	std::set<T> ans;

	for (T i = 1; i * i <= a; i++)
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
extern inline HASH_MAP<T, int> st_del(T a) // разложение числа на простые
{
	HASH_MAP<T, int> ans;
	const T remember = a;

	T i = 2;

	while (a != 1 && i * i <= remember && i <= a)
	{
		while (a % i == 0)
			ans[i]++, a /= i;

		i++;
	}

	if (a != 1)
		ans[a] = 1;

	return ans;
}

template <typename T>
extern inline std::map<T, int> st_del_sort(T a) // разложение числа на простые (сортир)
{
	std::map<T, int> ans;
	const T remember = a;

	T i = 2;

	while (a != 1 && i * i <= remember && i <= a)
	{
		while (a % i == 0)
			ans[i]++, a /= i;

		i++;
	}

	if (a != 1)
		ans[a] = 1;

	return ans;
}

template <typename T>
extern inline size_t amount_of_del(T a) // кол-во делителей
{
	HASH_MAP<T, int> del(std::move(st_del(a)));

	size_t ans = 1;

	for (auto it : del)
		ans *= (it.second + 1);

	return ans;
}

template <typename T>
extern inline size_t sum_del(T a) // сумма делителей
{
	HASH_MAP<T, int> del(std::move(st_del(a)));

	size_t ans = 1;

	for (auto it : del)
	{
		size_t res = 1 + it.first;

		for (int i = 2; i <= it.second; i++)
			res += binpow(it.first, i);

		ans *= res;
	}

	return ans;
}

template <typename T>
extern inline std::set<T> eratosfen(T a) // все простые числа от 1 до a
{
	std::set<T> ans;

	for (T i = 2; i <= a; i++)
		ans.insert(i);

	for (T i = 2; i <= a; i++)
	{
		if (ans.find(i) == ans.end())
			continue;

		for (T q = i * i; q <= a; q += i)
			ans.erase(q);
	}

	return ans;
}

/**
 Функция Эйлера равна кол-ву чисел от 1 до n, которые gcd(i, n) == 1
 Теорема Эйлера: a^q(m) % m == 1 (q(m) - ф. Эйлера)
 Малая Теорема Ферма: a^(p - 1) % p == 1 (где p - простое)
 */
 
template <typename T>
extern inline T phi (T n)
{
	T res = n;

	for (T p = 2; p * p <= n; p++)
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

#endif //_NUMTHEORY_H_
