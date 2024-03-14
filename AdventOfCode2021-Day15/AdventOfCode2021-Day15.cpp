/**
 * AdventOfCode2021-Day15.cpp : Advent of Code 2021, Day 15 (parts 1 and 2)
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

#define FILE_NAME "map.txt"
#define DEBUG 0

vector<string> cave;

void readMap(string fn)
{
    string s;
    ifstream fh(fn);

    while (getline(fh, s)) {
        cout << s << endl;
        cave.push_back(s);
    }
}

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

string makeKey(int x, int y) {
	return to_string(x) + "-" + to_string(y);
};

string makeKey(Node *n) {
	return to_string(n->x) + "-" + to_string(n->y);
};

void doMap()
{
    int size_x = cave[0].length();
    int size_y = cave.size();
    int end_x = size_x - 1;
    int end_y = size_y - 1;
    map<string, Node*> caveMap;
    vector<Node*> unvisitedList;
    map<string, bool> visited;

    auto nodeCmp = [](const Node* a, const Node* b) {
        return a->total > b->total;
    };

    for (int y = 0; y < size_y; ++y) {
        for (int x = 0; x < size_x; ++x) {
            Node* node = new Node;
            node->x = x;
            node->y = y;
            node->total = 999999999;
            node->risk = cave[y][x] - '0';
            if (x == 0 && y == 0) {
                node->total = 0;
            }

            caveMap[makeKey(x, y)] = node;
            unvisitedList.push_back(node);
        }
    }

    sort(unvisitedList.begin(), unvisitedList.end(), nodeCmp);
    while (unvisitedList.size()) {
        Node* node = unvisitedList.back();
        if (DEBUG) cout << node << endl;
        unvisitedList.pop_back();

        int x = node->x;
        int y = node->y;
        int total = node->total;

        if (x == end_x && y == end_y) {
            cout << "REACHED END: total = " << total << endl;
            break;
        }

        vector<Node*>checkNodes;
        Node* xx;
        if (y > 0) {
            checkNodes.push_back(xx = caveMap[makeKey(x, y - 1)]);
        }
        if (y < (size_y-1)) {
            checkNodes.push_back(xx = caveMap[makeKey(x, y + 1)]);
        }
        if (x > 0) {
            checkNodes.push_back(xx = caveMap[makeKey(x - 1, y)]);
        }
        if (x < (size_x-1)) {
            checkNodes.push_back(xx = caveMap[makeKey(x + 1, y)]);
        }
        for (Node* check : checkNodes) {
            if (visited[makeKey(check)])
                continue;

            int tent = total + check->risk;
            if (check->total > tent) {
                check->total = tent;
            }
        }
        
        visited[makeKey(node)] = true;
		sort(unvisitedList.begin(), unvisitedList.end(), nodeCmp);
    }

    cout << "END" << endl;
}

void step1()
{
    readMap(FILE_NAME);
    doMap();
}

void step2()
{
    readMap(FILE_NAME);

    int num_y = cave.size();
    vector<string> newCave;
    for (string s : cave) {
        string new_s = s;
        for (int i = 1; i <= 4; ++i) {
            for (char c : s) {
                c += i;
                if (c > '9') {
                    c -= 9;
                }
                new_s += c;
            }
        }
        newCave.push_back(new_s);
    }

	for (int i = 1; i <= 4; ++i) {
        for (int y = 0; y < num_y; ++y) {
            string s = newCave[y];
            string new_s = "";
            for (char c : s) {
                c += i;
                if (c > '9') {
                    c -= 9;
                }
                new_s += c;
            }

			newCave.push_back(new_s);
        }
    }

    for (string s : newCave) {
        //cout << s << endl;
    }

    cave = newCave;

    doMap();
}

int main()
{
    step2();
}
