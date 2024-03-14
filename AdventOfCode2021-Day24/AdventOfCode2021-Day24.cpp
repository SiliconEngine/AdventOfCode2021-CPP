/**
 * AdventOfCode2021-Day24.cpp : Advent of Code 2021, Day 24
 *
 * NOTE: The solutions for 2021 aren't as cleaned up and well documented as other
 * years I've done.
 *
 * @author	Tim Behrendsen
 *
 * This was a decompiling problem, so there is no program solution.
 *
 *        inp w                 w = [inp]
 *        mul x 0               //x = 0         [will be set again later]
 *        add x z               //x = z         [z from last step]
 *        mod x 26              x = z % 26
 *  *     div z 1               z /= [1|26]     [this varies, 1 or 26]
 *  *     add x 13              x += [n]        [varies, 0 - 15]
 *        eql x w               x = (x == w)
 *        eql x 0               x = ! x         [x is now a boolean]
 *        mul y 0               //y = 0
 *        add y 25              y = 25
 *        mul y x               y *= x          [y = 25 or 0]
 *        add y 1               y += 1          [y = 26 or 1]
 *        mul z y               z *= y
 *        mul y 0               //y = 0
 *        add y w               //y = w
 *  *     add y 14              y = w+[n]       [varies, 3-14]
 *        mul y x               y *= x          [x is a boolean, so 0 or w+[n]]
 *        add z y               z += y
 *
 * Algorithm ended up being a stack machine, with the following rules:
 *
 *    Step 1  : PUSH : value[0] + 14
 *    Step 2  : PUSH : value[1] + 8
 *    Step 3  : PUSH : value[2] + 5
 *    Step 4  : POP  : value[3] must be pop() - 0
 *    Step 5  : PUSH : value[4] + 10
 *    Step 6  : POP  : value[5] must be pop() - 13
 *    Step 7  : PUSH : value[6] + 16
 *    Step 8  : POP  : value[7] must be pop() - 9
 *    Step 9  : PUSH : value[8] + 6
 *    Step 10 : PUSH : value[9] + 13
 *    Step 11 : POP  : value[10] must be pop() - 14
 *    Step 12 : POP  : value[11] must be pop() - 3
 *    Step 13 : POP  : value[12] must be pop() - 2
 *    Step 14 : POP  : value[13] must be pop() - 14
 *
 * Which means each digit depends on one other digit:
 *
 *      value[3] = value[2] + 5
 *      value[5] = value[4] + 10 - 13 = -3
 *      value[7] = value[6] + 16 - 9 = +7
 *      value[10] = value[9] + 13 - 14 = -1
 *      value[11] = value[8] + 6 - 3 = +3
 *      value[12] = value[1] + 8 - 2 = +6
 *      value[13] = value[0] + 14 - 14 = 0
 *
 *PART 1 (LARGEST):
 * 0: 9
 * 1: 3
 * 2: 4
 * 3: 9
 * 4: 9
 * 5: 6
 * 6: 2
 * 7: 9
 * 8: 6
 * 9: 9
 * 10: 8
 * 11: 9
 * 12: 9
 * 13: 9
 *
 *PART 2 (SMALLEST):
 * 0: 1
 * 1: 1
 * 2: 1
 * 3: 6
 * 4: 4
 * 5: 1
 * 6: 1
 * 7: 8
 * 8: 1
 * 9: 2
 * 10: 1
 * 11: 4
 * 12: 7
 * 13: 1
 *
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
#include <bitset>

#define DEBUG 0

using namespace std;

vector<string> str_split(const string& s, const char* pattern) {
    regex sep_regex = regex{ pattern };
    sregex_token_iterator iter(s.begin(), s.end(), sep_regex, -1);
    sregex_token_iterator end;
    return { iter, end };
}

ostream& operator<<(ostream& out, const map<string,int>& m) {
    for (const auto& pair : m) {
        out << "    " << pair.first << ": " << pair.second << endl;
    }
    return out;
}

int get_param(map<string,int> &vars, string p) {
    if (p[0] >= 'w' && p[0] <= 'z')
        return vars[p];

    return stoi(p);
}

int algorithm() {
    int inp_list[] = { 1, 3, 5, 7, 9, 2, 4, 6, 8, 9, 9, 9, 9, 9 };
    int vary1[] = { 1, 1, 1, 26, 1, 26, 1, 26, 1, 1, 26, 26, 26, 26 };
    int vary2[] = { 13, 12, 11, 0, 15, -13, 10, -9, 11, 13, -14, -3, -2, -14 };
    int vary3[] = { 14, 8, 5, 4, 10, 13, 16, 5, 6, 13, 6, 7, 13, 3 };
    int w = 0, x = 0, y = 0, z = 0;

//    for (int step = 0; step < 14; ++step) {
//        w = inp_list[step];
//        if (((z % 26) + vary2[step]) != w) {
//            z = (z / vary1[step]) * 26 + w + vary3[step];
//        } else {
//            z /= vary1[step];		// 1 or 26
//	}
//    }

    w = inp_list[0];
    z = w + 14;                     // z in range 15-23

    w = inp_list[1];
//    if (((z % 26) + 12) != w) {	// always true
//        z = z * 26 + w + 8;
//    }
    z = z * 26 + w + 8;             // z in range (15-23) * 26 + w + 8

    w = inp_list[2];
//    if (((z % 26) + 11) != w) {       // always true
//        z = z * 26 + w + 5;
//    }
    z = z * 26 + w + 5;             // z in range ((15-23)*26+w+8) * 26 + w +5

    w = inp_list[3];
    if (((z % 26) + 0) != w) {
        z = (z / 26) * 26 + w + 4;
    } else {
        z /= 26;
    }

    w = inp_list[4];
//    if (((z % 26) + 15) != w) {           // +15 != w
//        z = z * 26 + w + 10;
//    }
    z = z * 26 + w + 10;

    w = inp_list[5];
    if (((z % 26) + -13) != w) {
        z = (z / 26) * 26 + w + 13;
    } else {
        z /= 26;
    }

    w = inp_list[6];
//    if (((z % 26) + 10) != w) {             // +10 != w
//        z = z * 26 + w + 16;
//    }
    z = z * 26 + w + 16;

    w = inp_list[7];
    if (((z % 26) + -9) != w) {
        z = (z / 26) * 26 + w + 5;
    } else {
        z /= 26;
    }

    w = inp_list[8];
//    if (((z % 26) + 11) != w) {
//        z = z * 26 + w + 6;
//    }
    z = z * 26 + w + 6;

    w = inp_list[9];
//    if (((z % 26) + 13) != w) {
//        z = z * 26 + w + 13;
//    }
    z = z * 26 + w + 13;

    w = inp_list[10];
    if (((z % 26) + -14) != w) {
        z = (z / 26) * 26 + w + 6;
    } else {
        z /= 26;
    }

    w = inp_list[11];
    if (((z % 26) + -3) != w) {
        z = (z / 26) * 26 + w + 7;
    } else {
        z /= 26;
    }

    w = inp_list[12];
    if (((z % 26) + -2) != w) {
        z = (z / 26) * 26 + w + 13;
    } else {
        z /= 26;
    }

    w = inp_list[13];
    if (((z % 26) + -14) != w) {
        z = (z / 26) * 26 + w + 3;
    } else {
        z /= 26;
    }

//    for (int step = 0; step < 14; ++step) {
//        w = inp_list[step];
//
//        //x = ((z % 26) + vary2[step]) != w;
//        //y = 25 * x + 1;
//        //z /= vary1[step];
//        //z *= y;
//        //y = w + vary3[step];
//        //y *= x;
//        //z += y;
//
//        if (((z % 26) + vary2[step]) != w) {
//            // x == 1
//            //z /= vary1[step];           // 1 or 26
//            //z *= 26;
//            //y = w + vary3[step];
//	    //z += y;
//            z = (z / vary1[step]) * 26 + w + vary3[step];
//        } else {
//            // x == 0
//            //y = w + vary3[step];
//            //y *= x;
//	    //z += y;
//            z /= vary1[step];		// 1 or 26
//	}
//
//    }

    return z;
}
void part1()
{
    string s;
    ifstream in;

    int inp_data[] = { 1,3,5,7,9,2,4,6,8,9,9,9,9,9 };
    int inp_idx = 0;
    map<string, int> vars = {
        { "w", 0 },
        { "x", 0 },
        { "y", 0 },
        { "z", 0 },
    };

    in.open("code.txt");
    while (getline(in, s)) {
        cout << s << endl;
        vector<string> v = str_split(s, " ");
        string op = v[0];
        string param1 = v[1];
        cout << "OP: " << op << " P:" << param1 << endl;

        if (op == "inp") {
            vars[param1] = inp_data[inp_idx++];

        } else if (op == "add") {
            vars[param1] = vars[param1] + get_param(vars, v[2]);
        } else if (op == "mul") {
            vars[param1] = vars[param1] * get_param(vars, v[2]);
        } else if (op == "div") {
            vars[param1] = vars[param1] / get_param(vars, v[2]);
        } else if (op == "mod") {
            vars[param1] = vars[param1] % get_param(vars, v[2]);
        } else if (op == "eql") {
            vars[param1] = vars[param1] == get_param(vars, v[2]);
        }
	cout << vars << endl;
    }

    cout << algorithm() << endl;
}

int main()
{
    part1();
}
