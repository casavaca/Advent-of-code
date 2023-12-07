//usr/bin/c++ -g -std=c++17 -o ${o=`mktemp`} "$0" && sed "s.x. .g" | "$o"; ret=$?; rm "$o"; exit $s

#include <functional>
#include <limits>
#include <numeric>
#include <string>
#include <numeric>
#include <iostream>
using namespace std;

long long ans1=0;
long long ans2=0;

int main()
{
    long long a,b,c;
    // Note: x in input is replaced by sed, see shebang.
    while(cin>>a>>b>>c) {
        std::array<long long, 3> tmp{a*b, b*c, c*a};
        ans1 += *std::min_element(tmp.begin(), tmp.end());
        ans1 += 2*reduce(tmp.begin(), tmp.end());
        
        std::array<long long, 3> tmp2{a, b, c};
        ans2 += 2*reduce(tmp2.begin(), tmp2.end()) - 2 * *std::max_element(tmp2.begin(), tmp2.end());
        ans2 += reduce(tmp2.begin(), tmp2.end(), 1, std::multiplies<>());
    }

    cout << ans1 << ' ' << ans2 << '\n';

    return 0;
}
