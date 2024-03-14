/**
 * AdventOfCode2021-Day20.cpp : Advent of Code 2021, Day 20 (parts 1 and 2)
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

string alg;
map<string, int> tranMap;

void genTran()
{
	bitset<9> b;
    for (int n = 0; n < 512; ++n) {
        b = n;
		string s;
		for (int i = 8; i >= 0; --i) {
			s += b[i] ? '#' : '.';
		}
        tranMap[s] = n;
	}
}

void dsp(vector<string>& image) {
    int line = 0;
    for (string s : image) {
        string fmt = to_string(line);
        fmt = string(3 - fmt.length(), ' ') + fmt;
        cout << fmt << "| " << s << " |" << endl;
        ++line;
    }
}

void enhance(vector<string>& curImage, vector<string>& dest)
{
    int numLines = curImage.size();
    int width = curImage[0].length();
    for (int y = 1; y < numLines - 1; ++y) {
        for (int x = 1; x < width - 1; ++x) {
            string bin = curImage[y - 1].substr(x - 1, 3) + curImage[y].substr(x - 1, 3) + curImage[y + 1].substr(x - 1, 3);
            dest[y][x] = alg[tranMap[bin]];
        }
    }
}

void part2()
{
    string s;
    ifstream in;
    int extra = 80;

    genTran();

    in.open("image.txt");
    vector<string> baseImage;
    getline(in, alg);
    getline(in, s);
    while (getline(in, s)) {
        cout << s << endl;
        baseImage.push_back(s);
    }

    int inpLength = baseImage[0].length();
    int curWidth = inpLength + extra * 2;
    vector<string> curImage;

    string bline = string(curWidth, '.');
    vector<string> blank;
    for (int i = 0; i < extra; ++i) {
        curImage.push_back(bline);
        blank.push_back(bline);
    }
    for (string& b : baseImage) {
        curImage.push_back(string(extra, '.') + b + string(extra, '.'));
        blank.push_back(bline);
    }
    for (int i = 0; i < extra; ++i) {
        curImage.push_back(bline);
        blank.push_back(bline);
    }

    cout << "CURRENT:" << endl;
    dsp(curImage);

    vector<string> newImage;

    for (int i = 0; i < 50; ++i) {
		newImage = blank;
		enhance(curImage, newImage);
		cout << endl << "NEW " << i << endl;
		//dsp(newImage);

		curImage = newImage;
	}

    int count = 0;
    //for (int y = 2; y < curImage.size() - 2; ++y) {
        //for (int x = 2; x < curImage[0].length() - 2; ++x) {
    for (int y = 50; y < 240; ++y) {
        for (int x = 2; x < curImage[0].length() - 2; ++x) {
            if (curImage[y][x] == '#') {
                ++count;
            }
        }
    }
    dsp(curImage);
    cout << "COUNT: " << count << endl;
}

void part1()
{
#define EXTRA 10
    string s;
    ifstream in;

    genTran();

    in.open("image.txt");
    vector<string> baseImage;
    getline(in, alg);
    getline(in, s);
    while (getline(in, s)) {
        cout << s << endl;
        baseImage.push_back(s);
    }

    int inpLength = baseImage[0].length();
    int curWidth = inpLength + EXTRA * 2;
    vector<string> curImage;

    string bline = string(curWidth, '.');
    vector<string> blank;
    for (int i = 0; i < EXTRA; ++i) {
		curImage.push_back(bline);
		blank.push_back(bline);
	}
	for (string& b : baseImage) {
		curImage.push_back(string(EXTRA, '.') + b + string(EXTRA, '.'));
		blank.push_back(bline);
	}
    for (int i = 0; i < EXTRA; ++i) {
        curImage.push_back(bline);
        blank.push_back(bline);
    }

    cout << "CURRENT:" << endl;
    dsp(curImage);

    vector<string> newImage = blank;

    enhance(curImage, newImage);
    cout << endl << "NEW:" << endl;
    dsp(newImage);

    curImage = newImage;
    newImage = blank;
    enhance(curImage, newImage);
    cout << endl << "NEW:" << endl;
    dsp(newImage);

    int count = 0;
    for (int y = 2; y < newImage.size() - 2; ++y) {
        for (int x = 2; x < newImage[0].length() - 2; ++x) {
            if (newImage[y][x] == '#') {
                ++count;
            }
        }
    }
    cout << "COUNT: " << count << endl;
}

int main()
{
    part2();
}
