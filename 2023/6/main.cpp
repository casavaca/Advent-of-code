//usr/bin/c++ -g -std=c++17 -o ${o=`mktemp`} "$0" && "$o" "$@"; ret=$?; rm "$o"; exit $s

#include "../template/header.hpp"

long long f(int tt, int dd)
{
    long long cnt=0;
    for (int i=0; i<tt; i++) {
        if (i * (tt-i) > dd)
            cnt++;
    }
    return cnt;
}

constexpr int N = 4;

int main()
{
    // std::array<int, 3> tt = {7, 15, 30};
    // std::array<int, 3> dd = {9, 40, 200};
    // std::array<long long, 3> ans = {0,0,0};

    std::array<int, N> tt = {41, 66, 72, 66};
    std::array<int, N> dd = {244,1047,1228,1040};
    std::array<long long, N> ans = {0,0,0};


    long long ans1=0;
    long long ans2=0;

    for (int i=0; i<N; i++) {
        ans[i] = f(tt[i], dd[i]);
    }
    ans1 = ans[0] * ans[1] * ans[2] * ans[3];

    cout << ans1 << ' ' << ans2 << '\n';

    return 0;
}

