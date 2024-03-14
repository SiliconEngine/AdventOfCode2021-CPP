/**
 * AdventOfCode2021-Day18.cpp : Advent of Code 2021, Day 18 (parts 1 and 2)
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

//ostream& operator<<(ostream& out, const Coord& c) {
//    out << c.x << "," << c.y << "," << c.z;
//    return out;
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

enum ItemType { I_NUM, I_PAIR };

struct Item {
    ItemType t;
    union {
        struct Pair* p;
        int num;
    };
};

struct Pair {
    struct Pair* parent;
    Item items[2];
};

struct PairIndex {
    struct Pair* p;
    int idx;
};
string formatNode(Pair* node)
{
    if (node == NULL)
        return "[NULL]";

	string s = "[";
	for (int idx = 0; idx < 2; ++idx) {
		if (node->items[idx].t == I_NUM) {
			s += to_string(node->items[idx].num);
		} else {
			s += formatNode(node->items[idx].p);
		}
		if (idx == 0)
			s += ",";
	}
	s += "]";
	return s;
}

string format(Pair *root)
{
	return formatNode(root);
}

ostream& operator<<(ostream& out, Pair* node) {
    out << formatNode(node);
    return out;
}

ostream& operator<<(ostream& out, const PairIndex& pi) {
    out << formatNode(pi.p) << ": idx " << pi.idx;
    return out;
}

class SnailNum
{
private:
    Pair* root;

public:
    void validateNode(Pair* node) {
        for (int idx = 0; idx < 2; ++idx) {
            if (node->items[idx].t == I_PAIR) {
                if (node->items[idx].p->parent != node) {
                    cout << "INVALID PARENT" << endl;
                    exit(0);
                }
                validateNode(node->items[idx].p);
            }
        }
    }

    void validate() {
        if (root != NULL)
            validateNode(root);
    }

    Pair *parse(string s) {
        //cout << "PARSE: " << s << endl;
        int idx[50];
        int level = 0;
        Pair* node = NULL;
        Pair* top = NULL;

        for (char c : s) {
            //cout << "    c: " << c << endl;
            if (c >= '0' && c <= '9') {
                int i = idx[level];
                node->items[i].num = c - '0';
                //cout << "    num set to " << c << endl;
                node->items[i].t = I_NUM;

            } else if (c == ',') {
                ++idx[level];

            } else if (c == '[') {
                //cout << "    OPEN BRACKET" << endl;
                Pair* parent = node;
                node = new Pair;

                if (top == NULL) {
                    top = node;
                }
                node->parent = parent;
                if (parent != NULL) {
                    parent->items[idx[level]].p = node;
                    parent->items[idx[level]].t = I_PAIR;
                }

                ++level;
                idx[level] = 0;

            } else if (c == ']') {
                node = node->parent;
                --level;
            }
        }

        return top;
    }

    void set(string s) {
        root = parse(s);
        validate();
    }

    void add(string s) {
        Pair* newadd = parse(s);
        if (root == NULL) {
            root = newadd;
            return;
        }
        Pair* newroot = new Pair;
        root->parent = newroot;
        newadd->parent = newroot;
        newroot->parent = NULL;
        newroot->items[0].t = newroot->items[1].t = I_PAIR;
        newroot->items[0].p = root;
        newroot->items[1].p = newadd;
        root = newroot;
        validate();
    }

    SnailNum() {
    }

    SnailNum(string s) {
        set(s);
    }

    Pair* findExplode(Pair* node, int depth)
    {
        if (node->items[0].t == I_NUM && node->items[1].t == I_NUM) {
            if (depth >= 4) {
				if (DEBUG) cout << "        FOUND PAIR AT DEPTH " << depth << endl;
                return node;
			}
        }

        for (int idx = 0; idx < 2; ++idx) {
            if (node->items[idx].t == I_PAIR) {
                Pair* ret = findExplode(node->items[idx].p, depth + 1);
                if (ret != NULL)
                    return ret;
            }
        }

        return NULL;
    }

    PairIndex getLeft(PairIndex pi)
    {
        Pair* node = pi.p;
        int idx = pi.idx;
        if (node == NULL)
            return PairIndex{ NULL, 0 };

        if (--idx >= 0) {
            if (node->items[idx].t == I_NUM) {
                return PairIndex{ node, idx };
            }

            return getLeft(PairIndex{ node->items[idx].p, 2 });
        }

        Pair* parent = node->parent;
        while (parent != NULL) {
            if (parent->items[1].t == I_PAIR && parent->items[1].p == node) {
                return getLeft(PairIndex{ parent, 1 });
            }

            node = parent;
            parent = node->parent;
        }

		return PairIndex{ NULL, 0 };
    }

    PairIndex getRight(PairIndex pi)
    {
        Pair* node = pi.p;
        int idx = pi.idx;
        if (node == NULL)
            return PairIndex{ NULL, 0 };

        if (++idx < 2) {
            if (node->items[idx].t == I_NUM) {
                return PairIndex{ node, idx };
            }

            return getRight(PairIndex{ node->items[idx].p, -1 });
        }

        Pair* parent = node->parent;
        while (parent != NULL) {
            if (parent->items[0].t == I_PAIR && parent->items[0].p == node) {
                return getRight(PairIndex{ parent, 0 });
            }

            node = parent;
            parent = node->parent;
        }

		return PairIndex{ NULL, 0 };
    }

	friend ostream& operator<<(ostream& out, SnailNum& sn) {
		out << format(sn.root);
		return out;
	}

	friend ostream& operator<<(ostream& out, SnailNum *sn) {
		out << format(sn->root);
		return out;
	}

    bool reduceExplode()
    {
        Pair* node = findExplode(root, 0);
        if (node == NULL)
            return false;

		if (DEBUG) cout << "        HAVE NODE" << node << endl;

		PairIndex left = getLeft({ node, 0 });
		if (DEBUG) cout << "        LEFT: " << left << endl;
		if (left.p != NULL) {
			left.p->items[left.idx].num += node->items[0].num;
		}

		PairIndex right = getRight({ node, 1 });
		if (DEBUG) cout << "        RIGHT: " << right << endl;
		if (right.p != NULL) {
			right.p->items[right.idx].num += node->items[1].num;
		}

		if (node->parent->items[0].p == node) {
			node->parent->items[0].t = I_NUM;
			node->parent->items[0].num = 0;
		} else {
			node->parent->items[1].t = I_NUM;
			node->parent->items[1].num = 0;
		}
		delete node;
        validate();

		return true;
	}

    PairIndex findSplit(Pair* node, int depth)
    {
        for (int idx = 0; idx < 2; ++idx) {
            if (node->items[idx].t == I_NUM && node->items[idx].num > 9) {
                if (DEBUG) cout << "        FOUND SPLIT (" << idx << "): " << node << endl;
                return PairIndex{ node, idx };
            }

            if (node->items[idx].t == I_PAIR) {
                PairIndex ret = findSplit(node->items[idx].p, depth + 1);
                if (ret.p != NULL)
                    return ret;
            }
        }

        return PairIndex{ NULL, 0 };
    }

    bool reduceSplit()
    {
        PairIndex ret = findSplit(root, 0);
        if (ret.p == NULL)
            return false;

		if (DEBUG) cout << "        HAVE NODE" << ret.p << endl;
		int n = ret.p->items[ret.idx].num;
		Pair* newpair = new Pair;
		newpair->parent = ret.p;
		newpair->items[0].t = newpair->items[1].t = I_NUM;
		newpair->items[0].num = n / 2;
		newpair->items[1].num = (n / 2) + (n & 1);

		ret.p->items[ret.idx].t = I_PAIR;
		ret.p->items[ret.idx].p = newpair;
        validate();
        return true;
    }

    void reduce()
    {
        for (;;) {
            if (DEBUG) cout << "    Reduce Explode" << endl;
			while (reduceExplode()) {
				if (DEBUG) cout << "        DID EXPLODE: " << root << endl;
			}

            if (DEBUG) cout << "    Reduce Split" << endl;
            bool found = reduceSplit();
            if (found) {
				if (DEBUG) cout << "        DID SPLIT: " << root << endl;
            }

			if (!found)
				return;
        }
        validate();
    }

    int calcMagnitude(Pair *node)
    {
        int n[2];

        for (int idx = 0; idx < 2; ++idx) {
            if (node->items[idx].t == I_NUM) {
                n[idx] = node->items[idx].num;
            } else {
                n[idx] = calcMagnitude(node->items[idx].p);
            }
        }

		return 3*n[0] + 2*n[1];
    }

	int getMagnitude()
	{
		return calcMagnitude(root);
	}
};

void part2()
{
    string s;
    ifstream in;
    vector<string> numList;

    in.open("math.txt");
    while (getline(in, s)) {
        cout << s << endl;
        numList.push_back(s);
    }

    SnailNum* sf = new SnailNum();
    int maxMag = 0;
    for (int i = 0; i < numList.size(); ++i) {
        for (int j = 0; j < numList.size(); ++j) {
            sf->set(numList[i]);
            sf->add(numList[j]);
            sf->reduce();
            int mag = sf->getMagnitude();
            if (mag > maxMag) {
                maxMag = mag;
            }

            sf->set(numList[j]);
            sf->add(numList[i]);
            sf->reduce();
            mag = sf->getMagnitude();
            if (mag > maxMag) {
                maxMag = mag;
            }
        }
    }

    cout << "Max Magnitude: " << maxMag << endl;
}


void part1()
{
    string s;
    ifstream in;
    vector<string> numList;

    in.open("math.txt");
    while (getline(in, s)) {
        cout << s << endl;
        numList.push_back(s);
    }

    //string test = "[[[[4,3],4],4],[7,[[8,4],9]]]";
    //SnailNum* sf = new SnailNum(test);

    SnailNum* sf = new SnailNum(numList[0]);

    for (int i = 1; i < numList.size(); ++i) {
        if (DEBUG) cout << endl << "ADD: " << numList[i] << endl;
        sf->add(numList[i]);
        if (DEBUG) cout << endl << "    AFTER ADD: " << *sf << endl;
        sf->reduce();
    }
    cout << "Magnitude: " << sf->getMagnitude() << endl;

    exit(0);





#if 0
	//string test = "[[[[1,3],[5,3]],[[1,3],[8,7]]],[[[4,9],[6,9]],[[8,2],[7,3]]]]";
    //string test = "[[[[[9,8],1],2],3],4]";

    //string test = "[[[[0,7],4],[7,[[8,4],9]]],[1,1]]";
    //string test = "[[[[[0,7],4],[7,[[8,4],9]]],[1,1]],0]";
    //string test = "[[[[0,7],4],[[1,2],[[8,4],9]]],[1,1]]";
    //string test = "[[[[0,7],4],[[1,2],[9,[8,4]]]],[1,1]]";
    //string test = "[[[[0,7],4],[[1,2],[[8,4],[[[6,7],5],2]]]],[1,1]]";
    //string test = "[[3,[2,[8,0]]],[9,[5,[4,[3,2]]]]]";

    string test = "[[[[4,3],4],4],[7,[[8,4],9]]]";
    SnailNum* sf = new SnailNum(test);
    cout << "AFTER NEW: " << *sf << endl;
    sf->add("[1,1]");
    cout << "AFTER ADD: " << *sf << endl;

    cout << endl;
    cout << test << endl;
    cout << *sf << endl;

    sf->reduce();
    cout << *sf << endl;
    cout << "Magnitude: " << sf->getMagnitude() << endl;
#endif
}


int main()
{
    part2();
}
