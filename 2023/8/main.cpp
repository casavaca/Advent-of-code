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

long long f(map<string, TT>& v, string inst)
{
    auto ptr = inst.begin();
    long long cnt = 0;
    string start = "AAA"; // set manually
    while(start != "ZZZ") {
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

    // cout << v;
    ans1 = f(v, inst);

    cout << ans1 << ' ' << ans2 << '\n';
    return 0;
}

