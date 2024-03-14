/**
 * AdventOfCode2021-Day19.cpp : Advent of Code 2021, Day 19 (parts 1 and 2)
 *
 * NOTE: The solutions for 2021 aren't as cleaned up and well documented as other
 * years I've done.
 *
 * @author	Tim Behrendsen
 */

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <algorithm>
#include <map>
#include <regex>
#include <queue>
#include <numeric>
#include <queue>
#include <stack>
//#include <windows.h>
#include <chrono>

#define DEBUG 0

using namespace std;

vector<string> str_split(const string& s, const char* pattern) {
    regex sep_regex = regex{ pattern };
    sregex_token_iterator iter(s.begin(), s.end(), sep_regex, -1);
    sregex_token_iterator end;
    return { iter, end };
}

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

typedef chrono::high_resolution_clock Clock;

int sgn(int val) {
    return (0 < val) - (val < 0);
}

void dumpVec(vector<string> vec) {
    for (auto t : vec) {
        cout << "'" << t << "', ";
    }
    cout << endl;
}

#define YELLOW "\x1b[33m"
#define NORMAL "\x1b[0m"

//void moveCursor(int row, int col)
//{
//    //system("CLS");
//    COORD c = { col, row };
//    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
//    SetConsoleCursorPosition(h, c);
//}

ostream& operator<<(ostream& out, const vector<string>& v) {
    bool first = true;
    for (auto s : v) {
        if (!first)
            cout << ",";
        cout << s;
        first = false;
    }
    return out;
}

map<string, string> rot[3] = {
    {
        { "+Y", "-Z" },
        { "-Z", "-Y" },
        { "-Y", "+Z" },
        { "+Z", "+Y" },
    },
    {
        { "+Z", "-X" },
        { "-X", "-Z" },
        { "-Z", "+X" },
        { "+X", "+Z" },
    },
    {
        { "+X", "-Y" },
        { "-Y", "-X" },
        { "-X", "+Y" },
        { "+Y", "+X" },
    },
};

struct CoordSet {
	string c[3];
};

class Cube {
private:
    string top = "Z+";
    string bottom = "Z-";
    string north = "Y+";
    string south = "Y-";
    string east = "X+";
    string west = "X-";

public:
    void rotateZL() {
        string t = north;
        north = east;
        east = south;
        south = west;
        west = t;
    }

    void rotateZR() {
        string t = north;
        north = west;
        west = south;
        south = east;
        east = t;
    }

    void rotateXL() {
        string t = top;
        top = north;
        north = bottom;
        bottom = south;
        south = t;
    }

    void rotateXR() {
        string t = top;
        top = south;
        south = bottom;
        bottom = north;
        north = t;
    }

    void rotateYL() {
        string t = top;
        top = east;
        east = bottom;
        bottom = west;
        west = t;
    }

    void rotateYR() {
        string t = top;
        top = west;
        west = bottom;
        bottom = east;
        east = t;
    }

    string getSig() {
        return east + "," + north + "," + top;
    }

	friend ostream& operator<<(ostream& out, const Cube& c) {
		out << "TOP/BOTTOM: " << c.top << "/" << c.bottom << ", ";
		out << "N/S: " << c.north << "/" << c.south << ", ";
        out << "E/W: " << c.east << "/" << c.west;
		return out;
	}
};

Cube combos[24];
string sigs[24];

