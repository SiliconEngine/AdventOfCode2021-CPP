/**
 * AdventOfCode2021-Day7.cpp : Advent of Code 2021, Day 7 (parts 1 and 2)
 *
 * NOTE: The solutions for 2021 aren't as cleaned up and well documented as other
 * years I've done.
 *
 * @author	Tim Behrendsen
 */

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include <bitset>

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

void step2()
{
    ifstream fh("whale.txt");
    string s;

    getline(fh, s);
    cout << s << endl;

    vector<string> list = str_split(s, ",");
    vector<int> crabs;
    int maxPos = 0;
    for (auto f : list) {
        int pos = stoi(f);
        crabs.push_back(pos);
        if (pos > maxPos)
            maxPos = pos;
    }

    int best = -1;
    for (int i = 0; i < maxPos; ++i) {
        int total = 0;
        for (auto c : crabs) {
            int n = abs(c - i);
            for (int amt = 1; amt <= n; ++amt) {
				total += amt;
			}
        }
		if (best < 0 || total < best) {
			best = total;
		}
        cout << "Pos " << i << " costs " << total << " (Best is " << best << ")" << endl;
    }

    cout << "best is " << best;
}

void step1()
{
    ifstream fh("whale.txt");
    string s;

    getline(fh, s);
    cout << s << endl;

    vector<string> list = str_split(s, ",");
    vector<int> crabs;
    int maxPos = 0;
    for (auto f : list) {
        int pos = stoi(f);
        crabs.push_back(pos);
        if (pos > maxPos)
            maxPos = pos;
    }

    int best = -1;
    for (int i = 0; i < maxPos; ++i) {
        int total = 0;
        for (auto c : crabs) {
            total += abs(c - i);
        }
		if (best < 0 || total < best) {
			best = total;
		}
        cout << "Pos " << i << " costs " << total << " (Best is " << best << ")" << endl;
    }

    cout << "best is " << best;
}

int main()
{
    step2();
}
