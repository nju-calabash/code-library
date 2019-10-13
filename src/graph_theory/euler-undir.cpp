
/*
%%%

title: Eulerian Cycle (Undirected)
desc:
    Find an Eulerian cycle for undirected graph.
usage:
    n, m: the number of vertices and edges
    add_edge(u, v, id): add an edge with label id
    euler(): try find an Eulerian cycle and return true if found
%%%
*/
// passed https://loj.ac/problem/10105
#include <bits/stdc++.h>
using namespace std;

#define rep(i, n) for (int i = 0; i < int(n); i++)
#define Rep(i, n) for (int i = 1; i <=int(n); i++)
#define range(x) begin(x), end(x)
#ifdef __LOCAL_DEBUG__
#define _debug(fmt, ...) fprintf(stderr, "[%s] " fmt "\n", __func__, ##__VA_ARGS__)
#else
#define _debug(...) ((void) 0)
#endif
typedef long long LL;
typedef unsigned long long ULL;

// +++
const int MAXM = 200005, MAXN = 100005;
int n, m;
int to[MAXM * 2];
bool vis[MAXM * 2];
vector<int> inc[MAXN];
int ptr[MAXN];
vector<int> path;

void add_edge(int u, int v, int id) {
    id <<= 1; to[id] = v; inc[u].push_back(id);
    id++;     to[id] = u; inc[v].push_back(id);
}

void dfs(int u) {
    for (; ptr[u] < inc[u].size(); ptr[u]++) {
        int id = inc[u][ptr[u]];
        if (!vis[id]) {
            vis[id] = vis[id ^ 1] = true;
            dfs(to[id]);
            path.push_back(id & 1 ? id / 2: -id / 2);
        }
    }
}

bool euler() {
    Rep (i, n) if (inc[i].size() & 1) return false;
    // To find Eulerian path, modify the following line.
    Rep (i, n) if (inc[i].size()) { dfs(i); break; }
    return path.size() == m;
}
// +++

int main() {
    n = m = 3;
    add_edge(1, 1, 1); add_edge(1, 2, 2); add_edge(1, 2, 3);
    assert(euler() == true);
    path.clear();
    m++; add_edge(2, 3, 4);
    assert(euler() == false);
    return 0;
}

