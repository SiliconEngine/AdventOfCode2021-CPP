/**
 * AdventOfCode2021-Day14.cpp : Advent of Code 2021, Day 14 (parts 1 and 2)
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

#define FILE_NAME "pairs.txt"
#define DEBUG 0

string startString;
map<string, char> rules;

void readPairs(string fn)
{
    string s;
    ifstream fh(fn);

    getline(fh, s);
    startString = s;

    // blank line
    getline(fh, s);

    while (getline(fh, s)) {
        cout << s << endl;

        string pair = { s[0], s[1] };
        rules[pair] = s[6];
    }
}

void dspCounts(map<string, long long> &counts)
{
    int most = 0, least = -1;
    for (auto i : counts) {
        cout << i.first << ": " << i.second << endl;
        if (i.second > most)
            most = i.second;
        if (i.second < least || least < 0)
            least = i.second;
    }
}

void dspLetterCounts(map<char, long long>& letterCounts) {
    for (auto i : letterCounts) {
        cout << i.first << ": " << i.second << endl;
    }
}

void step2()
{
#define NUM_STEPS 40

    readPairs(FILE_NAME);
	map<string, long long> pairCounts;
	map<char, long long> letterCounts;

    for (int i = 0; i < startString.length() - 1; ++i) {
        string p = { startString[i], startString[i + 1] };
        pairCounts[p]++;
    }
    for (char c : startString) {
        letterCounts[c]++;
    }

    dspCounts(pairCounts);

    for (int step = 1; step <= NUM_STEPS; ++step) {
		map<string, long long> tempCounts = pairCounts;

        for (auto ct : pairCounts) {
            if (ct.second == 0)
                continue;

            string p = ct.first;
            //cout << "p = " << p << endl;
            char c = rules[p];
            string p1 = { p[0], c };
            string p2 = { c, p[1] };
            if (DEBUG) cout << "    Splitting " << p << " inc = " << ct.second << ": into " << p1 << " (" << tempCounts[p1] + ct.second << ") and "
                << p2 << " (" << tempCounts[p2] + ct.second << ")" << endl;
            tempCounts[p1] += ct.second;
            tempCounts[p2] += ct.second;
            tempCounts[p] -= ct.second;
            letterCounts[c] += ct.second;
        }

        pairCounts = tempCounts;
        if (DEBUG) cout << endl << "STEP " << step << endl;
        if (DEBUG) dspCounts(pairCounts);
        if (DEBUG) dspLetterCounts(letterCounts);
    }

//    map<char, long long> letters;
//	for (auto ct : pairCounts) {
//        char c1 = ct.first[0];
//        char c2 = ct.first[1];
//        letters[c1] += ct.second;
//        letters[c2] += ct.second;
//    }

    long long most = 0, least = -1;
    for (auto i : letterCounts) {
        cout << i.first << ": " << i.second << endl;
        if (i.second > most)
            most = i.second;
        if (i.second < least || least < 0)
            least = i.second;
    }

    cout << "most: " << most << ", least = " << least << endl;
    cout << "Score: " << most - least << endl;
    


}



void step1()
{
    readPairs(FILE_NAME);

#define NUM_STEPS 10

    string work = startString;
	map<char, int> lastCounts;
    for (int step = 1; step <= NUM_STEPS; ++step) {
		if (DEBUG) cout << "WORK: " << work << endl;
        string next = "";
        for (int i = 0; i < work.length() - 1; ++i) {
			string p = (string)"" + work[i] + work[i+1];
            //cout << "    PAIR: " << p << endl;
            auto rule = rules.find(p);
            if (rule != rules.end()) {
                //cout << "    Find rule " << rule->first << ", xlate to " << rule->second << endl;
                next += (string)"" + work[i] + rule->second;
            } else {
                next += p;
            }
        }
        next += work.substr(work.length()-1, 1);

        if (DEBUG) cout << "    FINAL: " << next << endl;
        work = next;

        //cout << endl << "STEP " << step << endl;
        //dspCounts(work);

//		map<char, int> counts;
//		for (char c : work) {
//			++counts[c];
//		}
//
//		for (auto i : counts) {
//			cout << i.first << ": " << i.second << ", DIFF: " << i.second - lastCounts[i.first] << endl;
//		}
//
//        lastCounts = counts;
    }

    map<char, int> counts;
    for (char c : work) {
        ++counts[c];
    }

    int most = 0, least = -1;
    for (auto i : counts) {
        cout << i.first << ": " << i.second << endl;
        if (i.second > most)
            most = i.second;
        if (i.second < least || least < 0)
            least = i.second;
    }

    cout << "most: " << most << ", least = " << least << endl;
    cout << "Score: " << most - least << endl;
    cout << "Number of rules: " << rules.size() << endl;
}



int main()
{
    step2();
}
