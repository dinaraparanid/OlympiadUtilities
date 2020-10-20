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
	std::printf("%d", bin_search<int>(0, 10, [](const int a) {
	  if (a > 3) return 1;
	  if (a < 3) return -1;
	  return 0;
	}));
	return 0;
}
*/

template <typename T> constexpr T bin_search(const T a, const T b, std::function<int(const T)> fn, T cnst = 1)
{
	T start = a, finish = b;

	while (finish - start > cnst)
	{
		const T middle = (start + finish) / 2;

		switch (fn(middle))
		{
		case 1:
			finish = middle;
			break;
		case -1:
			start = middle;
			break;
		default:
			return middle;
		}
	}

	return start;
}
