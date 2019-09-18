/*
%%%

title: Basic Geometry Template
author: sy_chen

%%%
*/
#pragma GCC optimize("fast-math")
#include <bits/stdc++.h>
using namespace std;

#define rep(i, n) for (int i = 0; i < int(n); i++)
#define Rep(i, n) for (int i = 1; i <=int(n); i++)
#define range(x) begin(x), end(x)

// +++
typedef double T;
const double EPS = 1e-9;

inline int fcmp(double x, double y = 0.0) {
    if (fabs(x - y) <= EPS) return 0;
    return x < y ? -1 : 1;
}

// Point

typedef complex<T> pt, vec;
inline pt operator ^ (pt a, pt b) { return a * b; }
inline T operator , (pt a, pt b) 
    { return real(a) * real(b) + imag(a) * imag(b); }
inline T operator * (pt a, pt b) 
    { return real(a) * imag(b) - imag(a) * real(b); }
inline bool operator == (pt a, pt b) { return abs(a - b) < EPS; }

// >0: in order, <0: out of order, =0: nonstandard
inline int rotOrder(vec a, vec b, vec c) {
    return fcmp(double(a*b) * (b*c));
}

pt unit(pt x) { return x / abs(x); }

// Segment

typedef pair<pt, pt> seg;

inline bool operator & (seg s, pt p) { // pt in seg
    vec v1 = s.first - p, v2 = s.second - p;
    return (v1, v2) <= 0 and v1 * v2 == 0;
}

T distance(pt p, seg S) {
    pt s, t; tie(s, t) = S;
    if ((p - s, t - s) < EPS) return abs(p - s);
    if ((p - t, s - t) < EPS) return abs(p - t);
    return abs((s - p) * (t - p)) / abs(s - t);
}

T distance(seg S, seg T) {
    return min({distance(S.first, T), distance(S.second, T),
        distance(T.first, S), distance(T.second, S)});
}

inline bool nIntRectRect(seg a, seg b) {
    rep (i, 2) {
        if (min(real(a.first), real(a.second)) < 
            max(real(b.first), real(b.second))) return false;
        if (min(imag(a.first), imag(a.second)) < 
            max(imag(b.first), imag(b.second))) return false;
        swap(a, b);
    }
    return true;
}

inline bool operator & (seg a, seg b) { // seg seg intersection
    if (!nIntRectRect(a, b)) return false;
    return rotOrder(b.first-a.first, a.second-a.first, b.second-a.first) >= 0 && 
           rotOrder(a.first-b.first, b.second-b.first, a.second-b.first) >= 0;
}

inline pt getIntersection(pt P, vec v, pt Q, vec w) { // line line intersection
    return P + v * (w * (P - Q) / (v * w));
}

inline pt project(pt p, seg S) { // project p to line s
    pt s, t; tie(s, t) = S;
    double r = (t - s, p - s) / norm(t - s);
    return s + r * (t - s);
}

// Polygon

struct polygon : vector<pt> { 
    pt& get(int id) {
        while (id < 0) id += size();
        while (id >= size()) id -= size();
        return at(id);
    }

    pt& operator [] (int id) { return get(id); }

    seg getseg(int id) {    // {pts[id], pts[id+1]}
        return {get(id), get(id+1)};
    }

    //   Make convex hull from a set of points.
    //   The result starts from the point with minimum x (and minimum y if 
    // multiple) and is in counterclockwise order. If you want non-strict 
    // convex hull, change all <= into <.
    void make_hull() {
        sort(begin(), end(), [] (pt a, pt b) {
            return make_pair(real(a), imag(a)) < make_pair(real(b), imag(b));
        });
        int k = 0;
        vector<pt> h(size() * 2);
        for (int i = 0; i < size(); i++) {
            while (k > 1 and (h[k-1]-h[k-2]) * (at(i)-h[k-1]) <= 0) k--;
            h[k++] = at(i);
        }
        for (int i = size() - 2, t = k; i >= 0; i--) {
            while (k > t and (h[k-1]-h[k-2]) * (at(i)-h[k-1]) <= 0) k--;
            h[k++] = at(i);
        }
        h.resize(k-1); vector<pt>::operator = (h);
    }
    
    bool operator & (pt p) {
        int wn = 0;
        rep (i, size()) {
            if (make_pair(get(i), get(i+1)) & p) return true; // on the border
            int d1 = fcmp(imag(get(i)), imag(p));
            int d2 = fcmp(imag(get(i+1)), imag(p));
            int k = fcmp((get(i+1) - get(i)) * (p - get(i)));
            if (k > 0 and d1 <= 0 and d2 > 0) wn++;
            if (k < 0 and d2 <= 0 and d1 > 0) wn--;
        }
        return wn;
    }

    bool convex_contain(pt p) { // non-strict, passed "SPOJ INOROUT"
        auto qs = [&] (int x) { return at(x) - front(); };
        vec q = back() - front(); p -= front();
        if (rotOrder(p, qs(1), q) < 0) return false;
        int l = 0, r = size() - 1;
        while (l + 1 < r) {
            int m = (l + r) / 2;
            if (rotOrder(p, qs(m), q)) l = m; else r = m;
        }
        if (l == 0) return false;
        vec lp = qs(l), rp = qs(r);
        return fcmp(fabs(lp * p) + fabs(p * rp) + fabs((rp - lp) * (p - lp)),
                    lp * rp) == 0;
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
    assert(getIntersection({0, 0}, {1, 1}, {2, 0}, {0, 1}) == pt(2, 2));
    assert(project(pt(0, 2), make_pair(pt(0, 0), pt(2, 2))) == pt(1, 1));
    assert(distance(pt(0, 0), {pt(3, 4), pt(3, 5)}) == 5);
    assert(distance(pt(0, 0), {pt(3, 5), pt(3, 4)}) == 5);
    assert(distance(pt(1, 1), {pt(0, 0), pt(0, 2)}) == 1);
}

void polygon_test() {
    polygon a;
    a.emplace_back(1, 2);
    a.emplace_back(2, 3);
    a.emplace_back(3, 4);
    assert((a == vector<pt>{{1, 2}, {2, 3}, {3, 4}}));
    assert(a.get(-1) == pt(3, 4));
    assert(a.get(1) == pt(2, 3));
    assert(a.get(3) == pt(1, 2));
    (vector<pt>&)a = vector<pt>{{0, 0}, {0, 1}, {1, 1}, {1, 0}, {0.5, 0.5}, {0, 0.5}};
    a.make_hull();
    assert((a == vector<pt>{{0, 0}, {1, 0}, {1, 1}, {0, 1}}));
    assert((a & pt{0.5, 0.5}) == true);
    assert((a & pt{0, 0.5}) == true);
    assert((a & pt{1, 2}) == false);
}

int main() {
    class_test();
    point_test();
    polygon_test();
    return 0;
}
