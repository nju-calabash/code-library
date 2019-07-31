#include <bits/stdc++.h>
using namespace std;

/***
title: Basic Geometry Template
 ***/

// +++
using T = double;
typedef struct pt : complex<T> {
    pt(T x = 0, T y = 0) : complex<T>(x, y) {}
    T operator , (pt a) { return real() * a.real() + imag() * a.imag(); }
    T operator * (pt a) { return real() * a.imag() - imag() * a.real(); }
} vec;

struct polygon : vector<pt> {
    pt& operator [] (int id) { 
        id %= (int)size(); if (id < 0) id += (int)size();
        return vector<pt>::operator [](id); 
    }
};
// +++

void point_test() {
    assert(pt(1, 2) + pt(2, 3) == pt(3, 5));
    assert(pt(2, 4) - pt(1, 6) == pt(1, -2));
    pt x(1, 2); 
    x += pt(2, 4); assert(x == pt(3, 6));
    x *= 2; assert(x == pt(6, 12));
    x /= 2; assert(x == pt(3, 6));
    x -= pt(3, 4); assert(x == pt(0, 2));
    assert((pt(1, 2), pt(2, 3)) == 8);
    assert((pt(1, 2) * pt(-5, 3)) == 13);
}

void polygon_test() {
    polygon a;
    a.emplace_back(1, 2);
    a.emplace_back(2, 3);
    a.emplace_back(3, 4);
    assert((a == vector<pt>{{1, 2}, {2, 3}, {3, 4}}));
    assert(a[-1] == pt(3, 4));
    assert(a[1] == pt(2, 3));
    assert(a[3] == pt(1, 2));
}

int main() {
    point_test();
    polygon_test();
    return 0;
}
