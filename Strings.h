#pragma once

#include <string>
#include <vector>

/**
Префиксная функция
*/

std::vector<int> pref (std::string& s)
{
	std::vector<int> p(s.size(), 0);
	size_t cur = 0;
	
	for (size_t i = 0; i < p.size(); i++)
	{
		BREAK:
		cur = p[i - 1];

		while (s[cur] != s[i])
		{
			if (cur == 0)
			{
				p[i] = 0;
				i++;

				if (i == p.size()) 
					return p;
				else 
					goto BREAK;
			}

			cur = p[cur - 1];
		}

		p[i] = cur + 1;
	}

	return p;
}

// Z-функция

std::vector<size_t> z_func(const std::string& str)
{
	const size_t len = str.size();
	std::vector<size_t> z(len);

	if (0 == len)
		return z;

	z[0] = len;

	for (size_t cur = 1, left = 0, right = 1; cur < len; ++cur)
	{
		if (cur >= right)
		{
			size_t off = 0;
			while (cur + off < len && str[cur + off] == str[off])
				++off;

			z[cur] = off;
			right = cur + z[cur];
			left = cur;
		}

		else
		{
			const size_t equiv = cur - left;

			if (z[equiv] < right - cur)
				z[cur] = z[equiv];

			else
			{
				size_t off = 0;
				while (right + off < len && str[right - cur + off] == str[right + off])
					++off;
				z[cur] = right - cur + off;
				right += off;
				left = cur;
			}
		}
	}

	return z;
}
