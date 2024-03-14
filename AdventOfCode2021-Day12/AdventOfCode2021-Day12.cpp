/**
 * AdventOfCode2021-Day12.cpp : Advent of Code 2021, Day 12 (parts 1 and 2)
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

struct Cave {
    string name;
    bool multi;
    vector<string> paths;
};

map<string, Cave> caves;

void dspCaves()
{
    for (auto cave : caves) {
        cout << "CAVE " << cave.first << endl << "    ";
        bool first = true;
        for (auto p : cave.second.paths) {
            if (!first)
                cout << ", ";
            first = false;
            cout << p;
        }
        cout << endl;
    }
}

bool isMulti(string p) {
	return (p[0] >= 'A' && p[0] <= 'Z');
};

void readCaves(string name)
{
    string s;
    ifstream fh(name);

    while (getline(fh, s)) {
        cout << s << endl;

        auto path = str_split(s, "-");
        string p1 = path[0];
        string p2 = path[1];

        if (caves.count(p1) == 0) {
            Cave c;
            c.name = p1;
            c.multi = isMulti(p1);
            caves[p1] = c;
        }

        if (caves.count(p2) == 0) {
            Cave c;
            c.name = p2;
            c.multi = isMulti(p2);
            caves[p2] = c;
        }

        caves[p1].paths.push_back(p2);
        caves[p2].paths.push_back(p1);
    }
}

void step2()
{
    readCaves("caves.txt");
    dspCaves();
#define DEBUG 0

    struct Entry {
        string node;
        string path;
        string key;
        bool hasTwice;
        map<string, int> visited;
    };

    queue<Entry> q;
    Entry start;
    start.node = "start";
    start.path = "start";
    start.hasTwice = false;
    q.push(start);
    map<string, bool> endList;
    set<string> visitedPaths;

    int check = 10;
	while (!q.empty()) {
        //if (--check == 0) exit(0);
        Entry &ent = q.front();
        Cave& cave = caves[ent.node];
        if (DEBUG) cout << "Checking " << ent.node << ", path=" << ent.path << endl;

        if (ent.node == "end") {
			endList[ent.path] = true;
			q.pop();
            continue;
        }

        for (string p : cave.paths) {
            if (DEBUG) cout << "    Check path " << p << endl;
            if (p == "start")
                continue;

            bool multi = isMulti(p);
            Entry e = ent;
            bool hasNew = false;

            if (!multi && p != "end") {
                if (e.visited.count(p) == 0) {
                    if (DEBUG) cout << "        Init " << p << ", hasTwice = " << e.hasTwice << endl;
                    if (e.hasTwice) {
                        e.visited[p] = 1;
                    } else {
                        e.visited[p] = 1;
                        hasNew = true;
                    }

                    //e.visited[p] = e.hasTwice ? 1 : 2;
                    //// If a new twice, then we need to create two new nodes for each option
                    //hasNew = ! e.hasTwice;
                    //e.hasTwice = true;
                }
                int n = e.visited[p];
                if (DEBUG) cout << "        Count is " << n << endl;
                if (n == 0) {
                    continue;
                }
                e.visited[p] = n - 1;
            }

            e.node = p;
            e.path += "," + p;
            e.key += "," + p + '1';

            // Already checked this path?
            if (visitedPaths.count(e.key) == 0) {
				visitedPaths.insert(e.key);
				if (DEBUG) cout << "        PUSH: " << e.path << endl;
				q.push(e);
            }

            if (hasNew) {
                e.key[e.key.length()-1] = '2';
                if (visitedPaths.count(e.key) == 0) {
                    e.visited[p] = 1;
                    e.hasTwice = true;
                    if (DEBUG) cout << "        PUSH: " << e.path << " (#2)" << endl;
                    q.push(e);
                }
            }
        }

        q.pop();
	}

    //cout << "PATHS:" << endl;
    //for (auto p : endList) {
        //cout << "    " << p.first << endl;
    //}

    cout << endList.size() << " paths counted" << endl;
}

void step2save()
{
    readCaves("caves_x2.txt");
    dspCaves();
#define DEBUG 0

    struct Entry {
        string node;
        string path;
        string key;
        bool hasTwice;
        map<string, int> visited;
    };

    queue<Entry> q;
    Entry start;
    start.node = "start";
    start.path = "start";
    start.hasTwice = false;
    q.push(start);
    map<string, bool> endList;
    set<string> visitedPaths;

    int check = 10;
	while (!q.empty()) {
        //if (--check == 0) exit(0);
        Entry &ent = q.front();
        Cave& cave = caves[ent.node];
        if (DEBUG) cout << "Checking " << ent.node << ", path=" << ent.path << endl;

        if (ent.node == "end") {
			endList[ent.path] = true;
			q.pop();
            continue;
        }

        for (string p : cave.paths) {
            if (DEBUG) cout << "    Check path " << p << endl;
            if (p == "start")
                continue;

            bool multi = isMulti(p);
            Entry e = ent;
            bool hasNew = false;

            if (!multi && p != "end") {
                if (e.visited.count(p) == 0) {
                    if (DEBUG) cout << "        Init " << p << ", hasTwice = " << e.hasTwice << endl;
                    if (e.hasTwice) {
                        e.visited[p] = 1;
                    } else {
                        e.visited[p] = 1;
                        hasNew = true;
                    }

                    //e.visited[p] = e.hasTwice ? 1 : 2;
                    //// If a new twice, then we need to create two new nodes for each option
                    //hasNew = ! e.hasTwice;
                    //e.hasTwice = true;
                }
                int n = e.visited[p];
                if (DEBUG) cout << "        Count is " << n << endl;
                if (n == 0) {
                    continue;
                }
                e.visited[p] = n - 1;
            }

            e.node = p;
            e.path += "," + p;
            e.key = e.path;

            // Already checked this path?
            if (1 || visitedPaths.count(e.key) == 0) {
				visitedPaths.insert(e.key);
				if (DEBUG) cout << "        PUSH: " << e.path << endl;
				q.push(e);
            }

            if (hasNew) {
                e.key = "2";
                if (1 || visitedPaths.count(e.key) == 0) {
                    e.visited[p] = 1;
                    e.hasTwice = true;
                    if (DEBUG) cout << "        PUSH: " << e.path << " (#2)" << endl;
                    q.push(e);
                }
            }
        }

        q.pop();
	}

    cout << "PATHS:" << endl;
    for (auto p : endList) {
        cout << "    " << p.first << endl;
    }

    cout << endList.size() << " paths counted" << endl;
}

void step1()
{
    readCaves("caves.txt");
    dspCaves();

    struct Entry {
        string node;
        string path;
        set<string> visited;
    };

    queue<Entry> q;
    Entry start;
    start.node = "start";
    start.path = "start";
    start.visited.insert("start");
    q.push(start);
    vector<string> endList;
    set<string> visitedPaths;

    int check = 10;
	while (!q.empty()) {
        //if (--check == 0) exit(0);
        Entry &ent = q.front();
        Cave& cave = caves[ent.node];
        //cout << "Checking " << ent.node << ", path=" << ent.path << endl;

        if (ent.node == "end") {
            endList.push_back(ent.path);
			q.pop();
            continue;
        }

        for (string p : cave.paths) {
            bool multi = isMulti(p);
            if (!multi && ent.visited.count(p) != 0)
                continue;

            Entry e = ent;
            e.node = p;
            e.path += "," + p;

            // Already checked this path?
            if (visitedPaths.count(e.path) != 0)
                continue;
            visitedPaths.insert(e.path);

            if (!multi)
				e.visited.insert(p);

            q.push(e);
        }

        q.pop();
	}

    cout << "PATHS:" << endl;
    for (string p : endList) {
        cout << "    " << p << endl;
    }

    cout << endList.size() << " paths counted" << endl;
}

int main()
{
    step2();
}