void buildTrans()
{
    int comboCount = 0;

    { // Z UP
        Cube work;
        sigs[comboCount] = work.getSig();
        combos[comboCount++] = work;
        work.rotateZL();
        sigs[comboCount] = work.getSig();
        combos[comboCount++] = work;
        work.rotateZL();
        sigs[comboCount] = work.getSig();
        combos[comboCount++] = work;
        work.rotateZL();
        sigs[comboCount] = work.getSig();
        combos[comboCount++] = work;
    }

    { // Z DOWN
        Cube work;
        work.rotateXL();
        work.rotateXL();

        sigs[comboCount] = work.getSig();
        combos[comboCount++] = work;
        work.rotateZL();
        sigs[comboCount] = work.getSig();
        combos[comboCount++] = work;
        work.rotateZL();
        sigs[comboCount] = work.getSig();
        combos[comboCount++] = work;
        work.rotateZL();
        sigs[comboCount] = work.getSig();
        combos[comboCount++] = work;
    }

    { // Z-X
        Cube work;
        work.rotateXL();

        sigs[comboCount] = work.getSig();
        combos[comboCount++] = work;
        work.rotateZL();
        sigs[comboCount] = work.getSig();
        combos[comboCount++] = work;
        work.rotateZL();
        sigs[comboCount] = work.getSig();
        combos[comboCount++] = work;
        work.rotateZL();
        sigs[comboCount] = work.getSig();
        combos[comboCount++] = work;
    }

    { // Z+X
        Cube work;
        work.rotateXR();

        sigs[comboCount] = work.getSig();
        combos[comboCount++] = work;
        work.rotateZL();
        sigs[comboCount] = work.getSig();
        combos[comboCount++] = work;
        work.rotateZL();
        sigs[comboCount] = work.getSig();
        combos[comboCount++] = work;
        work.rotateZL();
        sigs[comboCount] = work.getSig();
        combos[comboCount++] = work;
    }

    { // Z-Y
        Cube work;
        work.rotateYL();

        sigs[comboCount] = work.getSig();
        combos[comboCount++] = work;
        work.rotateZL();
        sigs[comboCount] = work.getSig();
        combos[comboCount++] = work;
        work.rotateZL();
        sigs[comboCount] = work.getSig();
        combos[comboCount++] = work;
        work.rotateZL();
        sigs[comboCount] = work.getSig();
        combos[comboCount++] = work;
    }

    { // Z+Y
        Cube work;
        work.rotateYR();

        sigs[comboCount] = work.getSig();
        combos[comboCount++] = work;
        work.rotateZL();
        sigs[comboCount] = work.getSig();
        combos[comboCount++] = work;
        work.rotateZL();
        sigs[comboCount] = work.getSig();
        combos[comboCount++] = work;
        work.rotateZL();
        sigs[comboCount] = work.getSig();
        combos[comboCount++] = work;
    }


    for (int i = 0; i < comboCount; ++i) {
        if (DEBUG) cout << combos[i] << "  --  " << sigs[i] << endl;
    }
}

struct Coord {
    int x;
    int y;
    int z;
};

bool coordCmp(Coord& a, Coord& b) {
	if (a.x != b.x) {
		return a.x < b.x;
	}
	if (a.y != b.y) {
		return a.y < b.y;
	}
	return a.z < b.z;
}


Coord xlateCoord(int x, int y, int z, int orient)
{
    int x2 = 0, y2 = 0, z2 = 0;
    string& sig = sigs[orient];
    //cout << "    SIG: " << sig << endl;
    for (int j = 0; j < 3; ++j) {
        int n;
        switch (sig[j * 3]) {
        case 'X': n = x; break;
        case 'Y': n = y; break;
        case 'Z': n = z; break;
        }
        if (sig[j * 3 + 1] == '-')
            n = -n;

        switch (j) {
        case 0: x2 = n; break;
        case 1: y2 = n; break;
        case 2: z2 = n; break;
        }
    }

    return Coord{ x2, y2, z2 };
}

class Scanner
{
private:
public:
    vector<Coord> baseCoords;
    vector<Coord> tranCoords[24];

    void addCoord(int x, int y, int z)
    {
        //cout << "ADD: " << x << ", " << y << ", " << z << endl;
        baseCoords.push_back({ x, y, z });

        for (int i = 0; i < 24; ++i) {
            Coord c2 = xlateCoord(x, y, z, i);

			//cout << "    ROTATE TO: " << x2 << "," << y2 << "," << z2 << endl;
            tranCoords[i].push_back({ c2.x, c2.y, c2.z });
        }
    }

    void sortTranCoords()
    {
        for (int i = 0; i < 24; ++i) {
            sort(tranCoords[i].begin(), tranCoords[i].end(), coordCmp);
        }
    }
};

string makeKey(int x, int y, int z) {
    return to_string(x) + "," + to_string(y) + "," + to_string(z);
}

string makeKey(Coord &c) {
    return to_string(c.x) + "," + to_string(c.y) + "," + to_string(c.z);
}

