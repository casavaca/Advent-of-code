//usr/bin/c++ -g -std=c++20 -o ${o=`mktemp`} "$0" && "$o" "$@"; ret=$?; rm "$o"; exit $s

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <ranges>
#include <regex>

using namespace std;

/*
Game 1: 3 blue, 4 red; 1 red, 2 green, 6 blue; 2 green
Game 2: 1 blue, 2 green; 3 green, 4 blue, 1 red; 1 green, 1 blue
Game 3: 8 green, 6 blue, 20 red; 5 blue, 4 red, 13 green; 5 green, 1 red
Game 4: 1 green, 3 red, 6 blue; 3 green, 6 red; 3 green, 15 blue, 14 red
Game 5: 6 red, 1 blue, 3 green; 2 blue, 1 red, 2 green
*/

pair<long long, long long> f(string& s, int i) {
    // 12 red cubes, 13 green cubes, and 14 blue cubes
    for (auto& c:s) if(c==',') c = ' ';
    for (auto& c:s) if(c==';') c = ' ';
    stringstream ss;
    ss<<s;
    string tmp[10];
    ss>>tmp[0]>>tmp[1];
    int limit [] = {0,0,0};
    while(true) {
        ss >> tmp[0] >> tmp[1];
        int num = std::stoi(tmp[0]);
        int idx = 0;
        if (tmp[1] == "blue") {
            idx = 2;
        } else if (tmp[1] == "green") {
            idx = 1;
        } else if (tmp[1] == "red") {
            idx = 0;
        } else {
            idx = -1;
        }

        if (idx >= 0 && num > limit[idx]) {
            limit[idx] = num;
        }
        if (ss.eof())
            break;
    }
    return {((limit[0] <= 12 && limit[1] <= 13 && limit[2] <= 14) ? i+1 : 0), limit[0] * limit[1] * limit[2]};
}

int main()
{
    vector<string> v;
    string s;
    long long ans1=0;
    long long ans2=0;
    while(getline(cin, s)) {
        v.push_back(s);
    }
    for (auto i=0u; i<v.size(); i++) {
        auto p = f(v[i], i);
        ans1 += p.first;
        ans2 += p.second;
    }
    cout << ans1 << ' ' << ans2 << '\n';
    return 0;
}
