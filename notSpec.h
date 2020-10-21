#pragma once 
#include <functional>

/** Бинарный поиск
@param a - начало (наименьшая граница)
@param b - конец (наибольшая граница)
@param fn - функция, по которой выполняем бин.поиск
@param cnst - точность поиска (по-умолчанию = 1) 
@return искомый элемент

@example:
// считаем, что всё подключенно

int main()
{
  	// ищем 3 среди множества от 0 до 10
	std::printf("%d", bin_search<int>(0, 10, 3 [](const int a) { return a; }));
	return 0;
}
*/

template <typename T> constexpr T bin_search (const T a, const T b, const T check, std::function<int(const T)> fn, const T cnst = 1)
{
	T start = a, finish = b;

	while (finish - start > cnst)
	{
		const T middle = (start + finish) / 2;
                const T res = fn(middle);

		if (res - check < cnst)
			finish = middle;
		else if (res - check > cnst)
			start = middle;
		else
		        return middle;
	}

	return start;
}
