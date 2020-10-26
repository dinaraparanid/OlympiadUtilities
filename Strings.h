#pragma once
#include <string>
#include <vector>

extern inline std::vector<size_t> prefix_function(const std::string& s)
{
	const size_t n = s.length();
	std::vector<size_t> pi(n);
	
	for (size_t i = 1; i < n; ++i) 
	{
		int j = pi[i - 1];
		
		while (j > 0 && s[i] != s[j])
			j = pi[j - 1];
		
		if (s[i] == s[j])  ++j;
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
