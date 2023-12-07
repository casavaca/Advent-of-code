//usr/bin/c++ -g -std=c++17 -o ${o=`mktemp`} "$0" && sed "s.x. .g" | "$o"; ret=$?; rm "$o"; exit $s

#include <functional>
#include <limits>
#include <numeric>
#include <string>
#include <set>
#include <numeric>
#include <iostream>
using namespace std;

long long ans1=0;
long long ans2=0;

int main()
{
    string s;
    cin>>s;

    set<unsigned> visited; // use x << 16 + y. The input is smaller then this.
    unsigned x=10000, y = 10000;
    visited.insert((x << 16) + y);
    for (auto c:s) {
        switch (c) {
        case '>': x++; break;
        case '<': x--; break;
        case 'v': y++; break;
        case '^': y--; break;
        }
        visited.insert((x << 16) + y);
    }


    set<unsigned> visited2; // use x << 16 + y. The input is smaller then this.
    unsigned xs[2] = {10000, 10000};
    unsigned ys[2] = {10000, 10000};
    bool toggle = true;
    visited2.insert((xs[0] << 16) + ys[0]);
    for (auto c:s) {
        switch (c) {
        case '>': xs[toggle?1:0]++; break;
        case '<': xs[toggle?1:0]--; break;
        case 'v': ys[toggle?1:0]++; break;
        case '^': ys[toggle?1:0]--; break;
        }
        visited2.insert((xs[0] << 16) + ys[0]);
        visited2.insert((xs[1] << 16) + ys[1]);
        toggle = !toggle;
    }

    ans1 = visited.size();
    ans2 = visited2.size();
    cout << ans1 << ' ' << ans2 << '\n';
    return 0;
}
