/*
%%%

title: Nim Multiplication
author: Roundgod

%%%
*/

#include <bits/stdc++.h>
using namespace std;

// +++
int nim_mult_pow(int x, int y) {
    if (x < 2) return x & y;
    int m = 1;
    while (x >= 1 << (m << 1)) m <<= 1;
    int p = x >> m, s = y >> m, t = y & ((1 << m) - 1);
    int d1 = nim_mult_pow(p, s);
    int d2 = nim_mult_pow(p, t);
    return ((d1 ^ d2) << m) ^ nim_mult_pow(1 << (m - 1), d1);
}

int nim_mult(int x, int y) {
    if (x < y) swap(x, y);
    if (x < 2) return x & y;
    int m = 1;
    while (x >= 1 << (m << 1)) m <<= 1;
    int p = x >> m, q = x & ((1 << m) - 1);
    int s = y >> m, t = y & ((1 << m) - 1);
    int c1 = nim_mult(p, s);
    int c2 = nim_mult(p, t) ^ nim_mult(q, s);
    int c3 = nim_mult(q, t);
    return ((c1 ^ c2) << m) ^ c3 ^ nim_mult_pow(1 << (m - 1), c1);
}
// +++

int main() {
    assert(nim_mult(0, 0) == 0);
    assert(nim_mult(0, 3) == 0);
    assert(nim_mult(0, 5) == 0);
    assert(nim_mult(1, 1) == 1);
    assert(nim_mult(4, 7) == 10);
    assert(nim_mult(9, 6) == 1);
    assert(nim_mult(4, 8) == 11);
}
