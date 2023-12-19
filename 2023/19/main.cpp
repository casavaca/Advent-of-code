//usr/bin/c++ -g -std=c++20 -O3 -o ${o=`mktemp`} "$0" && "$o" "$@"; ret=$?; rm "$o"; exit $s

#include <numeric>
#include <iostream>
#include <vector>
#include <deque>
#include <array>
#include <string>
#include <functional>
#include <algorithm>
#include <variant>
#include <map>
#include <set>
#include <sstream>
#include <cstring>
#include <cassert>

#include "../template/header.hpp"

using namespace std;

long long ans1;
long long ans2;

struct SubRule {
    // cond;
    bool always;
    char name;
    char op;
    int val;
    // dest
    bool dest_is_final;
    string dest;
    bool accept;
};

struct Rule {
    string name;
    string ori_str;
    vector<SubRule> subrules;
};

ostream& operator<<(ostream& os, const Rule& r)
{
    os << "Rule name " << r.name << '\n';
    os << "  "         << r.ori_str << '\n';
    return os;
}

ostream& operator<<(ostream& os, const SubRule& sr)
{
    os << "  " << sr.always << ' ' << sr.name << ' ' << sr.op << ' ' << sr.val << ' ' << sr.dest_is_final << ' ' << sr.dest << ' ' << sr.accept << '\n';
    return os;
}

struct Item {
    array<int,4> val; // x m a s
};

typedef tuple<std::string, array<array<int, 2>, 4>> CacheItem;

variant<bool, string> step(const Rule& r,const Item& i)
{
    for (auto & sr : r.subrules) {
        if (sr.always) {
            if (sr.dest_is_final) {
                return sr.accept;
            } else {
                return sr.dest;
            }
        } else {
            std::function<bool(int,int)> cmp = std::less<int>();
            switch (sr.op) {
            case '>': {
                cmp = std::greater<int>();
                break;
            }
            case '<': break;
            default: assert(false);
            }
            int val;
            switch (sr.name) {
            case 'x': val = i.val[0]; break;
            case 'm': val = i.val[1]; break;
            case 'a': val = i.val[2]; break;
            case 's': val = i.val[3]; break;
            default: assert(false);
            }
            if (cmp(val, sr.val)) {
                if (sr.dest_is_final) {
                    return sr.accept;
                } else {
                    return sr.dest;
                }
            }
        }
    }
    assert(false);
}

SubRule parse_sub(string s)
{
    SubRule sr;
    auto colon_pos = s.find(':');
    if (colon_pos != string::npos) {
        s[colon_pos] = ' ';
        sr.always = false;
        sr.name = s[0];
        sr.op   = s[1];
        sr.val  = stoi(s.substr(2));
    } else {
        sr.always = true;
    }
    string second_half = s.substr(colon_pos+1);
    if (second_half == "A" || second_half == "R") {
        sr.dest_is_final = true;
        sr.accept = (second_half == "A");
    } else {
        sr.dest_is_final = false;
        sr.dest = second_half;
    }
    return sr;
}

Rule parse(string s)
{
    Rule ret;
    ret.ori_str = s;
    assert(s.size() && s[0] != '{');
    for (auto& c:s)if(c=='{'||c==','||c=='}') c=' ';
    stringstream ss(s);
    ss>>ret.name;
    string tmp;
    while(ss>>tmp) {
        ret.subrules.push_back(parse_sub(tmp));
    }
    return ret;
}

Item parse_item(string s)
{
    Item i;
    stringstream ss(s);
    istream_ignore(ss, "{},xmas=");
    ss>>i.val[0]>>i.val[1]>>i.val[2]>>i.val[3];
    return i;
}

map<string, Rule> g_rules;
vector<Item>      g_in;

bool acc(Item& i)
{
    string cur = "in";
    while(true) {
        auto v = step(g_rules[cur],i);
        if (v.index() == 0) {
            return get<bool>(v);
        } else {
            cur = get<string>(v);
        }
    }
    assert(false);
}

