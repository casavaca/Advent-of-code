//usr/bin/c++ -g -std=c++17 -o ${o=`mktemp`} "$0" && "$o" "$@"; ret=$?; rm "$o"; exit $s

#include <numeric>
#include <iostream>
#include <vector>
#include "../template/header.hpp"

using namespace std;

long long ans1;
long long ans2;

enum CardType {
    HIGH_CARD,
    ONE_PAIR,
    TWO_PAIR,
    THREE_OF_A_KIND,
    FULL_HOUSE,
    FOUR_OF_A_KIND,
    FIVE_OF_A_KIND,
};


struct Card {
    std::array<char, 5> hands;
    long long weight;
};

istream& operator>>(istream& is, Card& card)
{
    for (int i=0; i<5; i++) is >> card.hands[i];
    is>>card.weight;
    return is;
}

ostream& operator<<(ostream& os, const Card& card)
{
    for (int i=0; i<5; i++) os << card.hands[i] << ' ';
    os<<card.weight<<'\n';
    return os;
}

CardType get_type(const Card& card)
{
    map<char, int> m;
    for (auto c:card.hands) m[c]++;
    if (m.size() == 1) return FIVE_OF_A_KIND;
    if (m.size() == 2) {
        if (m.begin()->second == 4 || m.rbegin()->second == 4) return FOUR_OF_A_KIND;
        if (m.begin()->second == 3 || m.rbegin()->second == 3) return FULL_HOUSE;
        assert(false);
    }
    if (m.size() == 3) {
        for (auto p:m) if (p.second == 3) return THREE_OF_A_KIND;
        for (auto p:m) assert (p.second == 2 || p.second == 1);
        return TWO_PAIR;
    }
    if (m.size() == 4) return ONE_PAIR;
    if (m.size() == 5) return HIGH_CARD;
    assert(false);
}


bool operator<(const Card& lhs, const Card& rhs) {
    CardType l = get_type(lhs);
    CardType r = get_type(rhs);
    if (l<r) return true;
    if (l>r) return false;
    static char ww[128];
    {
        ww['2'] = 2;
        ww['3'] = 3;
        ww['4'] = 4;
        ww['5'] = 5;
        ww['6'] = 6;
        ww['7'] = 7;
        ww['8'] = 8;
        ww['9'] = 9;
        ww['T'] = 10;
        ww['J'] = 11;
        ww['Q'] = 12;
        ww['K'] = 13;
        ww['A'] = 14;
    };
    for (int i=0; i<5; i++) {
        if (ww[lhs.hands[i]] < ww[rhs.hands[i]]) return true;
        if (ww[lhs.hands[i]] > ww[rhs.hands[i]]) return false;
    }
    return true;
}


int main()
{
    vector<Card> vc;
    Card c;
    while(cin>>c) {
        vc.push_back(c);
    }

    // cout << vc << '\n';
    sort(vc.begin(), vc.end());
    // cout << vc << '\n';

    for (int i=0; i<vc.size(); i++) {
        ans1 += vc[i].weight * (i+1);
    }

    cout << ans1 << ' ' << ans2 << '\n';
    return 0;
}

