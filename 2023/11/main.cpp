//usr/bin/c++ -g -std=c++20 -O3 -o ${o=`mktemp`} "$0" && "$o" "$@"; ret=$?; rm "$o"; exit $s

#include <numeric>
#include <iostream>
#include <vector>
#include <array>
#include <algorithm>
#include <map>
#include <sstream>
#include <cassert>
using namespace std;

template <typename T>
vector<T> split_line(const string& line)
{
    vector<T> vec;
    istringstream is(line);
    T x;
    while (is >> x)
        vec.push_back(x);
    return vec;
}

// cin >> tuple<T...>
template <typename... T>
istream& operator>>(istream& is, tuple<T...>& t)
{
    apply([&is](auto&... args) { (is >> ... >> args); }, t);
    return is;
}

// cin >> pair<T1, T2>
template <typename T1, typename T2>
istream& operator>>(istream& is, pair<T1, T2>& p)
{
    is >> p.first >> p.second;
    return is;
}

// cin >> vector<T>
template <typename T>
istream& operator>>(istream& is, vector<T>& vec)
{
    for (T& x : vec)
        is >> x;
    return is;
}

// cin >> array<T, N>
template <typename T, size_t N>
istream& operator>>(istream& is, array<T, N>& arr)
{
    for (T& x : arr)
        is >> x;
    return is;
}

////////////////////////////////////////////////////////////////////////////////
// OUTPUT
////////////////////////////////////////////////////////////////////////////////

// cout << tuple<T...>
template <typename... T>
ostream& operator<<(ostream& os, const tuple<T...>& t)
{
    apply([&os](const auto&... args) { (os << ... << args); }, t);
    return os;
}

// cout << pair<T1, T2>
template <typename T1, typename T2>
ostream& operator<<(ostream& os, const pair<T1, T2>& p)
{
    os << p.first << " " << p.second;
    return os;
}

// cout << vector<T>
template <typename T>
ostream& operator<<(ostream& os, const vector<T>& vec)
{
    for (const T& x : vec)
        os << x << " ";
    return os;
}

vector<vector<char>> read_matrix(istream& is)
{
    vector<vector<char>> vec;
    string line;
    while (getline(is, line)) {
        vec.push_back(split_line<char>(line));
    }
    return vec;
}

void output_matrix(vector<vector<char>> & v)
{
    for (auto &vc:v) {
        cout << string(vc.begin(), vc.end()) << endl;
    }
}

void transpose_matrix(vector<vector<char>> & v)
{
    vector<vector<char>> ret(v[0].size(), vector<char>(v.size()));
    for (int i = 0; i < v.size(); i++) {
        for (int j = 0; j < v[0].size(); j++) {
            ret[j][i] = v[i][j];
        }
    }
    v = ret;
}


long long ans1;
long long ans2;

vector<int> expand_row(vector<vector<char>> &input) {
    int n = input.size();
    int m = input[0].size();
    vector<int> dup;
    for (int i = 0; i < n; i++) {
        // all is '.'
        if (std::all_of(input[i].begin(), input[i].end(), [](char x) { return x == '.'; })) {
            dup.push_back(i);
        }
    }
    return dup;
}
vector<int> expand_col(vector<vector<char>> &input) {
    transpose_matrix(input);
    auto ret = expand_row(input);
    transpose_matrix(input);
    return ret;
}

vector<int> dot_rows;
vector<int> dot_cols;

void expand(vector<vector<char>> &input) {
    dot_rows = expand_row(input);
    dot_cols = expand_col(input);
}

vector<pair<int,int>> find_pos(const vector<vector<char>>& input)
{
    vector<pair<int,int>> ret;
    for (int i = 0; i < input.size(); i++) {
        for (int j = 0; j < input[i].size(); j++) {
            if (input[i][j] == '#') {
                ret.push_back({i, j});
            }
        }
    }
    return ret;
}

long long shortest_path(vector<pair<int,int>> pos, const vector<vector<char>> &input, long long dupsize)
{
    long long ans = 0;
    for (int i=0; i<pos.size(); i++) {
        for (int j=i+1; j<pos.size(); j++) {
            ans += abs(pos[i].first - pos[j].first) + abs(pos[i].second - pos[j].second);
            int minx = min(pos[i].first, pos[j].first);
            int maxx = max(pos[i].first, pos[j].first);
            int miny = min(pos[i].second, pos[j].second);
            int maxy = max(pos[i].second, pos[j].second);

            for (int x=minx; x<=maxx; x++) {
                if (std::find(dot_rows.begin(), dot_rows.end(), x) != dot_rows.end()) {
                    ans += dupsize-1;
                }
            }
            for (int y=miny; y<=maxy; y++) {
                if (std::find(dot_cols.begin(), dot_cols.end(), y) != dot_cols.end()) {
                    ans += dupsize-1;
                }
            }
        }
    }
    return ans;
}

int main()
{
    // vector<vector<char>> input;
    // std::string s;
    // while(cin>>s) {
    //     input.emplace_back(s.begin(), s.end());
    // }
    auto input = read_matrix(cin);
    // output_matrix(input);
    expand(input);
    // output_matrix(input);

    // cout << dot_rows << endl;
    // cout << dot_cols << endl;

    vector<pair<int,int>> pos = find_pos(input);
    ans1 = shortest_path(pos, input, 2);
    ans2 = shortest_path(pos, input, 1000000);

    cout << ans1 << ' ' << ans2 << '\n';
    return 0;
}

