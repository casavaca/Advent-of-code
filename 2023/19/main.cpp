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

struct Item {
    array<int,4> val; // x m a s
};

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

long long g()
{
    return 0;
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
