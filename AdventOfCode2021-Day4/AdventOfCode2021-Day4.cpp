/**
 * AdventOfCode2021-Day4.cpp : Advent of Code 2021, Day 4 (parts 1 and 2)
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

class Card {
private:
    int spaces[5][5];
    bool checked[5][5];

public:
    int boardNum;

    Card() {
        boardNum = 0;
        for (int i = 0; i < 5; ++i) {
            for (int j = 0; j < 5; ++j) {
                spaces[i][j] = 0;
                checked[i][j] = 0;
            }
        }
    }

    void set(int r, int c, int n) {
        spaces[r][c] = n;
    }

    int get(int r, int c) {
        return spaces[r][c];
    }

    bool isChecked(int r, int c) {
        return checked[r][c];
    }

    void setChecked(int r, int c, bool state) {
        checked[r][c] = state;
    }

    bool findAndSet(int n) {
        for (int r = 0; r < 5; ++r) {
            for (int c = 0; c < 5; ++c) {
                if (spaces[r][c] == n) {
                    checked[r][c] = true;
                    return true;
                }
            }
        }
        return false;
    }

    bool testBingo() {
        // Check horizontal
        for (int r = 0; r < 5; ++r) {
			bool hasBingo = true;
            for (int c = 0; c < 5; ++c) {
                if (!checked[r][c]) {
                    hasBingo = false;
                    break;
                }
            }
            if (hasBingo) {
                return true;
            }
        }

        // Check vertical
        for (int c = 0; c < 5; ++c) {
			bool hasBingo = true;
            for (int r = 0; r < 5; ++r) {
                if (!checked[r][c]) {
                    hasBingo = false;
                    break;
                }
            }
            if (hasBingo) {
                return true;
            }
        }
        return false;
    }

    int calcScore()
    {
        int score = 0;
        for (int r = 0; r < 5; ++r) {
            for (int c = 0; c < 5; ++c) {
                if (!checked[r][c]) {
                    score += spaces[r][c];
                }
            }
        }
        return score;
    }

    void dump()
    {
        cout << "CARD" << endl;
        for (int r = 0; r < 5; ++r) {
            for (int c = 0; c < 5; ++c) {
                char fmt[20];
                sprintf_s(fmt, "%2d", spaces[r][c]);
                if (checked[r][c]) {
                    cout << "[" << fmt << "]";
                } else {
                    cout << " " << fmt << " ";
                }
            }
            cout << endl;
        }
    }


};

void step2()
{
    ifstream fh("bingo.txt");
    string s;
    vector<Card*> cards;

    getline(fh, s);

    vector<string> list = str_split(s, ",");
    vector<int> numList;
    for (auto n : list) {
        numList.push_back(stoi(n));
    }

    int numCards = 0;
    while (getline(fh, s)) {
		Card* card = new Card;
        card->boardNum = ++numCards;

        for (int row = 0; row < 5; ++row) {
			getline(fh, s);
			cout << s << endl;
            for (int col = 0; col < 5; ++col) {
                card->set(row, col, stoi(s.substr(col * 3, 2)));
            }
        }

		cards.push_back(card);
    }

    bool* hadBingo = new bool[numCards];
    for (int i = 0; i < numCards; ++i)
        hadBingo[i] = false;

    Card* lastCard = NULL;
    int lastScore = 0;
    int lastProd = 0;
    for (auto n : numList) {
        for (auto card : cards) {
            if (hadBingo[card->boardNum-1])
                continue;

            bool had = card->findAndSet(n);
            if (had) {
                card->dump();
                if (card->testBingo()) {
                    lastCard = card;
					lastScore = lastCard->calcScore();
					lastProd = lastScore * n;
                    hadBingo[card->boardNum - 1] = true;
                    cout << "BINGO! Card #" << card->boardNum << " Score=" << lastScore << ", prod = " << lastProd << endl;
                }
            }
        }
    }

	cout << "LAST BINGO! Board was " << lastCard->boardNum << ", Score is " << lastScore << ", prod is " << lastProd << endl;
	exit(0);

}

void step1()
{
    ifstream fh("bingo.txt");
    string s;
    vector<Card*> cards;

    getline(fh, s);

    vector<string> list = str_split(s, ",");
    vector<int> numList;
    for (auto n : list) {
        numList.push_back(stoi(n));
    }

    while (getline(fh, s)) {
		Card* card = new Card;

        for (int row = 0; row < 5; ++row) {
			getline(fh, s);
			cout << s << endl;
            for (int col = 0; col < 5; ++col) {
                card->set(row, col, stoi(s.substr(col * 3, 2)));
            }
        }

		cards.push_back(card);
    }

    for (auto n : numList) {
        for (auto card : cards) {
            bool had = card->findAndSet(n);
            if (had) {
                cout << "HAD " << n << endl;
                card->dump();
                if (card->testBingo()) {
                    int score = card->calcScore();
                    int prod = score * n;
                    cout << "BINGO! Score is " << score << ", prod is " << prod << endl;
                    exit(0);
                }
            }
        }
    }

}

int main()
{
    step2();
}
