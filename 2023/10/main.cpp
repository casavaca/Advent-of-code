//usr/bin/c++ -g -std=c++20 -o ${o=`mktemp`} "$0" && "$o" "$@"; ret=$?; rm "$o"; exit $s

#include <numeric>
#include <iostream>
#include <vector>
#include <array>
#include <algorithm>
#include <map>
#include <sstream>
#include <cassert>

using namespace std;

// convert a line of numbers to vector<T>
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

vector<vector<char>> read_matrix(istream& is)
{
    vector<vector<char>> vec;
    string line;
    while (getline(is, line)) {
        vec.push_back(split_line<char>(line));
    }
    return vec;
}

long long ans1;
long long ans2;

int start_i, start_j;

void find_start(const vector<vector<char>>& input)
{
    int i,j;
    bool gotit = false;
    for (i=0; i<input.size(); i++) {
        for (j=0; j<input[0].size(); j++) {
            if (input[i][j] == 'S') {
                gotit = true;
                break;
            }
        }
        if (gotit)
            break;
    }
    assert(gotit);

    assert(input[i][j] == 'S');

    start_i = i;
    start_j = j;
}

int f(vector<vector<char>>& input, bool mark_with_X = false)
{
    vector<vector<char>> visited = input;
    for (auto& v:visited) std::fill(v.begin(), v.end(), false);
    int cnt = 0;

    int i = start_i;
    int j = start_j;
    vector<pair<int,int>> q {{i,j}};

    bool up=false;
    bool down=false;
    bool left=false;
    bool right=false;
    if (i>0 && string("7F|").find(input[i-1][j]) != string::npos) {
        up = true;
    }
    if (i < input.size() - 1 && string("JL|").find(input[i+1][j]) != string::npos) {
        down = true;
    }
    if (j>0 && string("F-L").find(input[i][j-1]) != string::npos) {
        left = true;
    }
    if (j < input[0].size()-1 && string("-7J").find(input[i][j+1]) != string::npos) {
        right = true;
    }
    {
        int dbg=0;
        if (up) dbg++;
        if (down) dbg++;
        if (left) dbg++;
        if (right) dbg++;
        assert(dbg == 2);
    }
    if (up&&left) input[i][j] = 'J';
    if (up&&right) input[i][j] = 'L';
    if (down&&left) input[i][j] = '7';
    if (down&&right) input[i][j] = 'F';
    if (down&&up) input[i][j] = '|';
    if (left&&right) input[i][j] = '-';
    assert(input[i][j] != 'S');
    // cout << "ss is " << i << ' ' << j << endl;

    while(q.size()) {
        cnt++;
        vector<pair<int,int>> qq;
        for(auto& p:q) {
            if (visited[p.first][p.second]) continue;
            vector<pair<int,int>> next;
            visited[p.first][p.second] = true;
            switch (input[p.first][p.second]) {
            case '|': {
                next = {{p.first-1, p.second}, {p.first+1, p.second}};
                break;
            }
            case '-': {
                next = {{p.first, p.second-1}, {p.first, p.second+1}};
                break;
            }
            case 'L': {
                next = {{p.first-1, p.second}, {p.first, p.second+1}};
                break;
            }
            case '7': {
                next = {{p.first+1, p.second}, {p.first, p.second-1}};
                break;
            }
            case 'F': {
                next = {{p.first+1, p.second}, {p.first, p.second+1}};
                break;
            }
            case 'J': {
                next = {{p.first-1, p.second}, {p.first, p.second-1}};
                break;
            }
            case 'X':
            case 'S':
            case '.': {
                break;
            }
            default: {
                assert(false);
            }
            }

            if (mark_with_X)
                input[p.first][p.second] = 'X';

            if (next.size()) {
                if (visited[next[0].first][next[0].second] &&
                    visited[next[1].first][next[1].second]) {
                    return cnt-1;
                }
                // if (!visited[next[0].first][next[0].second])
                //     cout << "reached " << next[0].first << ' ' << next[0].second << ' ' << cnt << endl;
                // if (!visited[next[1].first][next[1].second])
                //     cout << "reached " << next[1].first << ' ' << next[1].second << ' ' << cnt << endl;
            }
            qq.insert(qq.end(), next.begin(), next.end());
        }
        q = qq;
    }
    assert(false);
}

