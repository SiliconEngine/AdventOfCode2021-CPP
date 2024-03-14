/**
 * AdventOfCode2021-Day21.cpp : Advent of Code 2021, Day 21 (parts 1 and 2)
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
#include <stack>
#include <bitset>
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

//{ 3, 1 }, // 1 1 1
//{ 4, 3 }, // 1 1 2, 1 2 1, 2 1 1
//{ 5, 6 }, // 1 2 2, 2 1 2, 2 2 1, 3 1 1, 1 3 1, 1 1 3
//{ 6, 7 }, // 1 2 3, 1 3 2, 3 1 2, 3 2 1, 2 3 1, 2 1 3, 2 2 2
//{ 7, 6 }, // 3 2 2, 2 3 2, 2 2 3, 3 3 1, 3 1 3, 1 3 3
//{ 8, 3 }, // 2 3 3, 3 2 3, 3 3 2
//{ 9, 1 }, // 3 3 3
int totalUniv[7] = { 1, 3, 6, 7, 6, 3, 1 };

long long p1wins = 0;
long long p2wins = 0;
long long loopCount = 0;
int maxLevel = 0;
long long maxUniv = 0;
int levelTotal = 0;
int maxScore = 21;

void run(int level, int p1pos, int p2pos, int p1score, int p2score, long long numUniv)
{
    //cout << "LEVEL: " << level << endl;
    if (level > maxLevel) {
        maxLevel = level;
    }
    if (numUniv > maxUniv) {
        maxUniv = numUniv;
    }
    for (int die1 = 3; die1 <= 9; ++die1) {
        int t_p1score = p1score;
        int t_p1pos = p1pos;
		long long t_numUniv = numUniv;
		++loopCount;

        //cout << "    1: " << die1 << endl;
        if ((t_p1pos += die1) > 10)
            t_p1pos -= 10;
        t_p1score += t_p1pos;
        t_numUniv *= totalUniv[die1 - 3];
		//cout << "    Die1: " << die1 << ", P1 pos " << t_p1pos << ", score: " << t_p1score << ", numUniv = " << t_numUniv << endl;
        if (t_p1score >= maxScore) {
            p1wins += t_numUniv;
            continue;
        }

        for (int die2 = 3; die2 <= 9; ++die2) {
			int t_p2score = p2score;
			int t_p2pos = p2pos;
			long long t2_numUniv = t_numUniv;
			++loopCount;

			//cout << "    2: " << die2 << endl;
            if (level == 0) {
                cout << die1 << " / " << die2 << endl;
            }

			if ((t_p2pos += die2) > 10)
				t_p2pos -= 10;
            t_p2score += t_p2pos;
            t2_numUniv *= totalUniv[die2 - 3];
			//cout << "        Die2: " << die2 << ", P2 pos " << t_p2pos << ", score: " << t_p2score << ", numUniv = " << t2_numUniv << endl;
            if (t_p2score >= maxScore) {
                //cout << "     DIE2 WIN" << endl;
                p2wins += t2_numUniv;
                continue;
            }
            if (level == 0) {
                levelTotal += t2_numUniv;
            }

            run(level+1, t_p1pos, t_p2pos, t_p1score, t_p2score, t2_numUniv);
        }
    }
}



void part2()
{
    // Example
    //int p1pos = 4;
    //int p2pos = 8;

    // Puzzle
    int p1pos = 10;
    int p2pos = 6;


    int p1score = 0;
    int p2score = 0;

    run(0, p1pos, p2pos, p1score, p2score, 1);

    cout << "P1 wins " << p1wins << endl;
    cout << "P2 wins " << p2wins << endl;
    cout << "Loop count was " << loopCount << endl;
    cout << "Max level was " << maxLevel << endl;
    cout << "Max univ was " << maxUniv << endl;
    cout << "Level total was " << levelTotal << endl;
}




void part1()
{
    // Example
    //int p1pos = 4;
    //int p2pos = 8;

    // Puzzle
    int p1pos = 10;
    int p2pos = 6;

    int die = 1;
    int numRolls = 0;

    auto getRoll = [&die, &numRolls]() {
        int roll = die;
        if (++die > 100) die = 1;
        roll += die;
        if (++die > 100) die = 1;
        roll += die;
        if (++die > 100) die = 1;
        numRolls += 3;
        return roll;
    };

    int p1score = 0, p2score = 0;
    for (;;) {
        int roll = getRoll();
        p1pos = ((p1pos - 1) + roll) % 10 + 1;
        p1score += p1pos;
        if (p1score >= 1000)
            break;

        roll = getRoll();
        p2pos = ((p2pos - 1) + roll) % 10 + 1;
        p2score += p2pos;
        if (p1score >= 1000)
            break;
    }

    cout << "P1 score = " << p1score << ", P2 score = " << p2score << endl;
    cout << "Num rolls = " << numRolls << endl;

    int minscore = min(p1score, p2score);
    cout << "Answer: " << minscore * numRolls << endl;

}

int main()
{
    part2();
}