long long f()
{
    long long ret = 0;
    for (auto & i:g_in) {
        if (acc(i)) {
            ret += i.val[3];
            ret += i.val[2];
            ret += i.val[1];
            ret += i.val[0];
        }
    }
    return ret;
}

map<CacheItem, long long> g_cache;

long long doit(string name, array<array<int, 2>, 4> boundaries, int start_rule, bool expect_all_match = false)
{
    if (start_rule == 0) {
        if (g_cache.count({name, boundaries}))
            return g_cache.at({name, boundaries});
    }

    long long ret = 0;

    auto sr = g_rules[name].subrules.at(start_rule);
    {
        int index = -1;
        if (!sr.always) {
            switch (sr.name) {
            case 'x': index = 0; break;
            case 'm': index = 1; break;
            case 'a': index = 2; break;
            case 's': index = 3; break;
            default: assert(false);
            }
        }

        bool all_match = sr.always;
        if (!all_match) {
            if (sr.op == '>') {
                all_match = ((boundaries[index][0] > sr.val) || (boundaries[index][1] <= sr.val));
            } else if (sr.op == '<') {
                all_match = ((boundaries[index][1] < sr.val) || (boundaries[index][0] >= sr.val));
            }
        }

        if (expect_all_match) {
            if (!all_match) {
                cout << boundaries << endl;
                cout << g_rules[name].subrules.at(start_rule) << endl;
            }
            assert(all_match);
        }

        if (!all_match) {
            // split;
            auto boundaries_1 = boundaries;
            auto boundaries_2 = boundaries;

            if (sr.op == '>') {
                boundaries_1[index][1] = sr.val;
                boundaries_2[index][0] = sr.val + 1;
            } else {
                boundaries_1[index][1] = sr.val - 1;
                boundaries_2[index][0] = sr.val;
            }
            ret += doit(name, boundaries_1, start_rule, true);
            ret += doit(name, boundaries_2, start_rule, true);
        } else {
            bool cond_ok = sr.always;
            if (!cond_ok) {
                std::function<long long(int,int)> cmp = std::less<int>();
                switch (sr.op) {
                case '>': {
                    cmp = std::greater<int>();
                    break;
                }
                case '<': break;
                default: assert(false);
                }
                int val;
                switch (sr.name) {
                case 'x': val = boundaries[0][0]; break;
                case 'm': val = boundaries[1][0]; break;
                case 'a': val = boundaries[2][0]; break;
                case 's': val = boundaries[3][0]; break;
                default: assert(false);
                }
                cond_ok = (cmp(val, sr.val));
            }

            if (cond_ok) {
                if (sr.dest_is_final && sr.accept) {
                    long long local = 1;
                    local *= (boundaries[0][1] - boundaries[0][0] + 1);
                    local *= (boundaries[1][1] - boundaries[1][0] + 1);
                    local *= (boundaries[2][1] - boundaries[2][0] + 1);
                    local *= (boundaries[3][1] - boundaries[3][0] + 1);
                    ret = local;
                } else if (sr.dest_is_final && !sr.accept) {
                    // nothing;
                } else {
                    ret = doit(sr.dest, boundaries, 0);
                }
            } else {
                ret = doit(name, boundaries, start_rule + 1);
            }
        }
    }

    if (start_rule == 0)
        g_cache[{name, boundaries}] = ret;
    return ret;
}

long long g()
{
    array<array<int, 2>, 4> start_condition;
    for (auto& ii:start_condition) {
        ii[0] = 1;
        ii[1] = 4000;
    }
    return doit("in", start_condition, 0);
}

int main()
{
    string s;
    while(cin>>s) {
        if (s[0] == '{') {
            g_in.push_back(parse_item(s));
        } else {
            Rule r = parse(s);
            g_rules[r.name] = r;
        }
    }
    ans1 = f();
    ans2 = g();
    cout << ans1 << ' ' << ans2 << '\n';
    return 0;
}
