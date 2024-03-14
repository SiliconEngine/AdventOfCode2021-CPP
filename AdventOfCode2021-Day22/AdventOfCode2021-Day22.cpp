/**
 * AdventOfCode2021-Day22.cpp : Advent of Code 2021, Day 22 (parts 1 and 2)
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
#include <algorithm>
#include <regex>

#define DEBUG 0

#define FN "cubes.txt"

using namespace std;

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

struct Range {
    int s;
    int e;
};
ostream& operator<<(ostream& out, Range* r) {
    out << "[" << r->s << ", " << r->e << "]";
    return out;
}
ostream& operator<<(ostream& out, Range r) {
    out << &r;
    return out;
}

struct Cuboid {
    bool isOn;
    Range x;
    Range y;
    Range z;
};

struct RangeList {
    int size;
    Range list[3];
};
ostream& operator<<(ostream& out, RangeList* rl) {
    out << "[size=" << rl->size;
    for (int i = 0; i < rl->size; ++i) {
	out << (i == 0 ? ": " : ", ");
	out << &rl->list[i];
    }
    return out;
}

ostream& operator<<(ostream& out, Cuboid *c) {
    out << "isOn=" << c->isOn << ", x=" << c->x.s << ".." << c->x.e
        << ",y=" << c->y.s << ".." << c->y.e
        << ",z=" << c->z.s << ".." << c->z.e;
    return out;
}
ostream& operator<<(ostream& out, Cuboid c) {
    out << &c;
    return out;
}

bool coordOverlap(Range a, Range b) {
    return a.s <= b.e && b.s <= a.e;
}

bool haveOverlap(Cuboid* a, Cuboid* b)
{
    return coordOverlap(a->x, b->x) && coordOverlap(a->y, b->y) && coordOverlap(a->z, b->z);
}

Range getOverlap(Range a, Range b) {
    return Range{ std::max(a.s, b.s), std::min(a.e, b.e) };
}

Cuboid *cubeIntersect(Cuboid* a, Cuboid* b)
{
    Range x = getOverlap(a->x, b->x);
    Range y = getOverlap(a->y, b->y);
    Range z = getOverlap(a->z, b->z);
    return new Cuboid({ 1, { x }, { y }, { z } });
}

/**
 * Count number of cubes by scanning the entire x,y,z space.
 * Slow algorithm for Part 1.
 */
int countSlow(vector<Cuboid*> &list, int size) {
    int lowx = 999999, lowy = 999999, lowz = 999999;
    int highx = -999999, highy = -999999, highz = -999999;
    for (Cuboid *cube : list) {
        if (cube->x.s < lowx) lowx = cube->x.s;
        if (cube->y.s < lowy) lowy = cube->y.s;
        if (cube->z.s < lowz) lowz = cube->z.s;
        if (cube->x.e > highx) highx = cube->x.e;
        if (cube->y.e > highy) highy = cube->y.e;
        if (cube->z.e > highz) highz = cube->z.e;
    }
    int count = 0;
    for (int x = lowx; x <= highx; ++x) {
	for (int y = lowy; y <= highy; ++y) {
	    for (int z = lowz; z <= highz; ++z) {
		bool inside = false;
		for (int i = 0; i < size; ++i) {
		    Cuboid* cube = list[i];
		    if (x >= cube->x.s && y >= cube->y.s && z >= cube->z.s &&
			x <= cube->x.e && y <= cube->y.e && z <= cube->z.e)
		    {
			inside = cube->isOn;
		    }
		}
		if (inside) {
		    ++count;
		}
	    }
	}
    }

    return count;
}

void part1()
{
    fstream in;
    string s;
    vector<Cuboid*> inputList;

    in.open(FN);
    while (getline(in, s)) {
	auto nums = get_matches(s, "[\\-0-9]+");
	Cuboid* c = new Cuboid({ s[1] == 'n', { stoi(nums[0]), stoi(nums[1]) }, {stoi(nums[2]), stoi(nums[3])}, {stoi(nums[4]), stoi(nums[5])}});
	if (c->x.s < -50 || c->x.e > 50 || c->y.s < -50 || c->y.e > 50 || c->z.s < -50 || c->z.e > 50)
	    continue;

	inputList.push_back(c);
    }

    // Do slow brute-force count
    int testcount = countSlow(inputList, inputList.size());
    cout << "FINAL COUNT: " << testcount << endl;
}

long long calcVolume(Cuboid* c) {
    long long diffx = c->x.e - c->x.s + 1;
    long long diffy = c->y.e - c->y.s + 1;
    long long diffz = c->z.e - c->z.s + 1;
    return diffx * diffy * diffz;
}

long long count_item(Cuboid *c, vector<Cuboid*> *rest)
{
    vector<Cuboid*> conflicts;
    long long total = calcVolume(c);

    for (Cuboid* item : *rest) {
	if (haveOverlap(c, item)) {
	    conflicts.push_back(cubeIntersect(c, item));
	}
    }

    for (int i = 0; i < conflicts.size(); ++i) {
	vector<Cuboid*> con_rest(conflicts.begin() + i + 1, conflicts.end());
	total -= count_item(conflicts[i], &con_rest);
    }

    for (Cuboid* item : conflicts)
	delete item;

    return total;
}

void part2()
{
    fstream in;
    string s;
    vector<Cuboid*> inputList;

    in.open(FN);
    while (getline(in, s)) {
        auto nums = get_matches(s, "[\\-0-9]+");
        Cuboid* c = new Cuboid({ s[1] == 'n', { stoi(nums[0]), stoi(nums[1]) }, {stoi(nums[2]), stoi(nums[3])}, {stoi(nums[4]), stoi(nums[5])}});
        inputList.push_back(c);
    }
    
    long long answer = 0;
    for (int i = 0; i < inputList.size(); ++i) {
	Cuboid* inpCube = inputList[i];
	if (!inpCube->isOn)
	    continue;

	vector<Cuboid*> rest(inputList.begin() + i + 1, inputList.end());
	answer += count_item(inpCube, &rest);
    }
    cout << answer << endl;
}

int main()
{
    //part1();
    part2();
}
