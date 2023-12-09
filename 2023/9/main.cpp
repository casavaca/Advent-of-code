//usr/bin/c++ -g -std=c++20 -o ${o=`mktemp`} "$0" && "$o" "$@"; ret=$?; rm "$o"; exit $s

#include <numeric>
#include <iostream>
#include <vector>
#include <array>
#include <algorithm>
#include <map>
#include <cassert>
#include "../template/header.hpp"

using namespace std;

long long ans1;
long long ans2;

typedef std::array<std::string, 2> TT;

pair<long long, long long> donumbers(vector<int>& v)
{
    assert(v.size());
    if (v.size() == 1) {
        assert(false);
        return {v[0], v[0]};
    }
    // if all is zero
    if (std::all_of(v.begin(), v.end(), [](int x) { return x == 0; })) {
        return {0,0};
    }
    vector<int> diff(v.size()-1);
    for (auto i=1u; i<v.size() ;i++) diff[i-1] = v[i] - v[i-1];
    auto last_level = donumbers(diff);
    return {v.front() - last_level.first, v.back() + last_level.second};
}

pair<long long, long long> doline(string line)
{
    // convert line into vector<int>
    vector<int> vec;
    stringstream ss(line);
    int a;
    while(ss >> a) {
        vec.push_back(a);
    }
    return donumbers(vec);
}


int main()
{
    string line;
    while(getline(cin, line)) {
        auto tmp = doline(line);
        ans1 += tmp.first;
        ans2 += tmp.second;
    }

    cout << ans1 << ' ' << ans2 << '\n';
    return 0;
}

