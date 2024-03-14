/**
 * AdventOfCode2021-Day13.cpp : Advent of Code 2021, Day 13 (parts 1 and 2)
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
#include <queue>

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

#define YELLOW "\x1b[33m"
#define NORMAL "\x1b[0m"

#define FILE_NAME "paper.txt"
#define DEBUG 0

int max_x = 0, max_y = 0;
vector<pair<int, int>> coords;
vector<pair<char, int>> folds;
vector<string> paper;

void dspPaper(vector<string> &paper)
{
    for (auto s : paper) {
        cout << s << endl;
    }
}

void readPaper(string fn)
{
    string s;
    ifstream fh(fn);

    while (getline(fh, s)) {
        cout << s << endl;
        if (s == "")
            break;

        auto p = str_split(s, ",");
        int x = stoi(p[0]);
        int y = stoi(p[1]);
        if (x > max_x) max_x = x;
        if (y > max_y) max_y = y;
        coords.push_back({ x, y });
    }

    while (getline(fh, s)) {
        cout << s << endl;
        auto p = str_split(s, "=");
        char axis = p[0][p[0].length() - 1];

        folds.push_back({ axis, stoi(p[1]) });
    }

    s = string(max_x + 1, '.');
    for (int y = 0; y <= max_y; ++y) {
        paper.push_back(s);
    }

    for (auto c : coords) {
        paper[c.second][c.first] = '#';
    }
}

vector<string> foldY(vector<string> &paper, int pos) {
    cout << "Folding Y at " << pos << endl;
    vector<string> newP;
    int max_x = paper[0].length() - 1;
    int max_y = paper.size() - 1;
    cout << "foldY: max_x = " << max_x << ", max_y = " << max_y << endl;

    int topSize = pos;
    int bottomSize = paper.size() - pos - 1;
    cout << "topSize = " << topSize << ", bottomSize = " << bottomSize << endl;
    int newSize = max(topSize, bottomSize);

    // Create blank paper correct size
    string s = string(max_x + 1, '.');
    for (int i = 0; i < newSize; ++i) {
        newP.push_back(s);
    }

    int newpos = newSize-1;
    int topPos = pos, botPos = pos;
	bool didLine = false;
    do {
        didLine = false;

        --topPos;
        if (topPos >= 0) {
            didLine = true;
            for (int x = 0; x <= max_x; ++x) {
                if (paper[topPos][x] == '#') {
                    newP[newpos][x] = '#';
                }
            }
        }

        ++botPos;
        if (botPos <= max_y) {
            didLine = true;
            for (int x = 0; x <= max_x; ++x) {
                if (paper[botPos][x] == '#') {
                    newP[newpos][x] = '#';
                }
            }
        }
        --newpos;
    } while (didLine && newpos >= 0);

    return newP;
}

vector<string> foldX(vector<string> &paper, int pos) {
    cout << "Folding X at " << pos << endl;
    vector<string> newP;
    int max_x = paper[0].length() - 1;
    int max_y = paper.size() - 1;
    cout << "foldX: max_x = " << max_x << ", max_y = " << max_y << endl;

    int leftSize = pos;
    int rightSize = max_x - pos;
    int newSize = max(leftSize, rightSize);
    cout << "leftSize = " << leftSize << ", rightSize = " << rightSize << ", newSize = " << newSize << endl;

    // Create blank paper correct size
    string s = string(newSize, '.');
    for (int i = 0; i <= max_y; ++i) {
        newP.push_back(s);
    }

    int newpos = newSize-1;
    int leftPos = pos, rightPos = pos;
	bool didLine = false;
    do {
        --leftPos;
        ++rightPos;
        didLine = false;

        if (leftPos >= 0) {
            didLine = true;
            for (int y = 0; y <= max_y; ++y) {
                if (paper[y][leftPos] == '#') {
                    newP[y][newpos] = '#';
                }
            }
        }

        if (rightPos <= max_x) {
            didLine = true;
            for (int y = 0; y <= max_y; ++y) {
                if (paper[y][rightPos] == '#') {
                    newP[y][newpos] = '#';
                }
            }
        }
        --newpos;
    } while (didLine && newpos >= 0);

    return newP;
}

int countDots(vector<string>& paper) {
    int max_x = paper[0].length() - 1;
    int max_y = paper.size() - 1;
    int count = 0;

    for (int y = 0; y <= max_y; ++y) {
		for (int x = 0; x <= max_x; ++x) {
            if (paper[y][x] == '#')
                ++count;
		}
    }
    return count;
}


void step1()
{
    readPaper(FILE_NAME);
    cout << "max_x = " << max_x << ", max_y = " << max_y << endl;

    vector<string> curPaper = paper;

    for (int i = 0; i < folds.size(); ++i) {
        vector<string> newPaper;

        switch (folds[i].first) {
        case 'x': newPaper = foldX(curPaper, folds[i].second); break;
        case 'y': newPaper = foldY(curPaper, folds[i].second); break;
        }

        //dspPaper(newPaper);
        curPaper = newPaper;
		cout << "Count is " << countDots(newPaper) << endl;
    }

	dspPaper(curPaper);


//    dspPaper(paper);
//
//    auto newPaper = foldY(paper, folds[0].second);
//    dspPaper(newPaper);
//
//    cout << "Count is " << countDots(newPaper) << endl;
//    exit(0);
//
//    newPaper = foldX(newPaper, folds[1].second);
//    dspPaper(newPaper);
//
//    cout << "Count is " << countDots(newPaper) << endl;
}



int main()
{
    step1();
}
