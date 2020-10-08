#include <cstdio>
#include "NumTheory.h"

int main ()
{
	auto ans = eratosfen(60);

	for (auto elem : ans)
		printf("%d ", elem);

	return 0;
}
