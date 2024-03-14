/**
 * AdventOfCode2021-Day10.cpp : Advent of Code 2021, Day 10 (parts 1 and 2)
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
#include <stack>
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

long long autocomplete(const string& line)
{
    stack<char> s;
    map<char, int> scores;
    scores[')'] = 1;
    scores[']'] = 2;
    scores['}'] = 3;
    scores['>'] = 4;

    auto cvt = [](char c) {
        switch (c) {
        case '(': c = ')'; break;
        case '{': c = '}'; break;
        case '[': c = ']'; break;
        case '<': c = '>'; break;
        }
        return c;
    };

    for (char c : line) {
        //cout << "    " << c << ": ";
        if (c == '(' || c == '{' || c == '[' || c == '<') {
            s.push(c);
            //cout << "Push" << endl;
        }
        else {
            if (s.empty()) {
                cout << "ILLEGAL EMPTY: " << line << endl;
                exit(0);
            }
            char match = s.top();
            s.pop();

            char expect = cvt(match);
            //cout << "Pop '" << match << "', expect = '" << expect << "', c = " << c << endl;

            if (c != expect) {
                cout << "    Corrupt" << endl;
                return 0;
            }
        }
    }

    string a = "";
    while (!s.empty()) {
        char c = s.top();
        a += cvt(c);
        s.pop();
    }

    cout << "    Auto complete is: " << a << endl;
    long long score = 0;
    for (char c : a) {
        score = score * 5 + scores[c];
    }
    if (score < 0) {
        cout << "NEGATIVE " << score << endl;
        exit(0);
	}

    return score;
}

void step2()
{
    ifstream fh("program.txt");
    string s;

    vector<long long> scoreList;
    while (getline(fh, s)) {
        cout << s << endl;
        long long score = autocomplete(s);
        cout << "    Score was " << score << endl;

        if (score != 0)
            scoreList.push_back(score);
    }

    sort(scoreList.begin(), scoreList.end());

    for (auto n : scoreList) {
        cout << n << endl;
    }

    cout << "MIDDLE: " << scoreList[scoreList.size() / 2] << endl;

}

int getScore(const string &line)
{
    stack<char> s;
    map<char, int> scores;
    scores[')'] = 3;
    scores[']'] = 57;
    scores['}'] = 1197;
    scores['>'] = 25137;

    for (char c : line) {
        //cout << "    " << c << ": ";
        if (c == '(' || c == '{' || c == '[' || c == '<') {
            s.push(c);
            //cout << "Push" << endl;
        } else {
            if (s.empty()) {
                cout << "ILLEGAL EMPTY: " << line << endl;
                exit(0);
            }
            char match = s.top();
            s.pop();

            char expect = 0;
            switch (match) {
            case '(': expect = ')'; break;
            case '{': expect = '}'; break;
            case '[': expect = ']'; break;
            case '<': expect = '>'; break;
            }
            //cout << "Pop '" << match << "', expect = '" << expect << "', c = " << c << endl;

            if (c != expect) {
                return scores[c];
            }
        }
    }

    return 0;
}

void step1()
{
    ifstream fh("program.txt");
    string s;

    int total = 0;
    while (getline(fh, s)) {
        cout << s << endl;
        int score = getScore(s);
        total += score;

        cout << "    Score was " << score << endl;
        //exit(0);
    }
	cout << "    Total score was " << total << endl;
}

int main()
{
    step2();
}
