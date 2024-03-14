/**
 * AdventOfCode2021-Day11.cpp : Advent of Code 2021, Day 11 (parts 1 and 2)
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

#define YELLOW "\x1b[33m"
#define NORMAL "\x1b[0m"

void dspGrid(int grid[10][10]) {
	for (int row = 0; row < 10; ++row) {
		for (int col = 0; col < 10; ++col) {
			if (grid[row][col] == 0)
				cout << YELLOW;
			cout << (char)(grid[row][col] + '0');
			if (grid[row][col] == 0)
				cout << NORMAL;
		}
		cout << endl;
	}
};


void step2()
{
    ifstream fh("octo.txt");
    string s;
    int grid[10][10];

    int r = 0;
    while (getline(fh, s)) {
        cout << s << endl;
        int col = 0;
        for (char c : s) {
            grid[r][col++] = c - '0';
        }
        ++r;
    }

    int flashCount = 0;
    int syncStep = -1;
    int step = 1;
	for (;;) {
        if (step % 1000 == 0) {
            cout << step << endl;
        }

        for (int row = 0; row < 10; ++row) {
            for (int col = 0; col < 10; ++col) {
                ++grid[row][col];
            }
        }

		bool flash[10][10]{ false };
        bool hadFlash;
        do {
            // Spread the flash
            hadFlash = false;
            for (int row = 0; row < 10; ++row) {
                for (int col = 0; col < 10; ++col) {
                    if (!flash[row][col] && grid[row][col] > 9) {
                        flash[row][col] = true;
                        hadFlash = true;
                        // Spread
                        for (int y = max(row - 1, 0); y <= min(9, row + 1); ++y) {
                            for (int x = max(0, col - 1); x <= min(9, col + 1); ++x) {
                                if (!(x == col && y == row)) {
                                    //cout << "y: " << y << ", x:" << x << endl;
                                    ++grid[y][x];
                                }
                            }
                        }
                    }
                }
            }
        } while (hadFlash);

        // Mark flashed to zero
        int count = 0;
        for (int row = 0; row < 10; ++row) {
            for (int col = 0; col < 10; ++col) {
                if (flash[row][col]) {
                    grid[row][col] = 0;
                    ++flashCount;
                    ++count;
                }
            }
        }

        if (count == 100) {
			dspGrid(grid);
            cout << "SYNCED ON STEP " << step << endl;
            exit(0);
        }

		//cout << endl << "GRID step " << step << endl;
		//dspGrid(grid);
        ++step;
    }
}

void step1()
{
    ifstream fh("octo_x.txt");
    string s;
    int grid[10][10];

    int r = 0;
    while (getline(fh, s)) {
        cout << s << endl;
        int col = 0;
        for (char c : s) {
            grid[r][col++] = c - '0';
        }
        ++r;
    }

    int flashCount = 0;
    for (int step = 1; step <= 100; ++step) {
        for (int row = 0; row < 10; ++row) {
            for (int col = 0; col < 10; ++col) {
                ++grid[row][col];
            }
        }

		bool flash[10][10]{ false };
        bool hadFlash;
        do {
            // Spread the flash
            hadFlash = false;
            for (int row = 0; row < 10; ++row) {
                for (int col = 0; col < 10; ++col) {
                    if (!flash[row][col] && grid[row][col] > 9) {
                        flash[row][col] = true;
                        hadFlash = true;
                        // Spread
                        for (int y = max(row - 1, 0); y <= min(9, row + 1); ++y) {
                            for (int x = max(0, col - 1); x <= min(9, col + 1); ++x) {
                                if (!(x == col && y == row)) {
                                    //cout << "y: " << y << ", x:" << x << endl;
                                    ++grid[y][x];
                                }
                            }
                        }
                    }
                }
            }
        } while (hadFlash);

        // Mark flashed to zero
        int count = 0;
        for (int row = 0; row < 10; ++row) {
            for (int col = 0; col < 10; ++col) {
                if (flash[row][col]) {
                    grid[row][col] = 0;
                    ++flashCount;
                    ++count;
                }
            }
        }

		cout << endl << "GRID step " << step << endl;
		dspGrid(grid);
    }

    cout << "FLASH COUNT IS " << flashCount << endl;
}

int main()
{
    step2();
}
