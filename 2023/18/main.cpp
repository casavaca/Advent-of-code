//usr/bin/c++ -g -std=c++20 -O3 -o ${o=`mktemp`} "$0" && "$o" "$@"; ret=$?; rm "$o"; exit $s

#include <numeric>
#include <iostream>
#include <vector>
#include <deque>
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

enum direction {U, R, D, L};
struct S {
    direction d;
    int len;
    string color;
};

void convert_color_to_instruction(S& s)
{
    switch(*(s.color.rbegin() + 1)) {
    case '0': s.d = R; break;
    case '1': s.d = D; break;
    case '2': s.d = L; break;
    case '3': s.d = U; break;
    default: assert(false);
    }
    s.len = std::stoi(s.color.substr(2, 5), NULL, 16);
}

void oo(vector<string>& input)
{
    // for (auto& s:input){
    //     cout << s << '\n';
    // }
    // cout << "-------------------------------------\n";
}

struct {
    set<long long> SLR{1};
    set<long long> SUD{1};
    map<long long, int> MLR;
    map<long long, int> MUD;

    map<int, long long> RMLR;
    map<int, long long> RMUD;

    map<int, long long> VMLR;
    map<int, long long> VMUD;

} g_context;

void clear_g_context()
{
    g_context.SLR.clear();
    g_context.SUD.clear();
    g_context.MLR.clear();
    g_context.MUD.clear();
    g_context.RMLR.clear();
    g_context.RMUD.clear();
    g_context.VMLR.clear();
    g_context.VMUD.clear();
}

void discretize(vector<S>& in)
{
    set<long long>      &SLR = g_context.SLR;
    set<long long>      &SUD = g_context.SUD;
    map<long long, int> &MLR = g_context.MLR;
    map<long long, int> &MUD = g_context.MUD;

    map<int, long long> &RMLR = g_context.RMLR;
    map<int, long long> &RMUD = g_context.RMUD;
    map<int, long long> &VMLR = g_context.VMLR;
    map<int, long long> &VMUD = g_context.VMUD;

    long long LR = 1, UD = 1;
    for (auto &s:in) {
        switch (s.d) {
        case R: LR += s.len; break;
        case L: LR -= s.len; break;
        case D: UD += s.len; break;
        case U: UD -= s.len; break;
        }
        SLR.insert(LR);
        SUD.insert(UD);
    }
    int LRcount = 0;
    int UDcount = 0;
    vector<long long> local_SLR(SLR.begin(), SLR.end());
    for (int i=0; i<local_SLR.size(); ++i) {
        long long pos = local_SLR[i];
        if ((i==0) || (local_SLR[i] == local_SLR[i-1] + 1)) {
            ++LRcount;
            MLR[pos] = LRcount;
            RMLR[LRcount] = pos;
        } else {
            ++LRcount;
            MLR[pos-1] = LRcount;
            RMLR[LRcount] = pos-1;
            VMLR[LRcount] = local_SLR[i] - local_SLR[i-1] - 1;
            ++LRcount;
            MLR[pos] = LRcount;
            RMLR[LRcount] = pos;
        }
    }
    vector<long long> local_SUD(SUD.begin(), SUD.end());
    for (int i=0; i<local_SUD.size(); ++i) {
        long long pos = local_SUD[i];
        if ((i==0) || (local_SUD[i] == local_SUD[i-1] + 1)) {
            ++UDcount;
            MUD[pos] = UDcount;
            RMUD[UDcount] = pos;
        } else {
            ++UDcount;
            MUD[pos-1] = UDcount;
            RMUD[UDcount] = pos-1;
            VMUD[UDcount] = local_SUD[i] - local_SUD[i-1] - 1;
            ++UDcount;
            MUD[pos] = UDcount;
            RMUD[UDcount] = pos;
        }
    }

    // cout << "MLR is  \n" << MLR << "--------------------\n";
    // cout << "RMLR is \n" << RMLR << "--------------------\n";
    // cout << "VMLR is \n" << VMLR << "--------------------\n";

    // cout << "MUD is  \n" << MUD << "--------------------\n";
    // cout << "RMUD is \n" << RMUD << "--------------------\n";
    // cout << "VMUD is \n" << VMUD << "--------------------\n";


    long long i = 1, j = 1;
    for (auto &s:in) {
        long long oldi = i;
        long long oldj = j;
        switch (s.d) {
        case R: j += s.len; s.len = MLR.at(oldj + s.len) - MLR.at(oldj); break;
        case L: j -= s.len; s.len = MLR.at(oldj) - MLR.at(oldj - s.len); break;
        case D: i += s.len; s.len = MUD.at(oldi + s.len) - MUD.at(oldi); break;
        case U: i -= s.len; s.len = MUD.at(oldi) - MUD.at(oldi - s.len); break;
        }
    }
}