void part1()
{
    string s;
    ifstream in;

    vector<Scanner *> scanners;
    buildTrans();

    in.open("scanners.txt");
    Scanner* curScan = NULL;
    while (getline(in, s)) {
        if (s.substr(0, 3) == "---") {
            curScan = new Scanner;
            scanners.push_back(curScan);
            continue;
        }
        if (s == "") {
            continue;
        }

        auto list = str_split(s, ",");
        curScan->addCoord(stoi(list[0]), stoi(list[1]), stoi(list[2]));
    }
    for (auto scan : scanners) {
        scan->sortTranCoords();
    }

    int relativeTo[50];
    Coord relativePos[50]{ 0 };
    int orients[50]{ 0 };
    for (int i = 1; i < 50; ++i) {
        orients[i] = -1;
    }
    int numOrients = scanners.size();
    bool didCompare[50]{ 0 };
    map<string, Coord> beacons;

    // Start with beacons for scanner 0
    for (Coord c : scanners[0]->tranCoords[0]) {
        beacons[makeKey(c)] = c;
    }


    //for (int s1 = 0; s1 < scanners.size(); ++s1) {
	//while (--numOrients) {
	for(;;) {
        int s1 = 0;
        if (DEBUG) cout << "Searching..." << endl;
        while (s1 < scanners.size() && (orients[s1] < 0 || didCompare[s1])) {
            if (DEBUG) cout << "    SKIPPING " << s1 << ", orients=" << orients[s1] << ", didcomp = " << didCompare[s1] << endl;
            ++s1;
        }
        if (s1 == scanners.size())
            break;
        didCompare[s1] = true;

        vector<Coord>& base = scanners[s1]->tranCoords[orients[s1]];

        for (int s2 = 0; s2 < scanners.size(); ++s2) {
            if (orients[s2] >= 0)
                continue;

            map<string, int> counter;

            for (int o1 = 0; o1 < 24; ++o1) {
                vector<Coord>& check = scanners[s2]->tranCoords[o1];
                for (int c1 = 0; c1 < base.size(); ++c1) {
                    for (int c2 = 0; c2 < check.size(); ++c2) {
                        int x = base[c1].x;
                        int y = base[c1].y;
                        int z = base[c1].z;
                        int relx = x - check[c2].x;
                        int rely = y - check[c2].y;
                        int relz = z - check[c2].z;
                        string s = to_string(o1) + "/" + to_string(relx) + "," + to_string(rely) + "," + to_string(relz);
                        ++counter[s];
                    }
                }
            }

            if (DEBUG) cout << endl << "CHECKING " << s2 << " AGAINST " << s1 << " (orientation " << orients[s1] << ")" << endl;
            for (auto& count : counter) {
                if (count.second >= 12) {
                    auto d = str_split(count.first, "[/,]");
                    int orient = stoi(d[0]);
                    Coord r{ stoi(d[1]), stoi(d[2]), stoi(d[3]) };
                    cout << count.first << ": " << count.second << " O: " << orient << endl;

                    if (count.second >= 12 && orients[s2] < 0) {
                        if (DEBUG) cout << "    SET orients[" << s2 << "] to " << orient << endl;
                        orients[s2] = orient;
                        relativeTo[s2] = s1;
                        r.x += relativePos[s1].x;
                        r.y += relativePos[s1].y;
                        r.z += relativePos[s1].z;
                        relativePos[s2] = r;
                        if (DEBUG) cout << "    REL POS: " << r.x << "," << r.y << "," << r.z << "]" << endl;

                        for (Coord c : scanners[s2]->tranCoords[orient]) {
                            c.x += r.x;
                            c.y += r.y;
                            c.z += r.z;
                            if (DEBUG) cout << "        " << makeKey(c) << endl;
							beacons[makeKey(c)] = c;
                        }
                    }
                }
            }
        }
    }

    vector<Coord> beaconList;
    for (auto b : beacons) {
        if (DEBUG) cout << "BEACON: " << b.first << endl;
        beaconList.push_back(b.second);
    }

    cout << endl << "SORTED: " << endl;
    sort(beaconList.begin(), beaconList.end(), coordCmp);
    for (auto b : beaconList) {
        cout << makeKey(b) << endl;
    }

    cout << beaconList.size() << " TOTAL BEACONS" << endl;

    // part2
    int most = 0;
    for (int s1 = 0; s1 < scanners.size(); ++s1) {
        for (int s2 = s1+1; s2 < scanners.size(); ++s2) {
            Coord p1 = relativePos[s1];
            Coord p2 = relativePos[s2];
            int dist = abs(p1.x - p2.x) + abs(p1.y - p2.y) + abs(p1.z - p2.z);
            cout << s1 << " to " << s2 << " = " << dist << endl;
            if (dist > most)
                most = dist;
        }
    }
    cout << "MOST WAS " << most << endl;
}

int main()
{
    part1();
}
