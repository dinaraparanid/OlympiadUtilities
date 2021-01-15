#include <cstdio>
#include <vector>
#include <set>

constexpr int inf = 2e9 + 1;

int main()
{
    int n = 0;
    std::scanf("%d", &n);
	
    std::vector<std::vector<std::pair<int, int>>> gr(n);
    std::vector<int> dist(n, inf);
    std::vector<int> pred(n, -1);
    std::set<std::pair<int, int>> s; // dist, v

	for (int i = 0; i < n; i++)
	{
        int x = 0, y = 0, wt = 0;
        std::scanf("%d%d%d", &x, &y, &wt), x--, y--;

        gr[x].push_back({ y, wt });
		// gr[y].push_back(x); - для неориетированного графа
	}

    int start = 0, finish = 0;
    std::scanf("%d%d", &start, &finish), start--, finish--;

    s.insert({0, start});
    dist[start] = 0;

	while (!s.empty())
	{
        const int v = s.begin()->second;
        s.erase(s.begin());

		for (auto p : gr[v])
		{
            const int u = p.first;
            const int wt = p.second;

			if (dist[v] + wt < dist[u])
			{
                s.erase({ dist[u], u });
                dist[u] = dist[v] + wt;
                s.insert({ dist[u], u });
                pred[u] = v;
			}
		}
	}

    std::vector<int> ans;
    ans.push_back(finish);
	
    int prev = pred[finish];

	while (prev != -1)
	{
        ans.push_back(prev);
        prev = pred[prev];
	}

    std::reverse(ans.begin(), ans.end());

    for (int i : ans)
        std::printf("%d ", i);

    return 0;
}
