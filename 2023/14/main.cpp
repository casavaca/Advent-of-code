//usr/bin/c++ -g -std=c++20 -O3 -o ${o=`mktemp`} "$0" && "$o" "$@"; ret=$?; rm "$o"; exit $s

#include <numeric>
#include <iostream>
#include <vector>
#include <array>
#include <string>
#include <algorithm>
#include <map>
#include <set>
#include <sstream>
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

void up(vector<string>& in)
{
    auto m = in.size();
    auto n = in[0].size();
    for (auto col = 0u; col < n; col++) {
        for (auto i=0u; i<m; ++i) {
            for (auto j=i; j>0; j--) {
                if (in[j][col] == 'O' && in[j-1][col] == '.') {
                    swap(in[j][col], in[j-1][col]);
                } else {
                    break;
                }
            }
        }
    }
}

void down(vector<string>& in)
{
    auto m = in.size();
    auto n = in[0].size();
    for (auto col = 0u; col < n; col++) {
        for (int i=m-1; i>=0; --i) {
            for (auto j=i; j<(int)(m-1); j++) {
                if (in.at(j)[col] == 'O' && in.at(j+1)[col] == '.') {
                    swap(in[j][col], in[j+1][col]);
                } else {
                    break;
                }
            }
        }
    }
}

void left_s(string& s)
{
    auto sz = s.size();
    int cnt = 0;
    int old_i = 0;
    for (int i=0; i<sz; i++) {
        if (s[i] == 'O') cnt++;
        else if (s[i] == '#') {
            for (int j = old_i; j<i; j++) {
                s[j] = cnt ? (--cnt, 'O') : '.';
            }
            old_i = i+1;
        }
    }
    for (int j = old_i; j<sz; j++) {
        s[j] = cnt ? (--cnt, 'O') : '.';
    }
}

void right_s(string& s)
{
    auto sz = s.size();
    int cnt = 0;
    int old_i = sz-1;
    for (int i=sz-1; i>=0; i--) {
        if (s[i] == 'O') cnt++;
        else if (s[i] == '#') {
            for (int j = old_i; j>i; j--) {
                s[j] = cnt ? (--cnt, 'O') : '.';
            }
            old_i = i-1;
        }
    }
    for (int j = old_i; j>=0; j--) {
        s[j] = cnt ? (--cnt, 'O') : '.';
    }
}

void left(vector<string>& in)
{
    auto m = in.size();
    for (auto row = 0u; row < m; row++) {
        left_s(in[row]);
    }
}

void right(vector<string>& in)
{
    auto m = in.size();
    for (auto row = 0u; row < m; row++) {
        right_s(in[row]);
    }
}

void cycle(vector<string>& in)
{
    up(in);
    left(in);
    down(in);
    right(in);
}
long long f(vector<string>& in)
{
    long long ans = 0;
    auto m = in.size();
    auto n = in[0].size();
    for (auto col = 0u; col < n; col++) {
        for (auto i=0u; i<m; ++i) {
            if (in[i][col] == 'O') ans += m - i;
        }
    }
    return ans;
}

string app(vector<string>& in)
{
    return std::reduce(in.begin(), in.end(), string(), std::plus<>());
}

int main()
{
    auto in = read_matrix(cin);
    auto dup = in;
    auto m = in.size();
    auto n = in[0].size();

    {
        auto dup = in;
        up(dup);
        ans1 = f(dup);
    }

    long long cnt = 0;
    set<string> s;
    vector<string> hist;
    vector<long long> hist_ans;
    while(!s.count(app(in)))
    {
        s.insert(app(in));
        hist.push_back(app(in));
        hist_ans.push_back(f(in));
        cycle(in);
        cnt++;
        // cout << cnt << ' ';
    }
    long long init = 0;
    long long mod = 0;

    for (int i=0; i<hist.size(); i++) {
        if (hist[i] == app(in)) {
            // cout << i << ' ' << hist.size() - i;
            init = i;
            mod = hist.size() - i;
        }
    }

    {
        int times = init + (1000000000 - init) % mod;
        for (int i=1; i<=times; i++) cycle(dup);
        ans2 = f(dup);
    }

    cout << ans1 << ' ' << ans2 << '\n';
    return 0;
}
