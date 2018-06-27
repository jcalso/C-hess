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

//NOTE: Program compiles in online compiler repl.it, have not tested/debugged in other IDE's yet

#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <unordered_map>
#include <utility>
#include <functional>
#include <stack>
using namespace std;

class UndoEntity{
	public:
	int prev_row, prev_col;
	int curr_row, curr_col;
	string prev_char;

	
	void set_values(int prev_row_in, int prev_col_in, int curr_row_in, int curr_col_in, string prev_char_in){
		prev_row = prev_row_in;		
		prev_col = prev_col_in;   //where the piece came from
		
		prev_char = prev_char_in; //character that was deleted due to piece movement
		
		curr_col = curr_col_in;
		curr_row = curr_row_in;
	}
};
class ChessBoard{
	public:
		int move_count = 0;
		
	vector<vector<string>> Board;
	
	unordered_map<string, int>coords; //handles matrix column coordinate conversion, letter -> number
	
	stack<UndoEntity>Undo; 
	
	
	void init(){
			Board.resize(18);
			string space = " ";
			string letter_coords = "abcdefgh";
			
		for( int i = 0; i < letter_coords.size(); i++){//PGN to Matrix coordinate conversion
			coords.insert(make_pair<string, int>(letter_coords.substr(i,1), i*2+1));
		
		}
		for(int i  = 0; i < Board.size(); i++){
				Board[i].resize(18);
		}
		
		for(int i  = 0; i < Board.size(); i++){

		
			for (int j = 0; j < Board.size(); j++){
				if(Board[i][j-1]== "|" && j != 17){ //set spaces between borders
					Board[i][j] = "   ";
				}
				else if(i%2 == 0 && j%2 == 1 && j!= 17){ //Set horizontal border
					Board[i][j] = "---";
				}
				else if(i%2 == 1 && j%2 == 0 && i != 17){//set vertical borders
					Board[i][j] = "|";
				}
				
				
				else if(j == 17 && i%2 == 1 && i != 17){ //set number coordinates
					Board[i][j] = space+to_string(9-(i+1)/2);
				}
			  else if(i == 17 && j%2 == 1 && j != 17){//set letter coordinates
			  	Board[i][j] =  space + letter_coords[(j+1)/2-1] + space;
			  }
		
				else {
					Board[i][j] = " ";
				}
			}
		}
		
	}
	
	void insert_pieces(){
		for(int i  = 0; i < Board.size()-1; i++){

		
			for (int j = 0; j < Board.size()-1; j++){
				if (i == 3 && j%2 == 1){
					Board[i][j] = " p ";
				}
				else if (i == 13 && j%2 == 1){
					Board[i][j] = " P ";
				}
			
			}
		}
		Board[1][1] = " r "; Board[1][15] = " r ";
		Board[1][3] = " n "; Board[1][13] = " n ";
		Board[1][5] = " b "; Board[1][11] = " b ";
		Board[1][7] = " q "; Board[1][9] = " k ";
		
		Board[15][1] = " R "; Board[15][15] = " R ";
		Board[15][3] = " N "; Board[15][13] = " N ";
		Board[15][5] = " B "; Board[15][11] = " B ";
		Board[15][7] = " Q "; Board[15][9] = " K ";
	}
	
	void print(){
		cout << endl;
		for(int i  = 0; i < Board.size(); i++){

			for (int j = 0; j < Board.size(); j++){
				cout << Board[i][j];
			}
				cout << endl;
		}
	
	}
	
	void flip_board(){
		for(int i = 0; i < Board.size()/2; i++){
			for (int j = 0; j < Board.size(); j++){
				swap(Board[i][j], Board[Board.size()-1-i][j]);
			}
		}
	}
	
	int convert_to_matrix_col(string board_row){ //handles coordinate conversion with numbers(rows)
		if(move_count%2 == 0){
			return (8-stoi(board_row))*2+1;
		}
		else{
			return stoi(board_row)*2;
		}
	}
	
	void move_piece(string initial_row_in, string initial_col_in, string final_row_in, string final_col_in){
		int row_i, col_i, row_f, col_f;
		
			row_i = convert_to_matrix_col(initial_row_in);
			row_f= convert_to_matrix_col(final_row_in);
			
			col_f = coords[final_col_in];
			col_i = coords[initial_col_in];
		

	UndoEntity newUndo;
	newUndo.set_values(row_i, col_i,row_f, col_f, Board[row_f][col_f]);
	Undo.push(newUndo);
	
		Board[row_f][col_f] = Board[row_i][col_i];
		Board[row_i][col_i] = "   ";
	}
	
	void undo_move(){
		Board[Undo.top().prev_row][Undo.top().prev_col] = Board[Undo.top().curr_row][Undo.top().curr_col];
		Board[Undo.top().curr_row][Undo.top().curr_col] = Undo.top().prev_char;
		Undo.pop();
		move_count--;
	}
	
	
};

void print_start_message(){
	
	cout << "Welcome to C++hess, type 'start' to begin a game, type 'help' for instructions, type 'quit' to quit."<< endl;
	
	string command;

	
	while (command != "start"){
		cout << "->";
		cin >> command;
		if (command == "help"){
			cout << "Your options at the beginning of each turn are 'move', 'undo', or 'resign'.\n To move, Enter two sets of coordinates to move a piece." << endl;
			cout << "Lowercase letters are black pieces, Uppercase letters are white" << endl;
			cout << "p - pawn, r - rook, n - knight, b - bishop, q - queen, k - king"<<endl;
			cout << "The board coordinate system uses letters for files(columns), and numbers for ranks(rows)." << endl;
		}
		else if(command == "quit"){
			exit(0);
		}
		else if(command == "start"){
			return;
		}
		else{
			cout << "Error: Invalid command" << endl;
		}
	}
	
}
int main (){
	ChessBoard b;
	
	bool checkmate = false;
	bool white_can_castle =true;
	bool black_can_castle = true;
	string command;
	
	
	print_start_message();
	b.init();
	b.insert_pieces(); 
	
	
	while(!checkmate){
		b.print();
	
	
		if(b.move_count %2 == 0){
			cout << "Move " << b.move_count + 1 << " - White to play" <<endl;
		}
		else {
			cout << "Move " << b.move_count + 1 << " - Black to play" << endl;
		}
	
		
	
		cout << "Piece at column: ";
			string initial_row, initial_col, final_row, final_col;
			cin >> initial_col;
			if(initial_col == "undo"){
				if(b.move_count == 0){
					cout << "Error: cannot undo any further";
					continue;
				}
				else{
					b.flip_board();
					b.undo_move();
				}
			}
			else if(initial_col == "resign"){
				if(b.move_count%2 == 0){
					cout << "White has resigned, Black wins";
				}
				else{
					cout << "Black has resigned, White wins";
			
				}
				exit(0);
			}
			else{
				cout << "and row: ";
				cin >> initial_row;
				cout <<"to column: ";
				cin >> final_col;
				cout << "and row: ";
				cin >> final_row;
				
				b.move_piece(initial_row, initial_col,final_row, final_col);
			
				b.flip_board();
				b.move_count++;
		}

	}

	return 0;
}

