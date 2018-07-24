#ifndef MOVE_H
#define MOVE_H

#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <unordered_map>
#include <utility>
#include <functional>
#include <stack>


using namespace std;

class Move {
public:
	int prev_row, prev_col;
	int curr_row, curr_col;
	string prev_char;


	void set_values(int prev_row_in, int prev_col_in, int curr_row_in, int curr_col_in, string prev_char_in) {
		prev_row = prev_row_in;
		prev_col = prev_col_in;   //where the piece came from

		prev_char = prev_char_in; //character that was deleted due to piece movement

		curr_col = curr_col_in;
		curr_row = curr_row_in;
	}
};

#endif
