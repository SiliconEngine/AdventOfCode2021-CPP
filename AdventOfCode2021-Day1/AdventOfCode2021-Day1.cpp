/**
 * AdventOfCode2021-Day1.cpp : Advent of Code 2021, Day 1 (parts 1 and 2)
 *
 * NOTE: The solutions for 2021 aren't as cleaned up and well documented as other
 * years I've done.
 *
 * @author	Tim Behrendsen
 */

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main()
{
    ifstream fh("sonar.txt");
    string s;

    int last[3] = { -1, -1, -1 };
    int lastSum = -1;
    int count = 0;
    while (getline(fh, s)) {
        int sum = -1;
        int n = stoi(s);

        last[2] = last[1];
        last[1] = last[0];
        last[0] = n;
        cout << s << " " << "lastSum = " << lastSum << ", " << last[0] << " " << last[1] << " " << last[2];

        if (last[2] >= 0) {
            sum = last[0] + last[1] + last[2];
            cout << "sum: " << sum;
            if (lastSum >= 0 && sum > lastSum) {
                ++count;
            }
        }

		lastSum = sum;
		cout << "(" << count << ")" << endl;
    }

    std::cout << "count is " << count << endl;
}
