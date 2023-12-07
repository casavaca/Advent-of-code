//usr/bin/c++ -g -std=c++17 -o ${o=`mktemp`} "$0" && "$o" "$@"; ret=$?; rm "$o"; exit $s

#include <limits>
#include <string>
#include <iostream>
using namespace std;

int main()
{
    string s;
    cin>>s;
    long long ans1=0;
    long long ans2=0;
    bool gotit = false;
    int i=0;
    for (auto c:s) {
        i++;
        if (c=='(') ans1++;
        else if (c==')') ans1--;
        if (ans1 < 0 && !gotit) {
            gotit = true;
            ans2 = i;
        }
    }

    cout << ans1 << ' ' << ans2 << '\n';

    return 0;
}
