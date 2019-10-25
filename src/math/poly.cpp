#include <bits/stdc++.h>
using namespace std;

#define rep(i, n) for (int i = 0; i < int(n); i++)
#define Rep(i, n) for (int i = 1; i <=int(n); i++)
#define range(x) begin(x), end(x)
typedef long long LL;
/*
%%%
title: Fast Polynomial Operations
desc: \lstinline|inverse|, \lstinline|log|, \lstinline|exp| are modulo $x^n$. \lstinline|operator *|, \lstinline|div| do not.
%%%
*/

extern const int mod;

LL powmod(LL b, LL e) {
    LL r = 1;
    while (e) {
        if (e & 1) r = r * b % mod;
        b = b * b % mod;
        e >>= 1;
    }
    return r;
}

void printvec(const vector<int>& a) {
    printf("[");
    rep (i, a.size()) {
        if (i) printf(", "); 
        printf("%d", a[i]);
    }
    printf("]\n");
}

// +++
#pragma GCC optimize("fast-math")

const int mod = 998244353;
namespace poly {
    typedef complex<double> num;

    int base = 1;
    vector<num> roots = {0, 1};
    vector<int> rev = {0, 1};
    const double pi = acos(-1.0);

    void ensure_base(int nbase) {
        if (nbase <= base) return;
        rev.resize(1<<nbase);
        rep (i, 1<<nbase) rev[i] = (rev[i>>1]>>1)|((i&1)<<(nbase-1));
        roots.resize(1<<nbase);
        while (base < nbase) {
            double angle = 2 * acos(-1) / (1 << (base + 1));
            for (int i = 1 << (base - 1); i < (1 << base); i++) {
                roots[i<<1] = roots[i];
                roots[i<<1|1] = polar(1.0, angle*(2*i+1-(1<<base)));
            }
            base++;
        }
    }

    void fft(vector<num> &a, int n = -1) {
        if (n < 0) n = a.size();
        assert((n & (n - 1)) == 0);
        int zeros = __builtin_ctz(n), sh = base - zeros;
        ensure_base(zeros);
        rep (i, n) if (i < (rev[i]>>sh)) swap(a[i], a[rev[i]>>sh]);
        for (int k = 1; k < n; k <<= 1) {
            for (int i = 0; i < n; i += 2*k) {
                for (int j = 0; j < k; j++) {
                    num z = a[i+j+k] * roots[j+k];
                    a[i+j+k] = a[i+j]-z;
                    a[i+j] += z;
                }
            }
        }
    }
    
    vector<int> multiply(const vector<int>& a, const vector<int>& b) {
        int sz = a.size() + b.size() - 1;
        vector<int> ret(sz);
        int nbase = 0;
        while ((1 << nbase) < sz) nbase++;
        ensure_base(nbase);
        sz = 1 << nbase;
        vector<num> fa(sz);
        rep (i, a.size()) fa[i].real(a[i]);
        rep (i, b.size()) fa[i].imag(b[i]);
        fft(fa);
        num r(0, -0.25 / sz);
        for (int i = 0; i <= (sz >> 1); i++) {
            int j = (sz - i) & (sz - 1);
            num z = (fa[j] * fa[j] - conj(fa[i] * fa[i])) * r;
            if (i != j) fa[j] = (fa[i] * fa[i] - conj(fa[j] * fa[j])) * r;
            fa[i] = z;
        }
        fft(fa);
        rep (i, ret.size()) ret[i] = llround(real(fa[i]));
        return ret;
    }
    
    vector<int> operator * (const vector<int>& a, const vector<int>& b) {
        int sz = a.size() + b.size() - 1;
        vector<int> res(sz);
        int nbase = 0;
        while ((1 << nbase) < sz) nbase++;
        ensure_base(nbase);
        sz = 1 << nbase;
        vector<num> fa(sz), fb(sz);
        rep (i, a.size()) {
            int x = (a[i] % mod + mod) % mod;
            fa[i] = num(x & ((1<<15)-1), x >> 15);
        }
        fft(fa);
        rep (i, b.size()) {
            int x = (b[i] % mod + mod) % mod;
            fb[i] = num(x & ((1<<15)-1), x >> 15);
        }
        fft(fb);
        double ratio = 0.25 / sz;
        for (int i = 0; i <= (sz >> 1); i++) {
            int j = (sz - i) & (sz - 1);
            num ra[2], rb[2];
            rep (s, 2) {
                num a1 = (fa[i] + conj(fa[j])),
                    a2 = (fa[i] - conj(fa[j])) * num(0, -1),
                    b1 = (fb[i] + conj(fb[j])) * ratio,
                    b2 = (fb[i] - conj(fb[j])) * num(0, -ratio);
                ra[s] = a1 * b1 + a2 * b2 * num(0, 1);
                rb[s] = a1 * b2 + a2 * b1;
                swap(i, j);
            }
            rep (s, 2) fa[j] = ra[s], fb[j] = rb[s], swap(i, j);
        }
        fft(fa); fft(fb);
        rep (i, res.size()) 
            res[i] = (llround(real(fa[i])) + 
                (llround(real(fb[i])) % mod << 15) +
                (llround(imag(fa[i])) % mod << 30)) % mod;
        return res;
    }

