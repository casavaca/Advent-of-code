//usr/bin/c++ -g -std=c++17 -o ${o=`mktemp`} "$0" && "$o" "$@"; ret=$?; rm "$o"; exit $s

#include <numeric>
#include <iostream>
#include <vector>
using namespace std;

long long f(long long tt, long long dd)
{
    long long cnt=0;
    for (int i=0; i<tt; i++) {
        if (i * (tt-i) > dd)
            cnt++;
    }
    return cnt;
}

long long doit(std::vector<long long> tt, std::vector<long long> dd)
{
    vector<long long> ans(tt.size(), 0);
    for (auto i=0u; i<tt.size(); i++) {
        ans[i] = f(tt[i], dd[i]);
    }
    return std::reduce(ans.begin(), ans.end(), 1, std::multiplies<long long>());
}

int main()
{
    long long ans1=doit({41,66,72,66}, {244,1047,1228,1040});
    long long ans2=doit({41667266}, {244104712281040});

    cout << ans1 << ' ' << ans2 << '\n';

    return 0;
}

