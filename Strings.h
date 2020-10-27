#pragma once
#include <string>
#include <vector>

extern inline std::vector<size_t> prefix_function(const std::string& s)
{
	const size_t size = s.length();
	std::vector<size_t> pi(size);
	
	for (size_t i = 1; i < size; ++i) 
	{
		int j = pi[i - 1];
		
		while (j > 0 && s[i] != s[j])
			j = pi[j - 1];
		
		if (s[i] == s[j])
			++j;
		
		pi[i] = j;
	}
	
	return pi;
}

extern inline std::vector<size_t> z_function(const std::string& s)
{
	const size_t size = s.length();
	std::vector<size_t> z(size);
	
	for (size_t i = 1, l = 0, r = 0; i < size; i++) 
	{
		if (i <= r)
			z[i] = std::min(r - i + 1, z[i - l]);
		
		while (i + z[i] < size && s[z[i]] == s[i + z[i]])
			z[i]++;
		
		if (i + z[i] - 1 > r)
			l = i, r = i + z[i] - 1;
	}
	
	return z;
}

extern inline std::pair<std::vector<size_t>, std::vector<size_t>> manaker(const std::string& s)
{
	const size_t size = s.length();
	std::vector<size_t> d1(size); // палиндромы нечётной длины 
	
	size_t l = 0, r = -1;
	
	for (size_t i = 0; i < size; ++i)
	{
		size_t k = i > r ? 1 : std::min(d1[l + r - i], r - i + 1);
		
		while (i + k < size && i - k >= 0 && s[i + k] == s[i - k])  ++k;
		
		d1[i] = k;
		
		if (i + k - 1 > r)
			l = i - k + 1, r = i + k - 1;
	}

	std::vector<size_t> d2(size); // палиндромы чётной длины 
	
	l = 0, r = -1;
	
	for (size_t i = 0; i < size; ++i)
	{
		size_t k = i > r ? 0 : std::min(d2[l + r - i + 1], r - i + 1);
		
		while (i + k < size && i - k - 1 >= 0 && s[i + k] == s[i - k - 1])  ++k;
		
		d2[i] = k;
		
		if (i + k - 1 > r)
			l = i - k, r = i + k - 1;
	}

	return std::make_pair(d1, d2);
}
