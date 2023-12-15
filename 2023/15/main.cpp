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
#include "../template/header.hpp"

using namespace std;

long long ans1;
long long ans2;

long long f(string s) {
    long long ans =0;
    for (auto c:s) {
        ans = (ans + (int)c)*17%256;
    }
    return ans;
}


int main()
{
    istream_ignore(cin, ",");
    vector<string> vs;
    {
        string s;
        while(cin>>s) vs.push_back(s);
    }

    for (auto& s:vs) {
        ans1 += f(s);
    }

    cout << ans1 << ' ' << ans2 << '\n';
    return 0;
}
