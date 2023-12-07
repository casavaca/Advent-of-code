//usr/bin/c++ -g -std=c++20 -o ${o=`mktemp`} "$0" && "$o"; ret=$?; rm "$o"; exit $s

#include <functional>
#include <istream>
#include <limits>
#include <numeric>
#include <regex>
#include <string>
#include <set>
#include <map>
#include <numeric>
#include <iostream>
#include <format>

#include "../template/segment_tree.hpp"

using namespace std;

long long ans1=0;
long long ans2=0;

struct Inst {
    enum { TOGGLE, TURN_ON, TURN_OFF } op;
    int x1, y1, x2, y2;
};
// istream in >> Inst
istream& operator>>(istream& in, Inst& inst)
{
    string s,n1,n2;
    in>>s;
    if (s == "toggle") inst.op = Inst::TOGGLE;
    else {
        in>>s;
        if (s == "on") inst.op = Inst::TURN_ON;
        else inst.op = Inst::TURN_OFF;
    }
    istream_ignore(in, ",");
    in>>inst.x1>>inst.y1>>s>>inst.x2>>inst.y2;
    istream_ignore(in, "");

    return in;
}
// ostream out << Inst
ostream& operator<<(ostream& out, const Inst& inst)
{
    switch (inst.op) {
        case Inst::TOGGLE: out << "toggle"; break;
        case Inst::TURN_ON: out << "turn on"; break;
        case Inst::TURN_OFF: out << "turn off"; break;
    }
    out << " " << inst.x1 << "," << inst.y1 << " through " << inst.x2 << "," << inst.y2;
    return out;
}

bool f(const vector<Inst>& Ins, int i, int j)
{
    bool ret = false;
    for (auto p = Ins.rbegin(); p != Ins.rend(); p++) {
        if (p->x1 <= i && i <= p->x2 && p->y1 <= j && j <= p->y2) {
            switch (p->op) {
            case Inst::TOGGLE: ret = !ret; break;
            case Inst::TURN_ON: return !ret;
            case Inst::TURN_OFF: return ret;
            }
        }
    }
    return ret;
}

long long g(const vector<Inst>& Ins, int i, int j)
{
    long long ret = 0;
    for (auto p = Ins.begin(); p != Ins.end(); p++) {
        if (p->x1 <= i && i <= p->x2 && p->y1 <= j && j <= p->y2) {
            switch (p->op) {
            case Inst::TOGGLE: ret += 2; break;
            case Inst::TURN_ON: ret++; break;
            case Inst::TURN_OFF: ret--; break;
            }
        }
        if (ret < 0) ret = 0;
    }
    return ret;
}
int main()
{
    Inst in;
    vector<Inst> ins;
    while(cin>>in) {
        ins.emplace_back(in);
    }

    // cout << ins << '\n';

    for (int i=0; i<=999; i++) {
        for (int j=0; j<=999; j++) {
            if (f(ins,i,j))
                ans1++;
            ans2 += g(ins, i, j);
        }
    }

    cout << std::format("{} {}\n", ans1, ans2);
    return 0;
}
