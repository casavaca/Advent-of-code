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
        } else {
            int num = std::stoi(tmp[0]);
            if (bar == false)
                winning.insert(num);
            else {
                if (winning.count(num)) {
                    ans++;
                }
            }
        }
    }
    return ans;
}

static array<int, 220> num_copy;

int main()
{
    vector<string> v;
    string s;
    long long ans1=0;
    long long ans2=0;
    int cnt = 0;
    // 1-indexed;
    map<int,int> m;
    while(getline(cin, s)) {
        m[++cnt] = f(s);
        if (m[cnt]) {
            ans1 += (1<<(m[cnt] - 1));
        }
    }

    num_copy.fill(1);

    for (auto& p:m) {
        int mul = num_copy[p.first];
        for (int i=1; i<=p.second; i++) {
            num_copy[p.first + i] += mul;
        }
    }

    for (auto& p:m) {
        ans2 += num_copy[p.first];
    }

    cout << ans1 << ' ' << ans2 << '\n';
    return 0;
}
