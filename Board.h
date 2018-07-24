#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <cmath>
#include <vector>
#include <unordered_map>
#include <stack>
#include <string>
#include <utility>
#include <functional>
#include "Move.h"

using namespace std;

class ChessBoard {
public:

	int move_count = 0;
	int draw_move_count = 0;

	vector<vector<string>> Board;
	unordered_map<string, int>coords; //handles matrix column coordinate conversion, letter -> number
	stack<Move>Undo;

	void init() { //Creates an empty 8x8 board

		string space = " ";
		string letter_coords = "abcdefgh";

		for (unsigned int i = 0; i < letter_coords.size(); i++) { //PGN to Matrix coordinate conversion
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

	int convert_to_matrix_row(string board_row) { //handles coordinate conversion with numbers(rows)
		if (move_count % 2 == 0) {
			return (8 - stoi(board_row)) * 2 + 1;
		}
		else {
			return stoi(board_row) * 2 - 1;
		}
	}

	Move create_move(string initial_pos, string final_pos) {
		int row_i, col_i, row_f, col_f;

		row_i = convert_to_matrix_row(initial_pos.substr(1, 1));
		row_f = convert_to_matrix_row(final_pos.substr(1, 1));
		if (move_count % 2 == 0) {
			col_f = coords[final_pos.substr(0, 1)];
			col_i = coords[initial_pos.substr(0, 1)];
		}
		else {
			col_f = 16 - (coords[final_pos.substr(0, 1)]);  //SOMETHING IS VERY WRONG HERE
			col_i = 16 - (coords[initial_pos.substr(0, 1)]);
		}

		//update move data stack
		Move newMove;
		newMove.set_values(row_i, col_i, row_f, col_f, Board[row_f][col_f]);
		return newMove;
	}

	void move_piece(string initial_pos, string final_pos) {

		Move move = create_move(initial_pos, final_pos);
		if (can_move_here(move.curr_row, move.curr_col, move.prev_row, move.prev_col)) {

			Undo.push(move);
			//if a piece wasn't taken, increment 50 move draw count
			if (Board[move.curr_row][move.curr_col] == "   ") {
				draw_move_count++;
			}
			else {
				draw_move_count = 0;
			}

			//move the target piece
			Board[move.curr_row][move.curr_col] = Board[move.prev_row][move.prev_col];
			Board[move.prev_row][move.prev_col] = "   ";

			flip_board();
			move_count++;
		}
		else {
			cout << endl << "Error: piece cannot move here" << endl;
		}

	}

	void undo_move() {
		Board[Undo.top().prev_row][Undo.top().prev_col] = Board[Undo.top().curr_row][Undo.top().curr_col];
		Board[Undo.top().curr_row][Undo.top().curr_col] = Undo.top().prev_char;
		Undo.pop();
		move_count--;
	}
	
	bool piece_is_wrong_color(string piece) {

		string white_pieces = "PRNBQK";
		string black_pieces = "prnbqk";

		if (move_count % 2 == 0) {
			for (unsigned int i = 0; i < black_pieces.length(); i++) {
				if (piece == black_pieces.substr(i, 1)) {
					return true;
				}
			}
		}
		else {
			for (unsigned int i = 0; i < white_pieces.length(); i++) {
				if (piece == white_pieces.substr(i, 1)) {
					return true;
				}
			}
		}
		return false;
	}

	bool target_is_wrong_color(string target) {
		string white_pieces = "PRNBQK";
		string black_pieces = "prnbqk";

		if (move_count % 2 == 0) {
			for (unsigned int i = 0; i < white_pieces.length(); i++) {
				if (target == " " + white_pieces.substr(i, 1) + " ") {
					return true;
				}
			}
		}
		else {
			for (unsigned int i = 0; i < black_pieces.length(); i++) {
				if (target == " " + black_pieces.substr(i, 1) + " ") {
					return true;
				}
			}
		}
		return false;
	}

	double distance(int x1, int y1, int x2, int y2) {
		return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
	}
	bool piece_in_the_way(int row_f, int col_f, int row_i, int col_i) {
		if (col_f == col_i && row_f > row_i) { //Piece travels N
			row_i += 2;
			while (row_i != row_f) {
				if (Board[row_i][col_f] != "   ") {
					return true;
				}
				row_i += 2;
			}
			
		}
		if (col_f == col_i && row_f < row_i) { //Piece travels S
			row_i -= 2;
			while (row_i != row_f) {
				if (Board[row_i][col_f] != "   ") {
					return true;
				}
				row_i -= 2;
			}

		}
		if (row_f == row_i && col_f > col_i) { //Piece travels E
			col_i += 2;
			while (col_i != col_f) {
				if (Board[row_i][col_f] != "   ") {
					return true;
				}
				row_i += 2;
			}
		} 
		if (row_f == row_i && col_f < col_i) { //Piece travels W
			col_i -= 2;
			while (col_i != col_f) {
				if (Board[row_i][col_f] != "   ") {
					return true;
				}
				row_i -= 2;
			}
		}

		if (col_f > col_i && row_f > row_i) { //Piece travels NE
			row_i += 2;
			col_i += 2;
			while (row_i != row_f && col_i != col_f) {
				if (Board[row_i][col_f] != "   ") {
					return true;
				}
				row_i += 2;
				col_i += 2;

			}

		}
		if (col_f > col_i && row_f < row_i) { //Piece travels NE
			row_i -= 2;
			col_i += 2;
			while (row_i != row_f && col_i != col_f) {
				if (Board[row_i][col_f] != "   ") {
					return true;
				}
				row_i -= 2;
				col_i += 2;

			}

		}
		if (col_f < col_i && row_f < row_i) { //Piece travels NE
			row_i -= 2;
			col_i -= 2;
			while (row_i != row_f && col_i != col_f) {
				if (Board[row_i][col_f] != "   ") {
					return true;
				}
				row_i -= 2;
				col_i -= 2;

			}

		}
		if (col_f < col_i && row_f > row_i) { //Piece travels NE
			row_i += 2;
			col_i -= 2;
			while (row_i != row_f && col_i != col_f) {
				if (Board[row_i][col_f] != "   ") {
					return true;
				}
				row_i += 2;
				col_i -= 2;

			}

		}
		return false;
	}
	bool can_move_here(int row_f, int col_f, int row_i, int col_i) {
		
		string piece = Board[row_i][col_i];
		string target = Board[row_f][col_f];
		//cout << row_i << col_i << endl << row_f << col_f;
		if (piece_is_wrong_color(piece)) {
			return false;
		}
		if (piece == "   "){
			return false;
		}
		if (target_is_wrong_color(target)) {
			return false;
		}
		if (row_f == row_i && col_i == col_f) {
			return false;
		}
		if (piece_in_the_way(row_f, col_f, row_i, col_i) && piece != " N " && piece != " n ") {
			return false;
		}
		if (piece == " P " || piece == " p ") { //pawn move check
			if (row_i == 13) {
				if (row_f == row_i - 4 && Board[row_f][col_f] == "   " || (row_f == row_i - 2 && col_i == col_f && Board[row_f][col_f] == "   ") || (row_f == row_i - 2 && (col_f == col_i + 2 || col_f == col_i - 2) && Board[row_f][col_f] != "   "))
					return true;
			}
			else if ((row_f == row_i - 2 && col_i == col_f && Board[row_f][col_f] == "   ") || (row_f == row_i - 2 && (col_f == col_i + 2 || col_f == col_i - 2) && Board[row_f][col_f] != "   ")) {
				return true;
			}
			return false;
		}
		if (piece == " N " || piece == " n ") {//knight move check
			double d = 4.47213595499958;
				return abs(distance(row_f, col_f, row_i, col_i) - d) < .001;
		}
		if (piece == " K " || piece == " k ") {
			double d1 = 2.0;
			double d2 = 2.8284271;
			return abs(distance(row_f, col_f, row_i, col_i) - d1) < .001 || abs(distance(row_f, col_f, row_i, col_i) - d2) < .001;
		}
		return true;
	}

}; //Class Checkerboard

#endif
