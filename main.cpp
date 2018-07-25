/* Copyright (c) 2018 Jonathan Calso

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.*/

#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <unordered_map>
#include <utility>
#include <functional>
#include <cctype>
#include <stack>
#include "Board.h"

using namespace std;

void print_start_message() {

	cout << "Welcome to C++hess, type 'start' to begin a game, type 'help' for instructions, type 'quit' to quit." << endl << "->";

	string command;
	cin >> command;

	while (command != "start") {
		if (command == "help") {
			cout << endl;
			cout << "Lowercase letters are black pieces, Uppercase letters are white" << endl;
			cout << "p - pawn, r - rook, n - knight, b - bishop, q - queen, k - king" << endl;
			cout << "The board coordinate system uses letters for files(columns), and numbers for ranks(rows)." << endl;
			cout << "Your options at the beginning of each turn are move, undo, or resign" << endl;
			cout << "To move, Enter two sets of coordinates to move a piece." << endl;
			cout << "For example, move piece at e2 to e4" << endl;
			cout << "To undo a move type 'undo' in the first prompt" << endl;
			cout << "To resign, type 'resign' in the first prompt" << endl << endl;

		}
		else if (command == "quit") {
			exit(0);
		}
		else if (command == "start") {
			return;
		}
		else {
			cout << endl << "Error: Invalid command" << endl;

		}
		cout << "Type 'start' to begin a game, type 'help' for instructions, type 'quit' to quit." << endl << "->";
		cin >> command;
	}

}

bool pos_is_valid(string coord) {
	string letters = "abcdefgh12345678";
	int count = 0;
	for (unsigned int i = 0; i < letters.size(); i++) {
		if (letters[i] == coord[0] && i < 8) {
			count++;
		}
		if (letters[i] == coord[1] && i >= 8) {
			count++;
		}
	}
	return (count == 2);
}

int main() {

	ChessBoard b;
	string command;

	print_start_message();
	b.init();
	b.insert_pieces();

	while (b.white_king_alive && b.black_king_alive) {

		b.print();

		if (b.move_count % 2 == 0) {
			cout << "Move " << (b.move_count + 2)/2 << " - White to play" << endl;
		}
		else {
			cout << "Move " << (b.move_count + 2)/2 << " - Black to play" << endl;
		}

		cout << "Move piece at: ";
		string initial_pos, final_pos;
		cin >> initial_pos;

		if (initial_pos == "undo") {
			if (b.move_count == 0 && b.Undo.empty()) {
				cout << endl << "Error: cannot undo any further";
				continue;
			}
			else {
				b.flip_board();
				b.undo_move();
			}
		}
		else if (initial_pos == "resign") {
			if (b.move_count % 2 == 0) {
				cout << "White has resigned, Black wins!";
			}
			else {
				cout << "Black has resigned, White wins!";

			}
			break;
		}
		else {
			
			cout << "to: ";
			cin >> final_pos;

			if (isdigit(initial_pos[0])) {
				
				reverse(initial_pos.begin(), initial_pos.end());
			}
			if (isdigit(final_pos[0])) {

				reverse(final_pos.begin(), final_pos.end());
			
			}
			if (pos_is_valid(initial_pos) && pos_is_valid(final_pos)) {

				b.move_piece(initial_pos, final_pos);

			}
			else {
				cout << endl << "Error: invalid coordinate." << endl;
			}
		}
	}
	if (!b.white_king_alive) {
		b.print();
		cout << "White king captured, Black Wins!";
	}
	if (!b.black_king_alive) {
		b.print();

		cout << "Black king captured, White Wins!";
	}
	if (b.draw_move_count == 50) {
		b.print();
		cout << "The game has ended in a draw: 50 moves without a piece taken";
	}


	return 0;
}
