#pragma once
#include <vector>
#include <stdexcept>
#include <string>
#include <cstdlib>
#include <cstdio>

/**
Хрень для нахождения максимума (минимума) от i до q.
Тупо делай следующее:

SegmentTree<int> t(vec, MAX (или MIN)); // твой вектор
vec[t.rmq(i, q)]; // макс / мин на промежутке от i до q
*/

/**
Перечисление для выбора
максимума / минимума
*/

enum STOrder
{
	MAX,
	MIN,
};

template <typename T> class SegmentTree
{
private:
	size_t last_; // размер
	std::vector<T> builder_; // то, где поддерживаем rmq
	std::vector<T> constructor_; // первоначальный вектор
	STOrder order_; // выбор поиска (минимум или максимум)

	static constexpr T left(const T _p) { return _p << 1; } // левая ветвь
	static constexpr T right(const T _p) { return (_p << 1) + 1; }  // правая ветвь

	/**
	Построение дерева отрезков (сегментов).
	@param _p - текущее значение,
	@param _l - текущий индекс влево,
	@param _r - текущий индекс вправо
	*/

	void build(const T _p, const size_t _l, const size_t _r)
	{
		if (_l == _r)
			builder_[_p] = _l; // дошли до листа
		else
		{
			build(left(_p), _l, (_l + _r) >> 1);        // строим левую ветвь
			build(right(_p), ((_l + _r) >> 1) + 1, _r);   // строим правую ветвь

			// считаем rmq для права и лева

			const T p1 = builder_[left(_p)];
			const T p2 = builder_[right(_p)];

			// выбираем лучший результат

			if (order_ == MAX)
				builder_[_p] = (constructor_[p1] >= constructor_[p2] ? p1 : p2);
			else
				builder_[_p] = (constructor_[p1] <= constructor_[p2] ? p1 : p2);
		}
	}

	/**
	Нахождение rmq (range maximum query).
	@param _p - текущее значение,
	@param _l - текущий индекс влево,
	@param _r - текущий индекс вправо,
	@return rmq(_i, _j) в ЧАСТИ дерева
	*/

	T rmq(const T _p, const size_t _l, const size_t _r, const size_t _i, const size_t _j)
	{
		if (_i > _r || _j < _l)  // если вышли за границу
			return -1;

		else if (_l >= _i && _r <= _j) // если попали
			return builder_[_p];

		// считаем rmq справа и слева

		const T p1 = rmq(left(_p), _l, (_l + _r) >> 1, _i, _j);
		const T p2 = rmq(right(_p), ((_l + _r) >> 1) + 1, _r, _i, _j);

		if (p1 == -1) return p2;
		if (p2 == -1) return p1;

		// выбираем лучший результат

		if (order_ == MAX)
			return (constructor_[p1] >= constructor_[p2] ? p1 : p2);
		else
			return (constructor_[p1] <= constructor_[p2] ? p1 : p2);
	}

public:

	/**
	Конструктор для дерева отрезков.
	@param _vec - вектор, для которого нужно найти rmq
	*/

	constexpr SegmentTree(const std::vector<T>& _vec, const STOrder _order)
	{
		constructor_ = _vec;
		last_ = _vec.size();
		order_ = _order;

		builder_.assign(last_ << 2, 0);
		build(1, 0, last_ - 1);
	}

	/**
	Перестройка дерева.
	@param _vec - новый вектор
	*/

	constexpr void rebuild(const std::vector<T>& _vec)
	{
		constructor_ = _vec;
		build(1, 0, last_ - 1);
	}

	constexpr void rebuild(const std::vector<T>& _vec, const STOrder _order)
	{
		constructor_ = _vec;
		last_ = _vec.size();
		order_ = _order;

		builder_.assign(last_ << 2, 0);
		build(1, 0, last_ - 1);
	}

	// перегрузка для нахождения
	// rmq(_i, _j) для всего дерева

	constexpr T rmq(const size_t _i, const size_t _q)
	{
		if (_q >= constructor_.size())
			throw std::out_of_range("Second index out of range");
		if (_i > _q)
			throw std::range_error("First index > Second index");
		return rmq(1, 0, last_ - 1, _i, _q);
	}
};

/**
Хрень для нахождения суммы от i до q.
ДЕЛАТЬ ВОТ ТАК (С МУВОМ, ЕСЛИ ВЕКТОР БОЛЬШЕ НЕ НУЖЕН):

std::vector<int> vec2(std::move(pref_array(vec1));
vec2[i + 1] - vec2[q] // сумма от включительно q до i включительно (считаем от 0)
*/

