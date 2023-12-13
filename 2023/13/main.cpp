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

long long ans1;
long long ans2;

vector<vector<string>> in;

void transpose_string_matrix(vector<string> & v)
{
    vector<string> ret(v[0].size(), string(v.size(), ' '));
    for (int i = 0; i < v.size(); i++) {
        for (int j = 0; j < v[0].size(); j++) {
            ret[j][i] = v[i][j];
        }
    }
    v = ret;
}

bool find_row_1(vector<string>& mat, int r)
{
    int i = r-1;
    int j = r;
    while(i >= 0 && j < mat.size()) {
        if (mat[i] != mat[j])
            return false;
        i--;
        j++;
    }
    return true;
}

bool find_row_2(vector<string>& mat, int r)
{
    int diff = 0;
    int i = r-1;
    int j = r;
    while(i >= 0 && j < mat.size()) {
        if (mat[i] != mat[j]) {
            if (diff)
                return false;
            for (int s=0; s<mat[i].size(); s++) {
                if (mat[i][s] != mat[j][s]) diff++;
            }
            if (diff > 1)
                return false;
        }
        i--;
        j++;
    }
    return diff == 1;
}

long long find_row(vector<string>& mat, int i)
{
    for (int r=1; r<mat.size(); r++) {
        if (i == 1 && find_row_1(mat, r))
            return r;
        if (i == 2 && find_row_2(mat, r))
            return r;
    }
    return 0;
}

long long f(vector<string>& mat, int i)
{
    long long ret = find_row(mat, i);
    if (ret)
        return ret * 100;
    transpose_string_matrix(mat);
    ret = find_row(mat, i);
    transpose_string_matrix(mat);
    assert(ret);
    return ret;
}

int main()
{
    string s;
    vector<string> mat;
    while(getline(cin, s)) {
        if(s == "") {
            in.push_back(mat);
            mat.clear();
            continue;
        }
        mat.push_back(s);
    }
    in.push_back(mat);

    for (auto& i:in) {
        ans1 += f(i,1);
        ans2 += f(i,2);
    }

    cout << ans1 << ' ' << ans2 << '\n';
    return 0;
}

