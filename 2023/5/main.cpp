//usr/bin/c++ -g -std=c++17 -o ${o=`mktemp`} "$0" && "$o" "$@"; ret=$?; rm "$o"; exit $s

#include <deque>
#include <functional>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>
#include <limits>

#include <cassert>

using namespace std;

// common io function

////////////////////////////////////////////////////////////////////////////////
// INPUT
////////////////////////////////////////////////////////////////////////////////

// cin >> tuple<T...>
template <typename... T>
istream& operator>>(istream& is, tuple<T...>& t)
{
    apply([&is](auto&... args) { (is >> ... >> args); }, t);
    return is;
}

// cin >> pair<T1, T2>
template <typename T1, typename T2>
istream& operator>>(istream& is, pair<T1, T2>& p)
{
    is >> p.first >> p.second;
    return is;
}

// cin >> vector<T>
template <typename T>
istream& operator>>(istream& is, vector<T>& vec)
{
    for (T& x : vec)
        is >> x;
    return is;
}

////////////////////////////////////////////////////////////////////////////////
// OUTPUT
////////////////////////////////////////////////////////////////////////////////

// cout << tuple<T...>
template <typename... T>
ostream& operator<<(ostream& os, const tuple<T...>& t)
{
    apply([&os](const auto&... args) { (os << ... << args); }, t);
    return os;
}

// cout << pair<T1, T2>
template <typename T1, typename T2>
ostream& operator<<(ostream& os, const pair<T1, T2>& p)
{
    os << p.first << " " << p.second;
    return os;
}

// cout << vector<T>
template <typename T>
ostream& operator<<(ostream& os, const vector<T>& vec)
{
    for (const T& x : vec)
        os << x << " ";
    return os;
}

typedef pair<long long, long long> R;

vector<R> parse_seed2(const string& s) {
    vector<R> ret;
    stringstream ss;
    ss<<s;
    string tmp;
    ss>>tmp;
    R r;
    while(ss>>r) { ret.push_back(r); }
    return ret;
}

vector<R> parse_seed1(const string& s) {
    vector<R> ret;
    stringstream ss;
    ss<<s;
    string tmp;
    ss>>tmp;
    R r;
    r.second = 1;
    while(ss>>r.first) { ret.push_back(r); }
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

vector<R> step(R s, map<long long, pair<long long, long long>>& m)
{
    vector<R> ret;
    if (m.empty()) {
        return {s};
    }
    while(s.second != 0) {
        for (auto & p:m) {
            // long long src_start = p.first;
            // long long len = p.second.second;
            // long long dest = p.second.first;
            auto it = m.lower_bound(s.first);
            if (it != m.begin()) {
                it--;
                long long it_src = it->first;
                long long it_len = it->second.second;
                long long it_dest = it->second.first;
                long long len = it_src + it_len - s.first;
                if (len > 0) {
                    len = min(len, s.second);
                    assert(s.first > it_src);
                    ret.emplace_back(it_dest + s.first - it_src, len);
                    assert(len);
                    s.first += len;
                    s.second -= len;
                }
                it++;
            }
            if (s.second == 0)
                break;
            if (it != m.end() && it->first > s.first) {
                long long len = it->first - s.first;
                long long it_src = it->first;
                long long it_len = it->second.second;
                long long it_dest = it->second.first;
                len = min(len, s.second);
                ret.emplace_back(s.first, len);
                assert(len);
                s.first += len;
                s.second -= len;
            }
            if (s.second == 0)
                break;
            while(it != m.end() && s.second > 0) {
                long long it_src = it->first;
                long long it_len = it->second.second;
                long long it_dest = it->second.first;
                long long len = min(s.second, it->second.second);
                ret.emplace_back(it->second.first, len);
                s.first += len;
                s.second -= len;
                it++;
            }
            if (s.second == 0)
                break;
            ret.push_back(s);
            s.second = 0;
        }
    }
    return ret;
}

vector<R> gen_ans2(R s)
{
    vector<R> mut = {s};
    for (int i=0; i<10; i++) {
        // cout << "debug layer " << i << " is -- " << mut << "\n";
        vector<R> next;
        for (auto r:mut) {
            vector<R> tmp = step(r, g_m[i]);
            next.insert(next.end(), tmp.begin(), tmp.end());
        }
        // cout << "debug" << next << "\n";
        mut = std::move(next);
    }
    return mut;
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
    vector<R> seeds1 = parse_seed1(seed_str);
    vector<R> seeds2 = parse_seed2(seed_str);

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

    // for (long long i=0; i<10; i++)
    // for (auto& p:g_m[i]) {
    //     cout << p.second.first << ' ' << p.first << ' ' << p.second.second << '\n';
    // }

    vector<R> locations;
    for (auto s:seeds1) {
        // cout << "debug seed is -- " << s << "\n";
        vector<R> new_loc = gen_ans2(s);
        long long dbg = std::numeric_limits<long long>::max();
        for (auto s:new_loc) {
            dbg = min(dbg, s.first);
        }
        // cout << "debug-- " << dbg << "\n";

        locations.insert(locations.end(), new_loc.begin(), new_loc.end());
    }
    ans1 = std::numeric_limits<long long>::max();
    for (auto s:locations) {
        ans1 = min(ans1, s.first);
    }

    for (auto s:seeds2) {
        // cout << "debug seed is -- " << s << "\n";
        vector<R> new_loc = gen_ans2(s);
        long long dbg = std::numeric_limits<long long>::max();
        for (auto s:new_loc) {
            dbg = min(dbg, s.first);
        }
        // cout << "debug-- " << dbg << "\n";

        locations.insert(locations.end(), new_loc.begin(), new_loc.end());
    }
    ans2 = std::numeric_limits<long long>::max();
    for (auto s:locations) {
        ans2 = min(ans2, s.first);
    }

    // cout << locations;
    
    // cout << "ans is \n";
    // cout << *min_element(locations.begin(), locations.end()) << '\n';
    cout << ans1 << ' ' << ans2 << '\n';

    return 0;
}

