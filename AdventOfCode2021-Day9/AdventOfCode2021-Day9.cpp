/**
 * AdventOfCode2021-Day9.cpp : Advent of Code 2021, Day 9 (parts 1 and 2)
 *
 * NOTE: The solutions for 2021 aren't as cleaned up and well documented as other
 * years I've done.
 *
 * @author	Tim Behrendsen
 */

#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include <bitset>
#include <map>
#include <set>
#include <unordered_set>

using namespace std;

std::vector<std::string> str_split(const std::string& s, const char* pattern) {
    std::regex sep_regex = std::regex{ pattern };
    std::sregex_token_iterator iter(s.begin(), s.end(), sep_regex, -1);
    std::sregex_token_iterator end;
    return { iter, end };
}

int sgn(int val) {
    return (0 < val) - (val < 0);
}

char getDir(const vector<string>& area, int r, int c, int r_dir, int c_dir)
{
    r += r_dir;
    c += c_dir;

    if (r < 0 || c < 0 || r >= area.size() || c >= area[0].length()) {
        return '9' + 1;
    }

    return area[r][c];
}

int calc(const vector<string>& area, bool (& visited)[100][100], int r, int c)
{
    if (r < 0 || c < 0 || r >= area.size() || c >= area[0].length()) {
        return 0;
    }

    char h = area[r][c];
    if (visited[r][c] || h >= '9') {
        return 0;
    }
    visited[r][c] = true;

    int n = 1 +
        calc(area, visited, r + 1, c) +
        calc(area, visited, r - 1, c) +
        calc(area, visited, r, c + 1) +
        calc(area, visited, r, c - 1);

    return n;
}

int calcBasin(const vector<string>& area, int r, int c) {
    bool visited[100][100] = { 0 };
    return calc(area, visited, r, c);
}

void step2()
{
    ifstream fh("heights.txt");
    string s;
    vector<string> area;

    while (getline(fh, s)) {
        cout << s << endl;
        area.push_back(s);
    }

    int cols = area[0].length();
    int rows = area.size();

    vector<pair<int, int>> lowPts;

    int riskSum = 0;
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            char h = area[r][c];

            char up = getDir(area, r, c, -1, 0);
            char down = getDir(area, r, c, +1, 0);
            char left = getDir(area, r, c, 0, -1);
            char right = getDir(area, r, c, 0, 1);

            if (h < up && h < down && h < left && h < right) {
                int risk = (h - '0') + 1;
                cout << "LOW: [" << r << ", " << c << "] (" << risk << ")" << endl;
                riskSum += risk;
                lowPts.push_back({ r, c });
            }
        }
    }

    cout << "SUM: " << riskSum << endl;

    vector<int> sizes;
    for (auto c : lowPts) {
        int size = calcBasin(area, c.first, c.second);
        cout << "PT: [" << c.first << ", " << c.second << "], size is " << size << endl;
        sizes.push_back(size);
    }

    sort(sizes.begin(), sizes.end());
    for (auto n : sizes) {
        cout << n << ", ";
    }
    cout << endl;

    int n = sizes.size();
    int final = sizes[n - 1] * sizes[n - 2] * sizes[n - 3];
    cout << "FINAL: " << final << endl;


}

void step1()
{
    ifstream fh("heights.txt");
    string s;
    vector<string> area;

    while (getline(fh, s)) {
        cout << s << endl;
        area.push_back(s);
    }

    int cols = area[0].length();
    int rows = area.size();

    int riskSum = 0;
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            char h = area[r][c];

            char up = getDir(area, r, c, -1, 0);
            char down = getDir(area, r, c, +1, 0);
            char left = getDir(area, r, c, 0, -1);
            char right = getDir(area, r, c, 0, 1);

            if (h < up && h < down && h < left && h < right) {
                int risk = (h - '0') + 1;
                cout << "LOW: [" << r << ", " << c << "] (" << risk << ")" << endl;
                riskSum += risk;
            }
        }
    }

    cout << "SUM: " << riskSum << endl;
}

int main()
{
    step2();
}
