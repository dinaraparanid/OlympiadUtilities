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
