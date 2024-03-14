/**
 * AdventOfCode2021-Day3.cpp : Advent of Code 2021, Day 3 (parts 1 and 2)
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


#define BITSIZE 12
typedef bitset<BITSIZE> bitvec;

bitvec reverse(bitvec v)
{
    bitvec v2;
	for (int i = 0; i < BITSIZE; ++i)
		v2[i] = v[(BITSIZE - 1) - i];
    return v2;
}

void step2()
{
    ifstream fh("binary.txt");
    string s;
    vector<bitvec> list;

    while (getline(fh, s)) {
        bitvec bv;

        for (int col = 0; col < BITSIZE; ++col) {
            //char c = s[(BITSIZE-1)-col];
            char c = s[col];
            if (c == '1') {
                bv[col] = true;
            }
        }

        list.push_back(bv);

        cout << s << ": " << bv << endl;
    }

    int lineCount = list.size();
    int half = lineCount / 2;
    cout << "size: " << lineCount << endl;

    int counts[BITSIZE] = { 0 };

    for (auto bv : list) {
        for (int i = 0; i < BITSIZE; ++i) {
            if (bv[i]) {
                ++counts[i];
            }
        }
    }

    for (int i = 0; i < BITSIZE; ++i) {
        cout << i << ": " << counts[i] << endl;
    }

    // oxygen
    vector<bitvec> lastList = list;
    int oxy;
    for (int i = 0; i < BITSIZE; ++i) {

        int zcount = 0;
        int ocount = 0;
		for (auto bv : lastList) {
			if (bv[i]) {
                ++ocount;
            } else {
                ++zcount;
            }
		}
        bool isMost = ocount >= zcount;

        cout << "Checking " << i << ", lastList size is " << lastList.size() << ", ocount is " << ocount << ", zcount is " << zcount << ", isMost is " << isMost << endl;

        vector<bitvec> newList;
        for (auto ent : lastList) {
            if ((isMost && ent[i]) || (! isMost && ! ent[i])) {
                cout << "    Keeping " << reverse(ent) << endl;
                newList.push_back(ent);
            }
        }
        cout << "    Processed, newList is " << newList.size() << endl;


        //cout << i << ": size is now " << newList.size() << endl;
        if (newList.size() == 1) {
            cout << "have one" << endl;
            auto v = newList[0];
            cout << "v is " << v << endl;

            bitvec v2 = reverse(v);
            cout << "v2 is " << v2 << endl;
            oxy = v2.to_ulong();

            break;
        }

        lastList = newList;
    }

    // CO2
    lastList = list;
    int co2;
    for (int i = 0; i < BITSIZE; ++i) {

        int zcount = 0;
        int ocount = 0;
		for (auto bv : lastList) {
			if (bv[i]) {
                ++ocount;
            } else {
                ++zcount;
            }
		}
        bool isMost = ocount >= zcount;

        cout << "Checking " << i << ", lastList size is " << lastList.size() << ", ocount is " << ocount << ", zcount is " << zcount << ", isMost is " << isMost << endl;

        vector<bitvec> newList;
        for (auto ent : lastList) {
            if ((isMost && ! ent[i]) || (! isMost && ent[i])) {
                cout << "    Keeping " << reverse(ent) << endl;
                newList.push_back(ent);
            }
        }
        cout << "    Processed, newList is " << newList.size() << endl;


        //cout << i << ": size is now " << newList.size() << endl;
        if (newList.size() == 1) {
            cout << "have one" << endl;
            auto v = newList[0];
            cout << "v is " << v << endl;

            bitvec v2 = reverse(v);
            cout << "v2 is " << v2 << endl;
            co2 = v2.to_ulong();

            break;
        }

        lastList = newList;
    }

    cout << "oxy is " << oxy << endl;
    cout << "co2 is " << co2 << endl;

    cout << "prod is " << oxy * co2 << endl;
}

void step1()
{
    ifstream fh("binary.txt");
    string s;

    int lineCount = 0;
    int totals[30] = { 0 };
    int numCols;
    while (getline(fh, s)) {
        ++lineCount;

        int col = 0;
        for (char c : s) {
            if (c == '1') {
                ++totals[col];
            }
            ++col;
        }
    }

    int gamma = 0;
    int eps = 0;

    string g = "", e = "";

    int half = lineCount / 2;
	cout << "Half: " << half << endl;
    for (int n : totals) {
        cout << "Total: " << n << endl;
        if (n == 0)
            break;

        gamma = gamma * 2;
        eps = eps * 2;
        if (n < half) {
            ++gamma;
            g += "1";
            e += "0";
        }
        else {
            ++eps;
            g += "0";
            e += "1";
        }
    }

    cout << "gamma (" << g << ") : " << gamma << endl;
    cout << "eps (" << e << ") : " << eps << endl;

    int power = gamma * eps;
    cout << "Power " << power << endl;
}

int main()
{
    step2();
}
