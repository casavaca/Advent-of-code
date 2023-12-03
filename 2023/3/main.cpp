//usr/bin/c++ -g -std=c++20 -o ${o=`mktemp`} "$0" && "$o" "$@"; ret=$?; rm "$o"; exit $s

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <ranges>
#include <regex>
#include <cassert>

using namespace std;

/*
467..114..
...*......
..35..633.
......#...
617*......
.....+.58.
..592.....
......755.
...$.*....
.664.598..
*/

static map<int, map<int, vector<int>>> m;


bool adj(const vector<string>& v, int n, int i, int j, int num)
{
    bool ret = false;
    assert (i>0 && i < v.size() - 1);
    assert (j>0 && j < v[0].size() - 1);
    for (int ii = n-1; ii<=n+1; ii++)
        for (int jj = i-1; jj <= j+1; jj++)
            if (v[ii][jj] != '.' && !isdigit(v[ii][jj])) {
                ret = true;
                if (v[ii][jj] == '*') {
                    m[ii][jj].push_back(num);
                }
            }
    return ret;
}

long long f(vector<string>& v)
{
    // for (auto s:v) cout << s << '\n';

    long long ret = 0;
    int m = v.size();
    int n = v[0].size();
    for (int i=0; i<m; i++) {
        long long local = 0;
        bool prev_is_num = false;
        int num_begin = -1;
        int num_end   = -1;

        for (int j=0; j<n; j++) {
            if (v[i][j] >= '0' && v[i][j] <= '9') {
                if (prev_is_num == false) {
                    prev_is_num = true;
                    num_begin = j;
                    num_end = j;
                    local = (v[i][j] - '0');
                } else {
                    assert(prev_is_num);
                    local *= 10;
                    local += (v[i][j] - '0');
                    num_end++;
                }
            } else {
                if (prev_is_num) {
                    if (adj(v, i,num_begin, num_end, local)) {
                        // cout << local << '\n';
                        ret += local;
                    }
                }
                prev_is_num = false;
            }
        }
    }
    return ret;
}

int main()
{
    vector<string> v;
    string s;
    long long ans1=0;
    long long ans2=0;
    bool first = true;
    while(getline(cin, s)) {
        s = string(".") + s + ".";
        if (first) {
            v.push_back(string(s.size(), '.'));
            first = false;
        }
        v.push_back(s);
    }
    v.push_back(string(v.back().size(), '.'));
    ans1 = f(v);

    for (auto& mi : m) {
        for (auto& mj : mi.second) {
            if (mj.second.size() == 2) {
                ans2 += mj.second[0] * mj.second[1];
            }
        }
    }

    // cout << ans1 << '\n';
    cout << ans1 << ' ' << ans2 << '\n';
    return 0;
}
