//usr/bin/c++ -g -std=c++20 -o ${o=`mktemp`} "$0" && "$o" "$@"; ret=$?; rm "$o"; exit $s

#include <iostream>
#include <string>
#include <set>
#include <sstream>
#include <vector>
#include <ranges>
#include <regex>
#include <cassert>

using namespace std;

/*
Card 1: 41 48 83 86 17 | 83 86  6 31 17  9 48 53
Card 2: 13 32 20 16 61 | 61 30 68 82 17 32 24 19
Card 3:  1 21 53 59 44 | 69 82 63 72 16 21 14  1
Card 4: 41 92 73 84 69 | 59 84 76 51 58  5 54 83
Card 5: 87 83 26 28 32 | 88 30 70 12 93 22 82 36
Card 6: 31 18 13 56 72 | 74 77 10 23 35 67 36 11
 */

long long f(const string& s) {
    long long ans = 0;
    stringstream ss;
    ss<<s;
    string tmp[2];
    ss>>tmp[0]>>tmp[1];
    std::set<int> winning;
    bool bar = false;
    while(ss>>tmp[0]) {
        if (tmp[0][0] == '|') {
            bar = true;
            // for (auto i:winning) cout << i << ' ';
        } else {
            int num = std::stoi(tmp[0]);
            if (bar == false)
                winning.insert(num);
            else {
                if (winning.count(num)) {
                    if (ans== 0) ans = 1;
                    else ans *= 2; // TODO: check input for over flow
                }
            }
        }
    }
    return ans;
}

int main()
{
    vector<string> v;
    string s;
    long long ans1=0;
    long long ans2=0;
    while(getline(cin, s)) {
        ans1 += f(s);
    }
    cout << ans1 << '\n';
    // cout << ans1 << ' ' << ans2 << '\n';
    return 0;
}
