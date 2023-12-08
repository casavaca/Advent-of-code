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

long long f(map<string, TT>& v, string inst, string start)
{
    auto ptr = inst.begin();
    long long cnt = 0;
    while(start != "ZZZ") {
        cnt++;
        start = (*ptr) == 'L' ? v[start][0] : v[start][1];
        ptr++;
        if (ptr==inst.end())
            ptr = inst.begin();
    }
    return cnt;
}

long long g(map<string, TT>& v, string inst, string start)
{
    auto ptr = inst.begin();
    long long cnt = 0;
    while(start[2] != 'Z') {
        cnt++;
        start = (*ptr) == 'L' ? v[start][0] : v[start][1];
        ptr++;
        if (ptr==inst.end())
            ptr = inst.begin();
    }
    return cnt;
}

int main()
{
    string inst;
    cin>>inst;
    
    istream_ignore(cin, "=(),");
    map<string, TT> v;
    string key;
    TT tmp;
    while(cin>>key>>tmp) {
        v[key] = tmp;
    }

    ans1 = f(v, inst, "AAA");

    vector<long long> steps;
    for (auto & e:v) {
        if (e.first[2] == 'A') {
            long long tmp = g(v,inst,e.first);
            steps.push_back(tmp);
        }
    }

    ans2 = std::reduce(steps.begin(), steps.end(), 1ll, std::lcm<long long,long long>);

    cout << ans1 << ' ' << ans2 << '\n';
    return 0;
}