void tr(vector<vector<char>>& old)
{
    vector<vector<char>> ret(old.size() * 2 + 1, vector<char>(old[0].size() * 2 + 1));
    for (int i=0; i<ret.size(); i++) {
        for (int j=0; j<ret[0].size(); j++) {
            // border is '.'
            if (i == 0 || j == 0 || i == ret.size()-1 || j == ret[0].size()-1) {
                ret[i][j] = '.';
                continue;
            }
            // odd
            if (i%2 == 1 && j%2 == 1) {
                ret[i][j] = old[i/2][j/2];
                continue;
            }
            // odd even
            if (i%2 == 0 && j%2 == 1) {
                if (string("7F|").find(old[i/2-1][j/2]) != string::npos &&
                    string("JL|").find(old[i/2][j/2]) != string::npos)
                    ret[i][j] = '|';
                else
                    ret[i][j] = '.';
                continue;
            }
            // even odd
            if (i%2 == 1 && j%2 == 0) {
                if (string("LF-").find(old[i/2][j/2-1]) != string::npos &&
                    string("J-7").find(old[i/2][j/2]) != string::npos)
                    ret[i][j] = '-';
                else
                    ret[i][j] = '.';
                continue;
            }
            assert(i%2==0 && j%2==0);
            ret[i][j] = '.';
        }
    }
    old = ret;
}

void oo(vector<vector<char>>& input)
{
#if 0
    for (auto& s:input){
        cout << string(s.begin(), s.end()) << '\n';
    }
    cout << "-------------------------------------\n";
#endif
}


void dfs(vector<vector<char>>& input, int i, int j)
{
    vector<pair<int,int>> q{{i,j}};
    while (!q.empty()) {
        vector<pair<int,int>> qq;
        for(auto& p:q) {
            if (input[p.first][p.second] == ':')
                continue;
            // up down left right
            if (p.first > 0 && input[p.first-1][p.second] != 'X') {
                qq.emplace_back(p.first-1, p.second);
            }
            if (p.first < input.size()-1 && input[p.first+1][p.second] != 'X') {
                qq.emplace_back(p.first+1, p.second);
            }
            if (p.second > 0 && input[p.first][p.second-1] != 'X') {
                qq.emplace_back(p.first, p.second-1);
            }
            if (p.second < input[0].size()-1 && input[p.first][p.second+1] != 'X') {
                qq.emplace_back(p.first, p.second+1);
            }
            input[p.first][p.second] = ':';
        }
        q = qq;
    }
}

int count(const vector<vector<char>>& input)
{
    int cnt = 0;
    for (int i = 0; i < input.size(); i++) {
        for (int j = 0; j < input[0].size(); j++) {
            if (i%2 == 1 && j%2 == 1 && input[i][j] != ':' && input[i][j] != 'X')
                cnt++;
        }
    }
    return cnt;
}

int g(vector<vector<char>>& input)
{
    oo(input);
    tr(input);
    oo(input);
    start_i = start_i * 2 + 1;
    start_j = start_j * 2 + 1;
    f(input, true);
    oo(input);
    dfs(input, 0, 0);
    oo(input);
    return count(input);
}



int main()
{
    // vector<vector<char>> input;
    // std::string s;
    // while(cin>>s) {
    //     input.emplace_back(s.begin(), s.end());
    // }
    auto input = read_matrix(cin);
    // cout << input << endl;

    find_start(input);

    ans1 = f(input);
    ans2 = g(input);

    cout << ans1 << ' ' << ans2 << '\n';
    return 0;
}

