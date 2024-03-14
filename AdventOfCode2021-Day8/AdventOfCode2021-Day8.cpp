/**
 * AdventOfCode2021-Day8.cpp : Advent of Code 2021, Day 8 (parts 1 and 2)
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
#include <map>
#include <set>

using namespace std;

#define FN "dsp.txt"

std::vector<std::string> str_split(const std::string& s, const char* pattern) {
    std::regex sep_regex = std::regex{ pattern };
    std::sregex_token_iterator iter(s.begin(), s.end(), sep_regex, -1);
    std::sregex_token_iterator end;
    return { iter, end };
}

void step2()
{
    ifstream fh(FN);
    string s;

    int total = 0;
    while (getline(fh, s)) {
        vector<string> tokens = str_split(s, " \\| ");
        string inp = tokens[0];
        string out = tokens[1];

        map<char, int> freq;
        //for (char c = 'a'; c <= 'g'; ++c)
            //freq[c] = 0;
        for (char c : inp) {
            if (c != ' ')
                ++freq[c];
        }

        // Copying map elements to a vector of pairs
        std::vector<std::pair<char, int>> vec;
        for (const auto& it : freq)
            vec.push_back(it);

        // Sorting the vector by value, keeping the key associated with its value
        std::sort(vec.begin(), vec.end(), [](const std::pair<char, int>& a, const std::pair<char, int>& b) {
            return a.second < b.second;
	});

        // Printing the sorted vector
        map<char, int> charToFreq;
        int i = 0;
	for (const auto& it : vec) {
            charToFreq[it.first] = it.second;
	}

        map<string,char> digTran = {
            { "467889", '0' },
            { "89", '1' },
            { "47788", '2' },
            { "77889", '3' },
            { "6789", '4' },
            { "67789", '5' },
            { "467789", '6' },
            { "889", '7' },
            { "4677889", '8' },
            { "677889", '9' },
        };

        vector<string> outList = str_split(out, " ");
        string tranNum;
        for (string tranItem : outList) {
            vector<int> nums;
            for (char c : tranItem) {
                nums.push_back(charToFreq[c]);
            }
            std::sort(nums.begin(), nums.end());
            string s;
            for (int n : nums)
                s += (char)(n + '0');
            tranNum += digTran[s];
        }

        total += stoi(tranNum);
    }

    cout << "total is " << total << endl;
}

void step1()
{
    ifstream fh(FN);
    string s;

    int easyCount = 0;
    while (getline(fh, s)) {
        vector<string> tokens = str_split(s, " \\| ");
        string inp = tokens[0];
        string out = tokens[1];

        vector<string> outDig = str_split(out, " ");
        for (int i = 0; i < 4; ++i) {
            int len = outDig[i].length();
            if (len == 2 || len == 3 || len == 4 || len == 7) {
                ++easyCount;
            }

        }
    }
    cout << "easy count = " << easyCount << endl;
}

int main()
{
    step2();
}
