//usr/bin/c++ -g -std=c++20 -o ${o=`mktemp`} "$0" && "$o"; ret=$?; rm "$o"; exit $s

#include <functional>
#include <limits>
#include <numeric>
#include <string>
#include <set>
#include <map>
#include <numeric>
#include <iostream>
#include <format>
using namespace std;

long long ans1=0;
long long ans2=0;

bool f(const string& s)
{
    int vowels_cnt = 0;
    bool cond[3] = {false, false, true};
    string vowels = "aeiou";
    for (int i=0; i<s.size(); i++) {
        if (vowels.find(s[i]) != vowels.npos)
            vowels_cnt++;
        if (i>=1) {
            cond[1] |= (s[i] == s[i-1]);
            // ab, cd, pq, or xy
            if ((s[i-1] == 'a' && s[i] == 'b') ||
                (s[i-1] == 'c' && s[i] == 'd') ||
                (s[i-1] == 'p' && s[i] == 'q') ||
                (s[i-1] == 'x' && s[i] == 'y'))
                cond[2] = false;
        }
    }
    cond[0] = (vowels_cnt >= 3);
    return cond[0] && cond[1] && cond[2];
}

bool g(const string& s)
{
    map<pair<char, char>, vector<int>> cnt;
    bool cond[2] = {false, false};
    for (int i=0; i<s.size(); i++) {
        if (i>=1) {
            cond[1] |= (s[i] == s[i-2]);
            cnt[{s[i-1], s[i]}].push_back(i);
        }
    }
    if (cond[1] == false) return false;
    for (char x = 'a'; x <= 'z'; x++)
        for (char y = 'a'; y <= 'z'; y++)
            if (x != y && cnt[{x, y}].size() >= 2)
                return true;
    // special case of 'aa', 'bb', etc.
    for (char x = 'a'; x <= 'z'; x++)
        if (cnt[{x, x}].size() >= 2 &&
            cnt[{x, x}].back() - cnt[{x,x}].front() > 1)
            return true;

    return false;
}

int main()
{
    string s;
    while(cin>>s) {
        ans1 += (f(s) ? 1 : 0);
        ans2 += (g(s) ? 1 : 0);
    }

    cout << std::format("{} {}\n", ans1, ans2);
    return 0;
}
