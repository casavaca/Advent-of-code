//usr/bin/c++ -g -std=c++20 -O3 -o ${o=`mktemp`} "$0" && "$o" "$@"; ret=$?; rm "$o"; exit $s

#include <numeric>
#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <array>
#include <algorithm>
#include <map>
#include <sstream>
#include <cassert>
#include <cstring>

using namespace std;

// input with commas as space:
// https://en.cppreference.com/w/cpp/locale/ctype_char
struct custom_delims : ctype<char>
{
    custom_delims(const std::string& s) : ctype<char>(make_table(s)) {}
    static const mask* make_table(const std::string& s)
    {
        static mask table[ctype<char>::table_size];
        memcpy(table, classic_table(), table_size);
        for (auto c:s) table[c] = ctype_base::space;
        return &table[0];
    }
};

static void istream_ignore(istream& is, const string& s)
{
    is.imbue(std::locale(is.getloc(), new custom_delims(s)));
};

long long ans1;
long long ans2;

struct P {
    std::string_view s;    // ended up not used....
    vector<int> q;
};

vector<P> in1;
vector<P> in2;
deque<string> g_1s; // using string_view; can't use vector.
deque<string> g_2s; // using string_view; can't use vector.

long long f_cache(map<int, long long>& cache, const P& p, int s_sz, int q_sz)
{
    assert(s_sz >=0);
    assert(q_sz >=0);
    int idx = (s_sz << 16) + q_sz;
    if (cache.count(idx))
        return cache[idx];

    long long ret = 0;
    if (q_sz == 0) {
        string_view v = p.s.substr(0, s_sz);
        if (all_of(v.begin(), v.end(), [](char c){return c != '#';}))
            ret = 1;
        cache[idx] = ret;
        return ret;
    }

    while (s_sz > 0 && p.s[s_sz-1] == '.')
        s_sz--;

    if (q_sz == 1 && s_sz < p.q[q_sz-1]) {
        cache[idx] = ret;
        return ret;
    }
    if (q_sz > 1 && s_sz < 2+ p.q[q_sz-1]) {
        cache[idx] = ret;
        return ret;
    }

    // case '#'
    bool case_sharp_ok = true;
    for (int i=1; i<=p.q[q_sz-1]; i++) {
        if (p.s[s_sz-i] == '.') {
            case_sharp_ok = false;
            break;
        }
    }
    if (q_sz > 1) {
        if (p.s[s_sz - p.q[q_sz-1] - 1] == '#') {
            case_sharp_ok = false;
        }
    }
    if (case_sharp_ok) {
        if (q_sz == 1) {
            ret += f_cache(cache, p, s_sz - p.q[q_sz-1], q_sz-1);
        } else {
            ret += f_cache(cache, p, s_sz - p.q[q_sz-1] - 1, q_sz-1);
        }
    }
    // case '.'
    if (p.s[s_sz-1] != '#') {
        ret += f_cache(cache, p, s_sz-1, q_sz);
    }
    cache[idx] = ret;
    return ret;
}

long long f(P& in)
{
    map<int, long long> cache;
    return f_cache(cache, in, in.s.size(), in.q.size());
}

int main()
{
    string s;
    while(getline(cin, s)) {
        if (s.empty()) break;
        istringstream ss(s);
        istream_ignore(ss, ",");
        string tmp;
        ss>>tmp;
        g_1s.push_back(tmp);
        tmp = tmp + "?" + tmp + "?" + tmp + "?" + tmp + "?" + tmp;
        g_2s.emplace_back(tmp);
        int n;
        vector<int> v;
        while(ss>>n) {
            v.push_back(n);
        }
        vector<int> vv;
        for (int i=0; i<5; i++) vv.insert(vv.end(), v.begin(), v.end());
        in1.emplace_back(P{g_1s.back(), v});
        in2.emplace_back(P{g_2s.back(), vv});
    }

    for (auto &i:in1) {
        long long tmp = f(i);
        ans1 += tmp;
    }
    for (auto &i:in2) {
        long long tmp = f(i);
        ans2 += tmp;
    }

    cout << ans1 << ' ' << ans2 << '\n';
    return 0;
}