void dfs(vector<string>& input, int i, int j)
{
    vector<pair<int,int>> q{{i,j}};
    while (!q.empty()) {
        vector<pair<int,int>> qq;
        for(auto& p:q) {
            if (input[p.first][p.second] == ':')
                continue;
            // up down left right
            if (p.first > 0 && input[p.first-1][p.second] != '#') {
                qq.emplace_back(p.first-1, p.second);
            }
            if (p.first < input.size()-1 && input[p.first+1][p.second] != '#') {
                qq.emplace_back(p.first+1, p.second);
            }
            if (p.second > 0 && input[p.first][p.second-1] != '#') {
                qq.emplace_back(p.first, p.second-1);
            }
            if (p.second < input[0].size()-1 && input[p.first][p.second+1] != '#') {
                qq.emplace_back(p.first, p.second+1);
            }
            input[p.first][p.second] = ':';
        }
        q = qq;
    }
}

long long count(const vector<string>& input)
{
    long long ans = 0;
    for (int i = 0; i < input.size(); i++) {
        for (int j = 0; j < input[0].size(); j++) {
            if (input[i][j] == '.' || input[i][j] == '#') {
                long long imul = 1;
                long long jmul = 1;
                if (g_context.VMLR.count(j))
                    jmul = g_context.VMLR[j];
                if (g_context.VMUD.count(i))
                    imul = g_context.VMUD[i];
                ans += imul * jmul;
            }
        }
    }
    return ans;
}

vector<string> doit(const vector<S> &in)
{
    map<long long, int> &MLR = g_context.MLR;
    map<long long, int> &MUD = g_context.MUD;

    vector<string> mat(MUD.size()+2, string(MLR.size()+2, '.'));
    int i = MUD[1];
    int j = MLR[1];
    mat.at(i).at(j) = '#';
    oo(mat);
    for (const auto& s:in) {
        int dir[2] = {0,0};
        switch (s.d) {
        case U: dir[0] = -1; break;
        case D: dir[0] = 1; break;
        case L: dir[1] = -1; break;
        case R: dir[1] = 1; break;
        }
        for (int step = 0; step < s.len; step++) {
            i += dir[0];
            j += dir[1];
            mat.at(i).at(j) = '#';
        }
    }
    oo(mat);
    dfs(mat, 0, 0);
    oo(mat);
    return mat;
}

long long g(vector<S> in)
{
    clear_g_context();
    for (auto& s:in)
        convert_color_to_instruction(s);

    discretize(in);
    vector<string> mat = doit(in);
    return count(mat);
}

long long f(vector<S> in)
{
    clear_g_context();
    discretize(in);
    vector<string> mat = doit(in);
    return count(mat);
}

int main()
{
    vector<S> in;
    char c; int i; string s;
    while(cin >> c >> i >> s) {
        switch (c) {
        case 'U': in.push_back({U, i, s}); break;
        case 'R': in.push_back({R, i, s}); break;
        case 'D': in.push_back({D, i, s}); break;
        case 'L': in.push_back({L, i, s}); break;
        default: assert(false);
        }
    }
    ans1 = f(in);
    ans2 = g(in);
    cout << ans1 << ' ' << ans2 << '\n';
    return 0;
}
