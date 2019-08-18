#include <bits/stdc++.h>
#define MAXN 5030
using namespace std;
#define Rep(i, n) for (int i = 1; i <= int(n); i++)

/*
%%%

title: Bipartite adjraph
desc:  
    Maximum matching, minimum vertex cover and maximum independent set for
    bipartite graph. All indices are 1-based. \par
    Set \lstinline|n1, n2| and \lstinline|adj[i]|, then call \lstinline|matching()| to get the size of the maximum matching.
usage:
    n1, n2: the size of left part and right part.
    adj[i]: the neighborhood of left vertex $i$.
    dx[i], dy[i]: the right/left matched index of left/right vertex $i$, or 0 if the vertex is not matched.

%%%
*/

// +++
int n1, n2;
vector<int> adj[MAXN];
int mx[MAXN], my[MAXN];
int dx[MAXN], dy[MAXN];
bool vis[MAXN];

bool vx[MAXN], vy[MAXN];

void mark(int u) {
    if (vx[u]) return;
    vx[u] = true;
    for (int v : adj[u]) if (v != mx[u]) {
        vy[v] = true;
        mark(my[v]);
    }
}

bool find(int u) {
    for (int v : adj[u]) {
        if (!vis[v] && dy[v] == dx[u] + 1) {
            vis[v] = true;
            if (!my[v] or find(my[v])) {
                mx[u] = v;
                my[v] = u;
                return true;
            }
        }
    }
    return false;
}

int matching() {
    queue<int> q;
    memset(mx, 0, sizeof(mx));
    memset(my, 0, sizeof(my));
    int ans = 0;
    while (true) {
        bool flag = false;
        while (q.size()) q.pop();
        memset(dx, 0, sizeof(dx));
        memset(dy, 0, sizeof(dy));
        Rep (i, n1) if (!mx[i]) q.push(i);
        while (q.size()) {
            int u = q.front(); q.pop();
            for (int v : adj[u]) if (!dy[v]) {
                dy[v] = dx[u] + 1;
                if (my[v]) {
                    dx[my[v] = dy[v]] + 1;
                    q.push(my[v]);
                } else flag = true;
            }
        }
        if (!flag) break;
        memset(vis, 0, sizeof(vis));
        Rep (i, n1) if (!mx[i] && find(i)) ans++;
    }
    return ans;
}

//+++

int main() {
    n1 = 3, n2 = 2;
    adj[1] = {1, 2};
    adj[2] = {2};
    assert(matching() == 2);
}
