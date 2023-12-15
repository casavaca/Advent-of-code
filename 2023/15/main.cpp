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
#include <cstring>
#include <cassert>

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
// input with commas as space:
// https://en.cppreference.com/w/cpp/locale/ctype_char
struct custom_delims : ctype<char>
{
    custom_delims(const std::string& s) : ctype<char>(make_table(s)) {}
    static const mask* make_table(const std::string& s)
    {
        static mask table[ctype<char>::table_size];
        memcpy(table, classic_table(), table_size);
        for (auto c:s) table[c] = ctype_base::space;
        return &table[0];
    }
};

static void istream_ignore(istream& is, const string& s)
{
    is.imbue(std::locale(is.getloc(), new custom_delims(s)));
};


int main()
{
    istream_ignore(cin, ",");
    vector<string> vs1;
    vector<pair<string,int>> vs2;
    {
        string s;
        while(cin>>s) {
            vs1.push_back(s);
            if (s.back() == '-') {
                vs2.push_back({s.substr(0, s.size()-1), 9999});
            } else {
                vs2.push_back({s.substr(0, s.size()-2), s.back() - '0'});
            }
        }
    }
    for (auto & s:vs1) ans1 += f(s);

    vector<pair<string, int>> m[256];
    for (auto& p:vs2) {
        auto idx = f(p.first);
        assert(idx >= 0 && idx < 256);
        auto it = find_if(m[idx].begin(), m[idx].end(),
                          [&p](const auto& a) { return a.first == p.first; });
        if (p.second == 9999) {
            if (it != m[idx].end())
                m[idx].erase(it);
        } else if (it != m[idx].end()) {
            it->second = p.second;
        } else {
            m[idx].push_back(p);
        }
    }

    for (int i=0; i<256;i++) {
        for (int j=1; j<=m[i].size(); j++) {
            ans2 += (i+1) * j * m[i][j-1].second;
        }
    }

    cout << ans1 << ' ' << ans2 << '\n';
    return 0;
}
