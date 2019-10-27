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
/*
%%%
title: Segment Tree with Dynamic Node Allocation
usage:
    init(l, r): initialize the tree in range $[l, r)$
    add(l, r, v): add $v$ to range $[l, r)$
    sum(l, r): return the sum in range $[l, r)$
    
%%%
*/
// +++
const int MAXN = (1 << 17) * 20;
int lbnd, rbnd, sz;
int lson[MAXN], rson[MAXN];
LL val[MAXN], tag[MAXN];

inline int alloc() {
    lson[sz] = rson[sz] = val[sz] = tag[sz] = 0;
    return sz++;
}

#define lson (lson[o] = lson[o] ?: alloc())
#define rson (rson[o] = rson[o] ?: alloc())

void init(int l, int r) {
    lbnd = l; rbnd = r; sz = 2;
}

void pull(int o) {
    val[o] = val[lson] + val[rson];
}

void fadd(LL x, int o, int ll, int rr) {
    val[o] += x * (rr - ll);
    tag[o] += x;
}

void push(int o, int ll, int rr) {
    int mm = (ll + rr) / 2;
    fadd(tag[o], lson, ll, mm);
    fadd(tag[o], rson, mm, rr);
    tag[o] = 0;
}

void add(int l, int r, LL v, int o = 1, int ll = lbnd, int rr = rbnd) {
    if (l <= ll and rr <= r) return fadd(v, o, ll, rr);
    int mm = (ll + rr) / 2;
    push(o, ll, rr);
    if (mm > l) add(l, r, v, lson, ll, mm);
    if (mm < r) add(l, r, v, rson, mm, rr);
    pull(o);
}

LL sum(int l, int r, int o = 1, int ll = lbnd, int rr = rbnd) {
    if (l <= ll and rr <= r) return val[o];
    int mm = (ll + rr) / 2;
    push(o, ll, rr);
    if (r <= mm) return sum(l, r, lson, ll, mm);
    if (l >= mm) return sum(l, r, rson, mm, rr);
    return sum(l, r, lson, ll, mm) + sum(l, r, rson, mm, rr);
}

// +++

int main() {
    init(1, 6);
    add(1, 2, 1);
    add(2, 3, 5);
    add(3, 4, 4);
    add(4, 5, 2);
    add(5, 6, 3);
    assert(sum(2, 5) == 11);
    add(2, 4, 2);
    assert(sum(3, 5) == 8);
    add(1, 6, 1);
    assert(sum(1, 5) == 20);
    return 0;
}
