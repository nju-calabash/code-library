/*
%%%

title: Basic Geometry Template
author: sy_chen

%%%
*/
#pragma GCC optimize("fast-math")
#include <bits/stdc++.h>
using namespace std;

// +++
typedef double T;
typedef complex<T> pt;
inline T operator , (pt a, pt b) 
    { return real(a) * real(b) + imag(a) * imag(b); }
inline T operator * (pt a, pt b) 
    { return real(a) * imag(b) - imag(a) * real(b); }

struct polygon : vector<pt> {
    pt& operator [] (int id) { 
        if (id < -size() and id >= size()) id %= (int)size(); 
        if (id < 0) id += (int)size();
        return vector::operator [](id); 
    }
};
// +++

void class_test() {
    pt x; x = polar(1.0, 0.0);
    assert(arg(x) == 0);
    x = 0;
}

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
    class_test();
    point_test();
    polygon_test();
    return 0;
}