    vector<int> inverse(const vector<int> &a, int n = -1) {
        assert(a[0]);
        if (n < 0) n = a.size();
        if (n == 1) return {(int)powmod(a[0], mod - 2)};
        auto ret = inverse(a, (n + 1) >> 1), fa = ret * ret * a, fb = ret;
        ret.resize(n); fb.resize(n); fa.resize(n);
        rep (i, n) ret[i] = ((fb[i] + fb[i] - fa[i]) % mod + mod) % mod;
        return ret;
    }

    vector<int> diff(const vector<int>& a) {
        vector<int> ret(a.size());
        Rep (i, a.size() - 1) ret[i-1] = 1ll * a[i] * i % mod;
        return ret;
    }

    vector<int> integrate(const vector<int>& a) {
        int n = a.size();
        vector<int> ret(n), inv(n);
        if (n > 1) inv[1] = 1;
        for (int i = 2; i < n; i++) 
            inv[i] = (mod - 1ll * inv[mod % i] * (mod / i) % mod) % mod;
        for (int i = n - 1; i; i--) ret[i] = 1ll * a[i-1] * inv[i] % mod;
        return ret;
    }

    vector<int> log(const vector<int> &a) {
        assert(a[0] == 1);
        auto ret = integrate(diff(a) * inverse(a));
        ret.resize(a.size());
        return ret;
    }

    vector<int> exp(const vector<int> &a, int n = -1) {
        if (n < 0) n = a.size();
        if (n == 1) return {1};
        auto ret = exp(a, (n+1)>>1); ret.resize(n);
        auto ff = log(ret); for (int& x : ff) if (x) x = mod - x;
        ff[0] = (ff[0] + 1) % mod;
        rep (i, n) ff[i] = (ff[i] + a[i]) % mod;
        ret = ret * ff; ret.resize(n);
        return ret;
    }
    
    //   quotient     remainder
    pair<vector<int>, vector<int>> 
    div(const vector<int> &a, const vector<int> &b) {
        int n = a.size(), m = b.size();
        if (n < m) return {vector<int>{0}, a};
        auto tmp = b; reverse(range(tmp));
        tmp = inverse(tmp, n - m + 1);
        auto rev = a; reverse(range(rev));
        auto q = tmp * rev; q.resize(n - m + 1); reverse(range(q));
        auto t = b * q;
        vector<int> r(m - 1);
        rep (i, m - 1) r[i] = (a[i] - t[i] + mod) % mod;
        return {q, r};
    }
}

// +++

int main() {
    using namespace poly;
    typedef vector<int> Poly;
    // Multiplication
    assert((Poly{1, 2, 3} * Poly{3, 4} == Poly{3, 10, 17, 12}));
    // Inverse
    assert((inverse(Poly{1, 7, 4, 9, 8}) == 
        Poly{1, 998244346, 45, 998244057, 1947}));
    // Differetiate
    assert((diff(Poly{2, 4, 5, 6}) == Poly{4, 10, 18, 0}));
    // Integrate
    assert((integrate(Poly{4, 6, 18, 12, 0}) == Poly{0, 4, 3, 6, 3}));
    // Log
    assert((log(Poly{1, 6, 4, 0}) == Poly{0, 6, 998244339, 48}));
    // Exp
    assert((exp(Poly{0, 3, 6, 1}) == Poly{1, 3, 499122187, 499122200}));
    // Division
    assert(div(Poly{3, 3, 1}, Poly{1, 1}) == make_pair(Poly{2, 1}, Poly{1}));
}

