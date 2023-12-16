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

using namespace std;

vector<string> read_matrix(istream& is)
{
    vector<string> vec;
    string line;
    while (getline(is, line)) {
        vec.push_back(line);
    }
    return vec;
}

long long ans1;
long long ans2;

vector<string> in;
enum Direction {
    U,
    D,
    L,
    R,
};
struct S {
    int i,j;
    int direction;
};
long long f(vector<string>& in, S start)
{
    set<pair<int,int>> reached;
    set<tuple<int,int,int>> cache;
    long long step = in.size() * in[0].size();
    deque<S> q {start};
    for (int i=0; i<step; i++) {
        deque<S> next;
        for (auto& s : q) {
            if (cache.count({s.i,s.j,s.direction})) {
                continue;
            }
            if (s.i < 0 || s.i >= in.size() || s.j < 0 || s.j >= in[0].size()) {
                continue;
            }
            cache.insert({s.i,s.j,s.direction});
            reached.insert({s.i,s.j});
            switch (in[s.i][s.j]) {
            case '.': {
                switch (s.direction) {
                case U: next.push_back({s.i-1,s.j,s.direction}); break;
                case D: next.push_back({s.i+1,s.j,s.direction}); break;
                case L: next.push_back({s.i,s.j-1,s.direction}); break;
                case R: next.push_back({s.i,s.j+1,s.direction}); break;
                }
            } break;
            case '-': {
                switch (s.direction) {
                case L: next.push_back({s.i,s.j-1,s.direction}); break;
                case R: next.push_back({s.i,s.j+1,s.direction}); break;
                default:
                    next.push_back({s.i,s.j-1,L});
                    next.push_back({s.i,s.j+1,R});
                }
            } break;
            case '|': {
                switch (s.direction) {
                case U: next.push_back({s.i-1,s.j,s.direction}); break;
                case D: next.push_back({s.i+1,s.j,s.direction}); break;
                default:
                    next.push_back({s.i-1,s.j,U});
                    next.push_back({s.i+1,s.j,D});
                }
            } break;
            case '/': {
                switch (s.direction) {
                case U: next.push_back({s.i,s.j+1,R}); break;
                case D: next.push_back({s.i,s.j-1,L}); break;
                case L: next.push_back({s.i+1,s.j,D}); break;
                case R: next.push_back({s.i-1,s.j,U}); break;
                }
            } break;
            case '\\': {
                switch (s.direction) {
                case D: next.push_back({s.i,s.j+1,R}); break;
                case U: next.push_back({s.i,s.j-1,L}); break;
                case R: next.push_back({s.i+1,s.j,D}); break;
                case L: next.push_back({s.i-1,s.j,U}); break;
                }
            } break;
            }
        }
        q = std::move(next);
    }
    return reached.size();
}


int main()
{
    in = read_matrix(cin);
    ans1 = f(in, {0,0,R});
    for (int i=0; i<in.size(); i++) {
        ans2 = max(ans2, f(in, {i,0,R}));
        ans2 = max(ans2, f(in, {i, (int)(in[0].size()-1),L}));
    }
    for (int j=0; j<in[0].size(); j++) {
        ans2 = max(ans2, f(in, {0,j,D}));
        ans2 = max(ans2, f(in, {(int)(in.size()-1),j,U}));
    }

    cout << ans1 << ' ' << ans2 << '\n';
    return 0;
}
