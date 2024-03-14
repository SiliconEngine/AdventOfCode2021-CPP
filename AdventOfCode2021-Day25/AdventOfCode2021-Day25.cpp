/**
 * AdventOfCode2021-Day25.cpp : Advent of Code 2021, Day 25
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
#include <chrono>

#define FN "sea.txt"

using namespace std;

class Cucumber {
public:
    int row;
    int col;
    int new_row;
    int new_col;
    char type;

    Cucumber(int row, int col, char t) : row(row), col(col), new_row(-1), new_col(-1), type(t) {}
};

int do_moves(vector<string>& seamap, vector<Cucumber*>& cuc_list) {
    int num_rows = seamap.size();
    int num_cols = seamap[0].length();

    vector<Cucumber*> move_list;
    for (Cucumber* c : cuc_list) {
        int new_row = c->row, new_col = c->col;
        if (c->type == '>') {
            if (++new_col >= num_cols)
                new_col = 0;
        } else {
            if (++new_row >= num_rows)
                new_row = 0;
        }

        if (seamap[new_row][new_col] == '.') {
            c->new_row = new_row;
            c->new_col = new_col;
            move_list.push_back(c);
        }
    }

    for (Cucumber* c : move_list) {
        seamap[c->new_row][c->new_col] = c->type;
        seamap[c->row][c->col] = '.';
        c->row = c->new_row;
        c->col = c->new_col;
    }

    return move_list.size();
}

int main()
{
    string s;
    ifstream in;

    in.open(FN);
    vector<string> seamap;
    vector<Cucumber*> east_list;
    vector<Cucumber*> south_list;
    int row = 0;
    while (getline(in, s)) {
        seamap.push_back(s);
        for (int col = 0; col < s.length(); ++col) {
            if (s[col] != '.') {
                Cucumber* c = new Cucumber(row, col, s[col]);
                if (c->type == '>')
		    east_list.push_back(c);
                else
		    south_list.push_back(c);
            }
        }
        ++row;
    }

    auto start = std::chrono::high_resolution_clock::now();

    int move = 0;
    int total_moved = 0;
    do {
        ++move;
        total_moved = do_moves(seamap, east_list);
        total_moved += do_moves(seamap, south_list);
    } while (total_moved != 0);

    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Elapsed time: " << elapsed.count() << " microseconds." << std::endl;

    cout << "number of moves was " << move << endl;
}
