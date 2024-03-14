/**
 * AdventOfCode2021-Day2.cpp : Advent of Code 2021, Day 2 (parts 1 and 2)
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

using namespace std;

std::vector<std::string> str_split(const std::string& s, const char* pattern) {
    std::regex sep_regex = std::regex{ pattern };
    std::sregex_token_iterator iter(s.begin(), s.end(), sep_regex, -1);
    std::sregex_token_iterator end;
    return { iter, end };
}

void step2()
{
    ifstream fh("sub.txt");
    string s;
    int depth = 0;
    int pos = 0;
    int aim = 0;

    while (getline(fh, s)) {
        vector<string> tokens = str_split(s, " ");
        string cmd = tokens[0];
        int val = stoi(tokens[1]);

        if (cmd == "forward") {
            pos += val;
            depth += aim * val;

        } else if (cmd == "up") {
            aim -= val;

        } else if (cmd == "down") {
            aim += val;

        }
        cout << s << ": depth=" << depth << " " << ", aim: " << aim << ", pos: " << pos << endl;
    }

    int prod = depth * pos;
    std::cout << depth << " " << pos << " " << prod;
}

void step1()
{
    ifstream fh("sub.txt");
    string s;
    int depth = 0;
    int pos = 0;

    while (getline(fh, s)) {
        vector<string> tokens = str_split(s, " ");
        string cmd = tokens[0];
        int val = stoi(tokens[1]);

        if (cmd == "forward") {
            pos += val;

        } else if (cmd == "up") {
            depth -= val;

        } else if (cmd == "down") {
            depth += val;

        }
        //cout << cmd << " " << val << endl;
    }

    int prod = depth * pos;
    std::cout << depth << " " << pos << " " << prod;
}

int main()
{
    step2();
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
// AdventOfCode2021-Day2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
