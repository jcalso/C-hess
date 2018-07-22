#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <unordered_map>
#include <utility>
#include <functional>
#include <stack>
#include "UndoEntity.h"

using namespace std;

class ChessBoard {
public:

	int move_count = 0;
	int draw_move_count = 0;
	bool black_in_check = false;
	bool white_in_check = false;

	bool white_can_castle_QS = true;
	bool white_can_castle_KS = true;
	bool black_can_castle_QS = true;
	bool black_can_castle_KS = true;


	vector<vector<string>> Board;

	unordered_map<string, int>coords; //handles matrix column coordinate conversion, letter -> number

	stack<UndoEntity>Undo;


	void init() {

		string space = " ";
		string letter_coords = "abcdefgh";

		for (unsigned int i = 0; i < letter_coords.size(); i++) {//PGN to Matrix coordinate conversion
			coords.insert(make_pair(letter_coords.substr(i, 1), i * 2 + 1));
		}

		Board.resize(18);
		for (unsigned int i = 0; i < Board.size(); i++) {
			Board[i].resize(18);
		}

		for (unsigned int i = 0; i < Board.size(); i++) {
			for (unsigned int j = 0; j < Board.size(); j++) {
				if (j != 0 && Board[i][j - 1] == "|" && j != 17) { //set spaces in empty squares
					Board[i][j] = "   ";
				}
				else if (i % 2 == 0 && j % 2 == 1 && j != 17) { //Set horizontal borders
					Board[i][j] = "---";
				}
				else if (i % 2 == 1 && j % 2 == 0 && i != 17) {//set vertical borders
					Board[i][j] = "|";
				}


				else if (j == 17 && i % 2 == 1 && i != 17) { //set number coordinates on edge of board
					Board[i][j] = space + to_string(9 - (i + 1) / 2) + space;
				}
				else if (i == 17 && j % 2 == 1 && j != 17) {//set letter coordinates on edge of board
					Board[i][j] = space + letter_coords[(j + 1) / 2 - 1] + space;
				}
				else if (i % 2 == 0 && j % 2 == 0 && (i&&j) != (0) && i != 16 && j != 16) {
					Board[i][j] = "+";
				}
				else {
					Board[i][j] = " ";
				}
			}
		}

	}

	void insert_pieces() {

		for (unsigned int i = 0; i < Board.size() - 1; i++) {
			for (unsigned int j = 0; j < Board.size() - 1; j++) {

				if (i == 3 && j % 2 == 1) { //insert black pawns
					Board[i][j] = " p ";
				}
				else if (i == 13 && j % 2 == 1) { //insert white pawns
					Board[i][j] = " P ";
				}

			}
		}

		//insert black pieces
		Board[1][1] = " r "; Board[1][15] = " r ";
		Board[1][3] = " n "; Board[1][13] = " n ";
		Board[1][5] = " b "; Board[1][11] = " b ";
		Board[1][7] = " q "; Board[1][9] = " k ";

		//insert white pieces
		Board[15][1] = " R "; Board[15][15] = " R ";
		Board[15][3] = " N "; Board[15][13] = " N ";
		Board[15][5] = " B "; Board[15][11] = " B ";
		Board[15][7] = " Q "; Board[15][9] = " K ";

	}

	void print() {
		cout << endl;
		for (unsigned int i = 0; i < Board.size(); i++) {
			for (unsigned int j = 0; j < Board.size(); j++) {
				cout << Board[i][j];
			}
			cout << endl;
		}
	}

	void flip_board() {
		for (unsigned int i = 0; i < Board.size() / 2; i++) {
			for (unsigned int j = 0; j < Board.size(); j++) {
				if (i != 17) {
					swap(Board[i][j], Board[Board.size() - 2 - i][j]);
				}
			}
		}
		for (unsigned int i = 0; i < Board.size(); i++) {
			for (unsigned int j = 0; j < Board.size() / 2; j++) {
				if (j != 17) {
					swap(Board[i][j], Board[i][Board.size() - 2 - j]);
				}
			}
		}
	}

	int convert_to_matrix_col(string board_row) { //handles coordinate conversion with numbers(rows)
		if (move_count % 2 == 0) {
			return (8 - stoi(board_row)) * 2 + 1;
		}
		else {
			return stoi(board_row) * 2 - 1;
		}
	}

	void move_piece(string initial_pos, string final_pos) {
		int row_i, col_i, row_f, col_f;

		row_i = convert_to_matrix_col(initial_pos.substr(1, 1));
		row_f = convert_to_matrix_col(final_pos.substr(1, 1));
		if (move_count % 2 == 0) {
			col_f = coords[final_pos.substr(0, 1)];
			col_i = coords[initial_pos.substr(0, 1)];
		}
		else {
			col_f = 8 - (abs(8 - coords[final_pos.substr(0, 1)]));  //SOMETHING IS VERY WRONG HERE
			col_i = 8 - (abs(8 - coords[initial_pos.substr(0, 1)]));
		}

		if (can_move_here(row_f, col_f, row_i, col_i)) {

			//update undo data stack
			UndoEntity newUndo;
			newUndo.set_values(row_i, col_i, row_f, col_f, Board[row_f][col_f]);
			Undo.push(newUndo);

			//if a piece wasn't taken, increment 50 move draw count
			if (Board[row_f][col_f] != "   ") {
				draw_move_count++;
			}
			else {
				draw_move_count = 0;
			}

			//move the target piece
			Board[row_f][col_f] = Board[row_i][col_i];
			Board[row_i][col_i] = "   ";

			flip_board();
			move_count++;

		}

		else {
			cout << "Error: piece cannot move here" << endl;
		}

	}

	void undo_move() {
		Board[Undo.top().prev_row][Undo.top().prev_col] = Board[Undo.top().curr_row][Undo.top().curr_col];
		Board[Undo.top().curr_row][Undo.top().curr_col] = Undo.top().prev_char;
		Undo.pop();
		move_count--;
	}

	bool can_move_here(int row_f, int col_f, int row_i, int col_i) {
		/*	cout << row_i<< col_i<< row_f<< col_f;
		string piece = Board[row_i][col_i];
		string target = Board[row_f][col_f];
		cout << piece << Board[row_f][col_f];
		if(piece == " P " || piece == " p "){
		if((row_f == row_i - 2 && target == "   " )|| (target != "   " && (row_f == row_i-2) && (col_f == col_i+2)) ||( target != "   " && (row_f == row_i-2) && col_f == col_i-2)){
		return true;
		}
		if((piece == " P " && row_i == 13 && row_f == 9) || (piece == " p " && row_i == 14 && row_f == 10)){

		return true;

		}
		}
		return false;*/
		return true;
	}

}; //Class Checkerboard

#endif
