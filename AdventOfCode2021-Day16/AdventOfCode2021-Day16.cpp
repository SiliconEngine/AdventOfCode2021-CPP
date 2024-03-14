/**
 * AdventOfCode2021-Day16.cpp : Advent of Code 2021, Day 16 (parts 1 and 2)
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

#define FILE_NAME "code.txt"
#define DEBUG 1

vector<string> cave;

struct Node {
    int x;
    int y;
    char risk;
    int total;
};

ostream& operator<<(ostream& out, const Node* n) {
    if (n == NULL) {
        out << "[NULL]";
        return out;
    }
    out << "[" << n->x << ", " << n->y << ": " << n->total << "]";
    return out;
}

class BitStream
{
private:
	string bits;
	int pos;

public:
	void loadFile(string fn)
	{
		string s;
		ifstream fh(fn);

		getline(fh, s);
		parseCode(s);
	}

	void parseCode(string hex)
	{
		map<char, string> hex2bin = {
			{ '0', "0000" },
			{ '1', "0001" },
			{ '2', "0010" },
			{ '3', "0011" },
			{ '4', "0100" },
			{ '5', "0101" },
			{ '6', "0110" },
			{ '7', "0111" },
			{ '8', "1000" },
			{ '9', "1001" },
			{ 'A', "1010" },
			{ 'B', "1011" },
			{ 'C', "1100" },
			{ 'D', "1101" },
			{ 'E', "1110" },
			{ 'F', "1111" },
		};

		bits = "";
		for (char c : hex) {
			bits += hex2bin[c];
		}
		pos = 0;
	}

	void dspBits()
	{
		cout << bits << endl;
	}

	bool empty()
	{
		return pos >= bits.length();
	}

	int getPos()
	{
		return pos;
	}

	int getMaxPos()
	{
		return bits.length();
	}

	int getNext()
	{
		if (!empty()) {
			return bits[pos++] - '0';
		}

		return 0;
	}

	unsigned int getInt(int n)
	{
		unsigned int ret = 0;
		while (n--) {
			ret <<= 1;
			ret |= getNext();
		}
		return ret;
	}

	string getString(int n)
	{
		return bits.substr(pos, n);
		pos += n;
	}

	string peek(int n)
	{
		return bits.substr(pos, n);
	}

	void normalize()
	{
		pos += (8 - pos % 8);
	}
};

BitStream bs;

int verSum = 0;

void step1()
{
	long long process();

	//bs.parseCode("D2FE28");
	//bs.parseCode("38006F45291200"); // 10, 20
	//bs.parseCode("EE00D40C823060"); // 1, 2, 3
	//bs.parseCode("8A004A801A8002F478"); // 15 (sum: 16)
	//bs.parseCode("620080001611562C8802118E34"); // 12, 13 (sum: 12)
	//bs.parseCode("C0015000016115A2E0802F182340"); // 12, 13 (sum: 23)
	//bs.parseCode("A0016C880162017C3686B18A3D4780"); // 6, 6, 12, 15, 15 (sum: 31)
	bs.loadFile("code.txt");

	bs.dspBits();
	process();

	cout << "Version sum is " << verSum << endl;
}

void step2()
{
	long long process();

	//bs.parseCode("C200B40A82"); // 1 + 2 = 3
	//bs.parseCode("04005AC33890"); // 6 * 9 = 54
	//bs.parseCode("880086C3E88112"); // MIN(7, 8, 9) = 7
	//bs.parseCode("CE00C43D881120"); // MAX(7, 8, 9) = 9
	//bs.parseCode("D8005AC2A8F0"); // 5 < 15 = 1
	//bs.parseCode("F600BC2D8F"); // 5 > 15 = 0
	//bs.parseCode("9C005AC2F8F0"); // 5 = 15 = 0
	//bs.parseCode("9C0141080250320F1802104A08"); // 1 + 3 = 2 * 2
	bs.loadFile("code.txt");

	bs.dspBits();
	long long n = process();

	cout << "Version sum is " << verSum << endl;
	cout << "Final value is " << n << endl;
}

int badCheck = 20;

long long process()
{
	//if (--badCheck == 0) exit(0);
	while (! bs.empty()) {
		cout << "Current pos is: " << bs.getPos() << ", max pos = " << bs.getMaxPos() << endl;
		cout << "    " << bs.peek(32) << endl;

		int ver = bs.getInt(3);
		verSum += ver;
		int type = bs.getInt(3);
		cout << "    ver: " << ver << ", type = " << type << endl;

		long long num = 0;
		long long chunk = 0;

		switch (type) {
		case 4:
			do {
				chunk = bs.getInt(5);
				num <<= 4;
				num |= chunk & 0x0f;
			} while (chunk & 0x10);

			//bs.normalize();
			cout << "num = " << num << endl;
			return num;
			//break;

		default:
			int typeId = bs.getInt(1);
			int totalLen = 0;
			int savePos = 0;
			vector<long long> subList;

			switch (typeId) {
			case 0:				// Next 15 bits are total length of sub-packets
				totalLen = bs.getInt(15);
				savePos = bs.getPos();
				cout << "    TypeID 0: len = " << totalLen << ", savePos = " << savePos << endl;
				do {
					long long newnum = process();
					subList.push_back(newnum);
					cout << "    Length based (" << totalLen << ", at " << (bs.getPos() - savePos) << "), newnum = " << newnum << endl;
				} while ((bs.getPos() - savePos) < totalLen);
				cout << "    Done with length-based" << endl;

				break;
			case 1:				// Next 11 bits are total number of sub-packets
				int numPackets = bs.getInt(11);
				cout << "    TypeID 1: numPackets = " << numPackets << endl;
				for (int i = 0; i < numPackets; ++i) {
					long long newnum = process();
					subList.push_back(newnum);
					cout << "    Count based (" << i + 1 << " of " << numPackets << ", newnum = " << newnum << endl;
				}
				cout << "    Done with count-based" << endl;
				break;
			}

			// Do operation type
			switch (type) {
			case 0:
			{ // sum op
				long long sum = 0;
				if (DEBUG) cout << "SUM:";
				for (auto n : subList) {
					sum += n;
					if (DEBUG) cout << " " << n;
				}
				if (DEBUG) cout << " = " << sum << endl;
				return sum;
			}

			case 1:
			{ // product
				long long prod = 1;
				if (DEBUG) cout << "PROD:";
				for (auto n : subList) {
					prod *= n;
					if (DEBUG) cout << " " << n;
				}
				if (DEBUG) cout << " = " << prod << endl;
				return prod;
			}

			case 2: // min
			{
				long long mn = -1;
				if (DEBUG) cout << "MIN:";
				for (auto n : subList) {
					if (DEBUG) cout << " " << n;
					if (mn < 0) {
						mn = n;
					} else {
						mn = min(mn, n);
					}
				}
				if (DEBUG) cout << " = " << mn << endl;
				return mn;
			}

			case 3: // max
			{
				long long mx = 0;
				if (DEBUG) cout << "MAX:";
				for (auto n : subList) {
					if (DEBUG) cout << " " << n;
					mx = max(mx, n);
				}
				if (DEBUG) cout << " = " << mx << endl;
				return mx;
			}

			case 5: // greater
				if (DEBUG) cout << "GT: " << subList[0] << " > " << subList[1] << endl;
				return subList[0] > subList[1];

			case 6: // less
				if (DEBUG) cout << "LT: " << subList[0] << " < " << subList[1] << endl;
				return subList[0] < subList[1];

			case 7: // equal
				if (DEBUG) cout << "EQ: " << subList[0] << " = " << subList[1] << endl;
				return subList[0] == subList[1];
			}
		}
		//bs.normalize();
	}

	return 0;
}

int main()
{
    step2();
}