template <typename T>
constexpr std::vector<int64_t> sum_arr(const std::vector<T>& arr)
{
	std::vector<int64_t> ans(arr.size() + 1);
	ans[0] = 0;
	ans[1] = arr[0];

	for (size_t i = 2; i <= arr.size(); i++)
		ans[i] = ans[i - 1] + arr[i - 1];

	return ans;
}

/**
Суффиксный массив.
Работает так:

std::string str = "ABCDABCD"
auto vec = std::move(suf_array(str));

assert(vec, ["ABCDABCD", "BCDABCD", "CDABCD", "DABCD", ...]

*/

template <typename T>
constexpr std::vector<std::string> suf_array_str(const std::string& str)
{
	std::vector<std::string> suf_arr(str.size());

	for (size_t i = 0; i < str.size(); i++)
		for (size_t q = 0; q <= i; q++)
			suf_arr[q].push_back(str[i]);

	return suf_arr;
}

/**
Преффиксный массив.
Работает так:

std::string str = "ABCDABCD"
auto vec = std::move(suf_array(str));

assert(vec, ["ABCDABCD", "ABCDABC", "ABCDAB", "ABCDA", ...]

*/

template <typename T>
constexpr std::vector<std::string> pref_array_str(const std::string& str)
{
	const size_t siz = str.size();
	std::vector<std::string> suf_arr(siz);

	for (size_t i = 0; i < str.size(); i++)
		for (size_t q = 0; q < siz - i; q++)
			suf_arr[q].push_back(str[i]);

	return suf_arr;
}

class UFDS // Union Search Disjoint Set (Система Непересекающихся Множеств)
{
	int* p_;        	// родители
	int* rank_;		// ранг (фиктивен)
	int* children_;		// кол-во детей + он сам
	int size_;		// размер
	int amount_of_sets_; 	// кол-во множеств

	void index_out_of_range(const int i) const
	{
		if (i >= size_)
		{
			std::fprintf(stderr, "Index out of range: var = %d size = %d\n", i, size_);
			std::free(p_);
			std::free(rank_);
			std::free(children_);
			std::abort();
		}
	}

public:
	explicit UFDS(const int n) noexcept
	{
		size_			= n;
		amount_of_sets_ 	= n;
		p_			= static_cast<int*>(std::malloc(n * sizeof(int)));
		rank_			= static_cast<int*>(std::calloc(n, sizeof(int)));
		children_		= static_cast<int*>(std::malloc(n * sizeof(int)));
		
		for (int* i = children_; i != children_ + size_; i++)
			*i = 1;

		for (int i = 0; i < n; i++)
			p_[i] = i;
	}
	
	~UFDS() noexcept
	{
		std::free(p_);
		std::free(rank_);
		std::free(children_);
	}

	[[nodiscard]] int unsafe_findSet(const int i) { return p_[i] == i ? i : p_[i] = unsafe_findSet(p_[i]); }
	[[nodiscard]] int findSet(const int i)
	{
		index_out_of_range(i);
		return unsafe_findSet(i);
	}
	
	[[nodiscard]] bool unsafe_isSameSet(const int i, const int j) { return findSet(i) == findSet(j); }
	[[nodiscard]] bool isSameSet(const int i, const int j)
	{
		index_out_of_range(i);
		index_out_of_range(j);
		return findSet(i) == findSet(j);
	}

	UFDS* unsafe_unionSet(const int i, const int j)
	{
		if (!isSameSet(i, j))
		{
			amount_of_sets_--;
			const int x = unsafe_findSet(i);
			const int y = unsafe_findSet(j);

			if (rank_[x] > rank_[y])
				p_[y] = x, children_[x] += children_[y];

			else
			{
				p_[x] = y;
				children_[y] += children_[x];
				
				if (rank_[x] == rank_[y])
					rank_[y]++;
			}
		}

		return this;
	}

	UFDS* unionSet(const int i, const int j)
	{
		index_out_of_range(i);
		index_out_of_range(j);
		return unsafe_unionSet(i, j);
	}

	[[nodiscard]] int numDisjointSets() noexcept { return amount_of_sets_; }

	[[nodiscard]] int unsafe_sizeOfSet(const int i) { return children_[unsafe_findSet(i)]; }
	[[nodiscard]] int sizeOfSet(const int i)
	{
		index_out_of_range(i);
		return unsafe_sizeOfSet(i);
	}
};
