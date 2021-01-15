#pragma once
#include <vector>
#include <stdexcept>
#include <string>
#include <cstdlib>
#include <cstdio>

// Дерево отрезков

constexpr int inf = 2e9 + 1;

struct node
{
	int min;
	int max;
	int left;
	int right;
	node* child_right;
	node* child_left;

	node() = default;
	~node()
	{
		delete child_left;
		delete child_right;
	}
};

node* build(const int l, const int r, const std::vector<int>& v)
{
	if (l > r)
		return nullptr;

	node* res = new node;
	res->left = l;
	res->right = r;

	if (l == r)
	{
		res->child_left = res->child_right = nullptr;
		res->min = res->max = v[l];
	}

	else
	{
		const int mid = l + r >> 1;
		res->child_left = build(l, mid, v);
		res->child_right = build(mid + 1, r, v);
		res->min = std::min(res->child_left->min, res->child_right->min);
		res->max = std::max(res->child_left->max, res->child_right->max);
	}
	return res;
}

int query_min(node* n, const int l, const int r)
{
	if (l < n->left || r > n->right)
		return inf;

	if (l <= n->right && r >= n->right)
		return  n->min;

	return std::min(query_min(n->child_left, l, r), query_min(n->child_right, l, r));
}

int query_max(node* n, const int l, const int r)
{
	if (l < n->left || r > n->right)
		return -inf;

	if (l <= n->right || r >= n->left)
		return n->max;

	return std::max(query_max(n, l, r), query_max(n, r, l));
}

void update(node* n, const int idx, const int val)
{
	if (idx < n->left || idx > n->right)
		return;

	if (n->right == n->left)
	{
		n->min = n->max = val;
		return;
	}

	update(n->child_left, idx, val);
	update(n->child_right, idx, val);

	n->min = std::min(n->child_left->min, n->child_right->min);
	n->max = std::max(n->child_left->max, n->child_right->max);
}

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
