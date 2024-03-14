/**
 * AdventOfCode2021-Day17.cpp : Advent of Code 2021, Day 17 (parts 1 and 2)
 *
 * NOTE: The solutions for 2021 aren't as cleaned up and well documented as other
 * years I've done.
 *
 * @author	Tim Behrendsen
 */

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <algorithm>
#include <map>
#include <regex>
#include <queue>
#include <numeric>
#include <queue>
//#include <windows.h>
#include <chrono>

#define DEBUG 0

using namespace std;

vector<string> str_split(const string& s, const char* pattern) {
    regex sep_regex = regex{ pattern };
    sregex_token_iterator iter(s.begin(), s.end(), sep_regex, -1);
    sregex_token_iterator end;
    return { iter, end };
}

vector<string> get_matches(const string& str, const string& pattern)
{
    vector<string> matches;
    regex regex_pattern(pattern);
    smatch match_results;
    string search_str = str;

    while (regex_search(search_str, match_results, regex_pattern))
    {
        matches.push_back(match_results[0]);
        search_str = match_results.suffix().str();
    }

    return matches;
}

typedef chrono::high_resolution_clock Clock;

int sgn(int val) {
    return (0 < val) - (val < 0);
}

void dumpVec(vector<string> vec) {
    for (auto t : vec) {
        cout << "'" << t << "', ";
    }
    cout << endl;
}

#define YELLOW "\x1b[33m"
#define NORMAL "\x1b[0m"

//void moveCursor(int row, int col)
//{
//    //system("CLS");
//    COORD c = { col, row };
//    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
//    SetConsoleCursorPosition(h, c);
//}

//ostream& operator<<(ostream& out, const Coord& c) {
//    out << c.x << "," << c.y << "," << c.z;
//    return out;
//}

ostream& operator<<(ostream& out, const vector<string>& v) {
    bool first = true;
    for (auto s : v) {
        if (!first)
            cout << ",";
        cout << s;
        first = false;
    }
    return out;
}

bool simuX(vector<int> &target, int ivx) {
    int tx1 = target[0];
    int tx2 = target[1];
    if (DEBUG) cout << "Check X " << ivx << ", TARGET: " << tx1 << ", " << tx2 << endl;

    int x = 0;
    int vx = ivx;
    do {
        x += vx;
        if (vx > 0)
            --vx;

        if (x >= tx1 && x <= tx2) {
            if (DEBUG) cout << "HIT: [" << x << "]" << endl;
            return true;
        }

        if (vx == 0)
            return false;

        if (DEBUG) cout << "    [" << x << "]" << endl;
    } while (x <= tx2);

    if (DEBUG) cout << "MISS" << endl;
    return false;
}

bool simuY(vector<int> &target, int ivy) {
    int ty1 = target[2];
    int ty2 = target[3];
    if (DEBUG) cout << "Check Y " << ivy << ", TARGET: " << ty1 << ", " << ty2 << endl;

    int y = 0;
    int vy = ivy;
    do {
        y += vy;
        --vy;

        if (y >= ty1 && y <= ty2) {
            if (DEBUG) cout << "HIT: [" << y << "]" << endl;
            return true;
        }

        if (DEBUG) cout << "    [" << y << "]" << endl;
    } while (y >= ty1);

    if (DEBUG) cout << "MISS" << endl;
    return false;
}

int highest;

int simu(vector<int> &target, int ivx, int ivy) {
    int tx1 = target[0];
    int tx2 = target[1];
    int ty1 = target[2];
    int ty2 = target[3];
    if (DEBUG) cout << "TARGET: " << tx1 << ", " << ty1 << " : " << tx2 << ", " << ty2 << endl;

    int x = 0;
    int y = 0;
    int vx = ivx;
    int vy = ivy;
    int loopCount = 100;
    highest = -99999;
    do {
        //if (--loopCount == 0) exit(0);
        x += vx;
        y += vy;
        if (vx > 0)
            --vx;
        --vy;
        if (y > highest) {
            highest = y;
        }

        if (x >= tx1 && x <= tx2 && y >= ty1 && y <= ty2) {
            if (DEBUG) cout << "HIT: [" << x << ", " << y << "]" << endl;
            return true;
        }

        if (DEBUG) cout << "    [" << x << ", " << y << "]" << endl;
    } while (x <= tx2 && y >= ty1);

    if (DEBUG) cout << "MISS" << endl;
    return false;
}

void part1()
{
    string s;
    // Test target
    //vector<int> target = { 20, 30, -10, -5 };


    // Input target
    vector<int> target = { 287, 309, -76, -48 };

    // x1000 test
    //vector<int> target = { 287000, 309000, -76000, -48000 };

    //simu(target, 10, -2);
    //simu(target, 7, 2);
    //simu(target, 6, 3);
    //simu(target, 9, 0);
    //simu(target, 17, -4);
    //simu(target, 6, 9);

    //simuX(target, 17);
    //simuY(target, -4);

    int vyRange = max(abs(target[2]), abs(target[3]));

    vector<int> goodVxList;
    for (int vx = 1; vx <= target[1]; ++vx) {
        if (simuX(target, vx)) {
            goodVxList.push_back(vx);
        }
    }

    for (int n : goodVxList) {
        if (DEBUG) cout << "GOOD X: " << n << endl;
    }

    vector<int> goodVyList;

    for (int vy = 0-vyRange; vy <= vyRange; ++vy) {
        if (simuY(target, vy)) {
            goodVyList.push_back(vy);
        }
    }

    for (int n : goodVyList) {
        if (DEBUG) cout << "GOOD Y: " << n << endl;
    }

    vector<pair<int, int>> goodList;
    vector<int> highestList;

    int goodCount = 0;
    for (int vx : goodVxList) {
		for (int vy : goodVyList) {
            if (simu(target, vx, vy)) {
                ++goodCount;
            }
		}
    }
    cout << "good count = " << goodCount << endl;
    exit(0);




    for (int vx : goodVxList) {
		for (int vy : goodVyList) {
            if (simu(target, vx, vy)) {
                goodList.push_back({ vx, vy });
                highestList.push_back(highest);
            }
		}
    }

// BRUTE FORCE
//    for (int vx = 1; vx <= target[1]; ++vx) {
//		for (int vy = 0-vyRange; vy <= vyRange; ++vy) {
//            if (simu(target, vx, vy)) {
//                goodList.push_back({ vx, vy });
//                highestList.push_back(highest);
//            }
//		}
//    }

    int maxHighest = -9999999;
	for (int i = 0; i < goodList.size(); ++i) {
        auto p = goodList[i];
        //cout << "GOOD: [" << p.first << ", " << p.second << "]: " << highestList[i] << endl;
        if (highestList[i] > maxHighest) {
            maxHighest = highestList[i];
        }
    }

    cout << "max highest: " << maxHighest << endl;
    cout << "total combos: " << goodList.size() << endl;

}


int main()
{
    part2();
}
