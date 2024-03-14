/**
 * AdventOfCode2021-Day6.cpp : Advent of Code 2021, Day 6 (parts 1 and 2)
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
    ifstream fh("fish.txt");
    string s;

    getline(fh, s);
	cout << s << endl;

    vector<string> list = str_split(s, ",");
    uint64_t fishDay[10] = { 0 };

    for (auto f : list) {
        ++fishDay[stoi(f)];
    }

    cout << "START" << endl;
	for (auto n : fishDay) {
		cout << n << ", ";
	}
    cout << endl;
    for (int day = 1; day <= 256; ++day) {
        uint64_t newFish = fishDay[0];
        for (int i = 0; i < 8; ++i) {
            fishDay[i] = fishDay[i + 1];
        }
        fishDay[6] += newFish;
        fishDay[8] = newFish;

        cout << "After " << day << " days: ";
        uint64_t total = 0;
        for (auto n : fishDay) {
            cout << n << ", ";
            total += n;
        }
        cout << " (" << total << " fish)" <<endl;
        //cout << "After " << day << " days: " << fish.size() << endl;
    }

}

void step1()
{
    ifstream fh("fish.txt");
    string s;

    getline(fh, s);
	cout << s << endl;

    vector<string> list = str_split(s, ",");
    vector<int> fish;
    for (auto f : list) {
        fish.push_back(stoi(f));
    }

    for (int day = 1; day <= 80; ++day) {
        int newFish = 0;
        for (int i = 0; i < fish.size(); ++i) {
            int n = fish[i];
            if (n == 0) {
                fish[i] = 6;
                ++newFish;
            }
            else {
                fish[i] = n - 1;
            }
        }
        for (int i = 0; i < newFish; ++i) {
			fish.push_back(8);
		}

        //cout << "After " << day << " days: ";
        //for (auto n : fish) {
            //cout << n << ",";
        //}
        cout << endl;
    }

    cout << "TOTAL FISH: " << fish.size() << endl;
}

int main()
{
    step2();
}
