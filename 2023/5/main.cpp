//usr/bin/c++ -g -std=c++20 -o ${o=`mktemp`} "$0" && "$o" "$@"; ret=$?; rm "$o"; exit $s

#include <algorithm>
#include <iostream>
#include <string>
#include <set>
#include <sstream>
#include <vector>
#include <ranges>
#include <regex>
#include <cassert>

using namespace std;

vector<long long> parse_seed(const string& s) {
    vector<long long> ret;
    stringstream ss;
    ss<<s;
    string tmp;
    ss>>tmp;
    while(ss>>tmp) {
        try {
            ret.push_back(std::stoll(tmp));
        } catch (...) {
            std::cout << "debug---" << tmp << "\n";
            exit(1);
        }
    }
    return ret;
}

map<long long, pair<long long, long long>> g_m[10];
void gen_map(const vector<string> &v, long long cnt) {
    for (auto& s:v) {
        stringstream ss;
        ss<<s;
        string tmp[3];
        ss>>tmp[0]>>tmp[1]>>tmp[2];
        g_m[cnt][std::stoll(tmp[1])].first = std::stoll(tmp[0]);
        g_m[cnt][std::stoll(tmp[1])].second = std::stoll(tmp[2]);;
    }
}

long long step(long long s, map<long long, pair<long long, long long>>& m)
{
    for (auto & p:m) {
        long long src_start = p.first;
        long long len = p.second.second;
        long long dest = p.second.first;
        if (s >= src_start && s < src_start + len) {
            return dest + (s - src_start);
        }
    }
    return s;
}
long long gen_ans1(long long s)
{
    for (long long i=0; i<10; i++) {
        s = step(s, g_m[i]);
    }
    return s;
}

int main()
{
    vector<string> v;
    string s;
    string seed_str;
    long long ans1=0;
    long long ans2=0;
    long long cnt = 0;
    // 1-indexed;
    map<long long,long long> m;
    getline(cin, seed_str);
    vector<long long> seeds = parse_seed(seed_str);

    getline(cin, s);
    getline(cin, s);
    assert(s.back() == ':');

    while(getline(cin, s)) {
        if (s == "") {
            gen_map(v, cnt);
            cnt++;
            v.clear();
            getline(cin, s);
        } else {
            v.push_back(s);
        }
    }
    gen_map(v, cnt);
    cnt++;
    v.clear();
    assert (cnt == 7);

    for (long long i=0; i<10; i++)
    for (auto& p:g_m[i]) {
        cout << p.second.first << ' ' << p.first << ' ' << p.second.second << '\n';
    }

    vector<long long> locations;
    for (auto s:seeds) {
        locations.push_back(gen_ans1(s));
    }

    
    cout << "ans is \n";
    cout << *min_element(locations.begin(), locations.end()) << '\n';
    // cout << ans1 << ' ' << ans2 << '\n';
    return 0;
}

