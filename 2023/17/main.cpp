//usr/bin/c++ -g -std=c++20 -O3 -o ${o=`mktemp`} "$0" && "$o" "$@"; ret=$?; rm "$o"; exit $s

#include <numeric>
#include <iostream>
#include <vector>
#include <deque>
#include <array>
#include <string>
#include <algorithm>
#include <map>
#include <set>
#include <sstream>
#include <cstring>
#include <cassert>

#include "../template/header.hpp"

long long ans1;
long long ans2;

vector<string> g_in;
enum direction {U, R, D, L};

struct S{
    int i,j;
    int hist;
    direction d;
    long long ans;
};
bool operator<(const S& a, const S& b) {
    return a.i < b.i || (a.i == b.i && a.j < b.j) || (a.i == b.i && a.j == b.j && a.d < b.d) ||
            (a.i == b.i && a.j == b.j && a.d == b.d && a.hist < b.hist);
}

static inline int V(int i, int j)
{
    int n_row = g_in.size();
    int n_col = g_in[0].size();
    if (i < 0 || i >= n_row || j < 0 || j >= n_col) return -1;
    return g_in[i][j] - '0';
}

long long f(int which_part) // use g_in
{
    int n_row = g_in.size();
    int n_col = g_in[0].size();
    deque<S> q {{0,0,1,D,0}, {0,0,1,R,0}};
    map<S, long long> cache;
    while(q.size()) {
        deque<S> next;
        for (auto& s:q) {

            if (cache.count(s) > 0 && s.ans >= cache[s]) {
                continue;
            }
            cache[s] = s.ans;

            deque<S> potential_next;
            if (s.hist >= 4) {
                switch (s.d) {
                case L:
                case R:
                    potential_next.push_back({s.i-1, s.j, 1, U,s.ans + V(s.i-1,s.j)});
                    potential_next.push_back({s.i+1, s.j, 1, D,s.ans + V(s.i+1,s.j)});
                    break;
                case U:
                case D:
                    potential_next.push_back({s.i, s.j+1, 1, R, s.ans + V(s.i, s.j+1)});
                    potential_next.push_back({s.i, s.j-1, 1, L, s.ans + V(s.i, s.j-1)});
                    break;
                default: assert(false);
                }
            }
            if (s.hist < 10) {
                switch(s.d) {
                case U: potential_next.push_back({s.i-1, s.j, s.hist+1, s.d, s.ans + V(s.i-1, s.j)}); break;
                case R: potential_next.push_back({s.i, s.j+1, s.hist+1, s.d, s.ans + V(s.i, s.j+1)}); break;
                case L: potential_next.push_back({s.i, s.j-1, s.hist+1, s.d, s.ans + V(s.i, s.j-1)}); break;
                case D: potential_next.push_back({s.i+1, s.j, s.hist+1, s.d, s.ans + V(s.i+1, s.j)}); break;
                }
            }
            for (auto &p:potential_next) {
                if (p.i < 0 || p.i >= n_row || p.j < 0 || p.j >= n_col) continue;
                next.push_back(p);
            }
        }
        q = std::move(next);
    }
    long long ans = numeric_limits<long long>::max();
    direction directions[] = {U,D,R,L};
    vector<S> haha;
    if (which_part == 1) {
        haha = {
            {n_row-1, n_col-1, 1, D, 0},
            {n_row-1, n_col-1, 2, D, 0},
            {n_row-1, n_col-1, 3, D, 0},
        };
    } else {
        haha = {
            {n_row-1, n_col-1, 4, D, 0},
            {n_row-1, n_col-1, 5, D, 0},
            {n_row-1, n_col-1, 6, D, 0},
            {n_row-1, n_col-1, 7, D, 0},
            {n_row-1, n_col-1, 8, D, 0},
            {n_row-1, n_col-1, 9, D, 0},
            {n_row-1, n_col-1, 10,D, 0},
        };
    }
    for (int i=0; i<4; i++) {
        for (auto& s:haha) {
            s.d = directions[i];
            if (cache.count(s) > 0)
                ans = min(ans, cache[s]);
        }
    }
    return ans;
}

int main()
{
    g_in = read_matrix(cin);
    ans1 = f(1);
    ans2 = f(2);

    cout << ans1 << ' ' << ans2 << '\n';
    return 0;
}
