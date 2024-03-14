/**
 * AdventOfCode2021-Day23-Part1.cpp : Advent of Code 2021, Day 1, Part 1
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
#include <unordered_set>
#include <unordered_map>
#include <regex>
#include <queue>

#define FN "map.txt"
bool dsp = false;		// Display final path

using namespace std;

vector<string> str_split(const string& s, const char* pattern) {
    regex sep_regex = regex{ pattern };
    sregex_token_iterator iter(s.begin(), s.end(), sep_regex, -1);
    sregex_token_iterator end;
    return { iter, end };
}

int sgn(int val) {
    return (0 < val) - (val < 0);
}

struct Amphipod {
    int8_t row;
    int8_t col;
    int8_t type;
};
ostream& operator<<(ostream& out, const Amphipod& p) {
    out << "[T:" << (char)(p.type + 'A') << ": " << (int)p.row << ", " << (int)p.col << "]";
    return out;
}

struct Coord {
    uint8_t row;
    uint8_t col;
};

Coord getCoord(int idx) {
    Coord c;
    if (idx < 11) {
	c.row = 1;
	c.col = idx + 1;
    }
    else {
	idx -= 11;
	c.row = 2 + idx % 2;
	c.col = 3 + (idx / 2) * 2;
    }
    return c;
}

int getIndex(int row, int col) {
    if (row == 1)
	return col - 1;
    return 11 + ((col - 3) / 2) * 2 + row - 2;
}

// Return the state of the position at row, col
char getPosState(string& state, int row, int col) {
    return state[getIndex(row, col)];
}

// Unpack pod data from packed state string
void getPods(string& state, Amphipod* pods) {
    int pod_types[4] = { 0, 0, 0, 0 };
    for (int i = 0; i < 19; ++i) {
	char pod = state[i];
	if (pod != '.') {
	    int8_t type = pod - 'A';
	    Coord c = getCoord(i);
	    pods[type * 2 + pod_types[type]] = { (int8_t)c.row, (int8_t)c.col, (int8_t)type };
	    ++pod_types[type];
	}
    }
}

int costs[] = { 1, 10, 100, 1000 };

struct Move {
    int8_t from_row;
    int8_t from_col;
    int8_t to_row;
    int8_t to_col;
    int cost;
};
ostream& operator<<(ostream& out, const Move& m) {
    out << "[From: " << (int)m.from_row << ", " << (int)m.from_col << " to "
	<< (int)m.to_row << ", " << (int)m.to_col << " (" << m.cost << ")";
    return out;
}

struct Node {
    string state;
    int dist;

    Node(string state, int d) : state(state), dist(d) {}
};
ostream& operator<<(ostream& out, const Node& n) {
    out << "[Node " << &n << ": " << n.state << ", d=" << n.dist << "]";
    return out;
}

struct CompareNode {
    bool operator()(const Node* a, const Node* b) const {
	return a->dist > b->dist;
    }
};

void dspNode(Node* node) {
    cout << "############# DIST = " << node->dist << endl;
    cout << "#" << node->state.substr(0, 11) << "#" << endl;
    cout << "###" << node->state[11] << "#" << node->state[13]
	<< "#" << node->state[15] << "#" << node->state[17] << "###" << endl;
    cout << "  #" << node->state[12] << "#" << node->state[14]
	<< "#" << node->state[16] << "#" << node->state[18] << "#" << endl;
}

void part1()
{
    fstream in;
    string s;
    vector<string> charmap;

    in.open(FN);
    while (getline(in, s)) {
	charmap.push_back(s);
    }

    // Parse pods into a state string that linearly represents the pod positions.
    string start_state(19, '.');
    for (int row = 2; row <= 3; ++row) {
	for (int col = 3; col <= 9; col += 2) {
	    char pod = charmap[row][col];
	    int idx = 11 + (row - 2) + ((col - 3) / 2) * 2;
	    start_state[idx] = pod;
	}
    }

    string end_state = "...........AABBCCDD";

    unordered_map<string, Node*> node_map;
    unordered_map<string, string> predecessor;
    unordered_set<string> visited;
    priority_queue<Node*, vector<Node*>, CompareNode> pq;
    Node* start_node = new Node(start_state, 0);
    node_map[start_state] = start_node;

    pq.push(start_node);

    int max_depth = 0;
    int total_loops = 0;
    int total_nodes = 0;
    while (!pq.empty()) {
	++total_loops;
	Node* cur_node = pq.top();
	pq.pop();
	string& state = cur_node->state;
	if (pq.size() > max_depth)
	    max_depth = pq.size();

	if (state == end_state) {
	    cout << "Found " << state << ", dist = " << cur_node->dist << endl;
	    break;
	}
	if (visited.find(state) != visited.end())
	    continue;
	visited.insert(state);

	Amphipod pods[8];
	getPods(state, pods);

	// Generate possible moves
	vector<Move> moves;
	for (auto& p : pods) {
	    int move_cost = costs[p.type];
	    int total_cost = 0;

	    // If in hallway, then only valid move is try for its room
	    if (p.row == 1) {
		int dest_row = -1;
		int dest_col = p.type * 2 + 3;
		int start_col = p.col;
		int move_dir = sgn(dest_col - start_col);
		for (int col = start_col + move_dir; col != dest_col + move_dir; col += move_dir) {
		    // Something blocking, then no move
		    if (getPosState(state, 1, col) != '.') {
			total_cost = -1;
			break;
		    }
		    total_cost += move_cost;
		}
		if (total_cost < 0)
		    continue;               // No valid move for pod
		// Check if valid slot available
		if (getPosState(state, 2, dest_col) != '.')
		    continue;               // Top is closed
		char slot2 = getPosState(state, 3, dest_col);
		if (slot2 != '.') {
		    if (slot2 != (p.type + 'A'))
			continue;           // Wrong pod in slot
		    // OK, we can move to top
		    dest_row = 2;
		    total_cost += move_cost;
		}
		else {
		    // Can move to bottom
		    dest_row = 3;
		    total_cost += move_cost * 2;
		}

		moves.push_back(Move({ (int8_t)p.row, (int8_t)p.col, (int8_t)dest_row, (int8_t)dest_col, total_cost }));

	    }
	    else {
		// Must currently be in a room
		int p_room = p.type * 2 + 3;        // Its proper room

		// Blocked in?
		if (p.row == 3 && getPosState(state, 2, p.col) != '.')
		    continue;               // Yes, no move

		// Currently in its proper column?
		if (p.row == 3 && p.col == p_room)
		    continue;		// Yes, doesn't need to move

		// In its proper column, but pod below it is a different pod?
		// If so then this pod can move
		if (p.row == 2 && p.col == p_room && getPosState(state, 3, p.col) == (p.type + 'A'))
		    continue;               // No, same pod, so stays

		// Valid for pod to move. Generate potential valid moves into
		// the hallway.

		total_cost = (p.row - 1) * move_cost;   // Cost to hallway
		for (int col = p.col - 1; col >= 1; --col) {
		    // Cannot stop in front of any room
		    if (col == 3 || col == 5 || col == 7 || col == 9)
			continue;
		    if (getPosState(state, 1, col) != '.')
			break;

		    moves.push_back(Move({ (int8_t)p.row, (int8_t)p.col, (int8_t)1, (int8_t)col, total_cost + (p.col - col) * move_cost }));
		}
		for (int col = p.col + 1; col <= 11; ++col) {
		    // Cannot stop in front of any room
		    if (col == 3 || col == 5 || col == 7 || col == 9)
			continue;
		    if (getPosState(state, 1, col) != '.')
			break;

		    moves.push_back(Move({ (int8_t)p.row, (int8_t)p.col, (int8_t)1, (int8_t)col, total_cost + (col - p.col) * move_cost }));
		}
	    }
	}

	for (auto m : moves) {
	    int idx1 = getIndex(m.from_row, m.from_col);
	    int idx2 = getIndex(m.to_row, m.to_col);
	    string new_state = state;
	    new_state[idx2] = state[idx1];
	    new_state[idx1] = '.';

	    Node* new_node = NULL;
	    auto it = node_map.find(new_state);
	    if (it != node_map.end()) {
		new_node = it->second;
	    }
	    else {
		++total_nodes;
		new_node = new Node(new_state, 99999999);
		node_map[new_state] = new_node;
	    }
	    if (visited.find(new_state) == visited.end() && new_node->dist > cur_node->dist + m.cost) {
		new_node->dist = cur_node->dist + m.cost;
		pq.push(new_node);
		predecessor[new_state] = state;
	    }
	}
    }

    if (dsp) {
	cout << "PATH:" << endl;
	string trav_state = end_state;
	do {
	    Node* node = node_map[trav_state];
	    cout << *node << endl;
	    dspNode(node);
	    trav_state = predecessor[trav_state];
	} while (trav_state != start_state);

	cout << "total_loops = " << total_loops << ", max_depth = " << max_depth << ", total_nodes = " << total_nodes << endl;
    }
}

int main()
{
    part1();
}
