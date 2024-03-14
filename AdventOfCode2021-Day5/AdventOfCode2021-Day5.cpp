/**
 * AdventOfCode2021-Day5.cpp : Advent of Code 2021, Day 5 (parts 1 and 2)
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

#define MAXGRID 1000

void step2()
{
    ifstream fh("vents.txt");
    string s;
    char grid[MAXGRID][MAXGRID] = { 0 };

    while (getline(fh, s)) {
		cout << s << endl;
        vector<string> tokens = str_split(s, " -> ");
        vector<string> c1 = str_split(tokens[0], ",");
        vector<string> c2 = str_split(tokens[1], ",");

        int x1 = stoi(c1[0]);
        int y1 = stoi(c1[1]);
        int x2 = stoi(c2[0]);
        int y2 = stoi(c2[1]);

        if (x1 == x2) {
            int ty1, ty2;
            if (y1 < y2) {
                ty1 = y1;
                ty2 = y2;
            } else {
				ty1 = y2;
				ty2 = y1;
			}
            for (int y = ty1; y <= ty2; ++y) {
                ++grid[x1][y];
            }

        } else if (y1 == y2) {
            int tx1, tx2;
            if (x1 < x2) {
                tx1 = x1;
                tx2 = x2;
            } else {
				tx1 = x2;
				tx2 = x1;
			}
            for (int x = tx1; x <= tx2; ++x) {
                ++grid[x][y1];
            }

        } else {
            //cout << "    DIAG" << endl;

            int x = x1;
            int y = y1;
            int incx = sgn(x2 - x1);
            int incy = sgn(y2 - y1);
			do {
                //cout << "    SET " << x << ", " << y << endl;
                ++grid[x][y];
                x += incx;
                y += incy;
            } while (x != x2);
			//cout << "    SET " << x << ", " << y << endl;
			++grid[x][y];
        }
    }

    int overlapCount = 0;
    for (int y = 0; y < MAXGRID; ++y) {
        for (int x = 0; x < MAXGRID; ++x) {
            //cout << grid[x][y]+0 << " ";
            if (grid[x][y] > 1) {
                ++overlapCount;
            }
        }
        //cout << endl;
    }

    cout << "count = " << overlapCount << endl;
}

void step1()
{
    ifstream fh("vents.txt");
    string s;
    char grid[MAXGRID][MAXGRID] = { 0 };

    while (getline(fh, s)) {
		cout << s << endl;
        vector<string> tokens = str_split(s, " -> ");
        vector<string> c1 = str_split(tokens[0], ",");
        vector<string> c2 = str_split(tokens[1], ",");

        int x1 = stoi(c1[0]);
        int y1 = stoi(c1[1]);
        int x2 = stoi(c2[0]);
        int y2 = stoi(c2[1]);

        if (x1 != x2 && y1 != y2)
			continue;

        cout << "    PROCESS" << endl;

        if (x1 == x2) {
            int ty1, ty2;
            if (y1 < y2) {
                ty1 = y1;
                ty2 = y2;
            } else {
				ty1 = y2;
				ty2 = y1;
			}
            for (int y = ty1; y <= ty2; ++y) {
                ++grid[x1][y];
            }
        }

        if (y1 == y2) {
            int tx1, tx2;
            if (x1 < x2) {
                tx1 = x1;
                tx2 = x2;
            } else {
				tx1 = x2;
				tx2 = x1;
			}
            for (int x = tx1; x <= tx2; ++x) {
                ++grid[x][y1];
            }
        }
    }

    int overlapCount = 0;
    for (int y = 0; y < MAXGRID; ++y) {
        for (int x = 0; x < MAXGRID; ++x) {
            //cout << grid[x][y]+0 << " ";
            if (grid[x][y] > 1) {
                ++overlapCount;
            }
        }
        //cout << endl;
    }

    cout << "count = " << overlapCount << endl;
}

int main()
{
    step2();
}
