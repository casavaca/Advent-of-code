//usr/bin/c++ -g -std=c++20 -o ${o=`mktemp`} "$0" && "$o" "$@"; ret=$?; rm "$o"; exit $s

#include <iostream>
#include <ranges>
#include <regex>

using namespace std;

void part1(const std::vector<string>& v)
{
    long long ans = 0;
    for (auto& s : v) {
        auto digits = s | views::filter([](char c) { return isdigit(c); });
        ans += (digits.front() - '0') * 10 + digits.back() - '0';
    }
    cout << ans << '\n';
}

void part2(std::vector<string>& v)
{
    // the problem is ambiguous.
    // doesn't specify what to do with "twone" (two + one) "nineight" (night + eight)
    // Here let's transform "twone" as "21", which got the correct anwser.
    // A trick is used, one is replaced with "one1one", so that
    // other numbers that may overlap with one (e.g, two) can still be grepped.

    long long ans = 0;
    string s;

    // regex of one to nine
    const std::array<const std::regex, 9> regs = {
        regex("one"),
        regex("two"),
        regex("three"),
        regex("four"),
        regex("five"),
        regex("six"),
        regex("seven"),
        regex("eight"),
        regex("nine"),
    };

    const std::array<const char*, 9> targets = {
        ("one1one"),
        ("two2two"),
        ("three3three"),
        ("four4four"),
        ("five5five"),
        ("six6six"),
        ("seven7seven"),
        ("eight8eight"),
        ("nine9nine"),
    };

    for (auto &s:v) {
        // find substring in string
        for (unsigned i = 0; auto& r : regs) {
            s = std::regex_replace(s, r, targets[i]);
            ++i;
        }
        auto digits = s | views::filter([](char c) { return isdigit(c); });
        ans += (digits.front() - '0') * 10 + digits.back() - '0';
    }
    cout << ans << '\n';
}

int main()
{
    vector<string> v;
    string s;
    while(cin>>s) {
        v.emplace_back(std::move(s));
    }
    part1(v);
    part2(v);
    return 0;
}
