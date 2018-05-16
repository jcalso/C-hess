#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <unordered_map>
#include <utility>
#include <functional>
using namespace std;

//TO DO 
//move checking
//castling
//Check/checkmate
//pawn promotion
//en passant
//50 move draw
//3 fold repetition draw

class ChessBoard{
	public:
	
	vector<vector<string>> Board;
	
	unordered_map<string, int>coords;

	
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
				if(Board[i][j-1]== "|" && j != 17){
					Board[i][j] = "   ";
				}
				else if(i%2 == 0 && j%2 == 1 && j!= 17){ //Set horizontal border
					Board[i][j] = "---";
				}
				else if(i%2 == 1 && j%2 == 0 && i != 17){
					Board[i][j] = "|";
				}
				
				
				else if(j == 17 && i%2 == 1 && i != 17){ //set number coordinates
					Board[i][j] = space+to_string(9-(i+1)/2);
				}
			  else if(i == 17 && j%2 == 1 && j != 17){
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
	
	
	
	
	void move_piece(string initial_row_in, string initial_col_in, string final_row_in, string final_col_in, int move_count){
		int row_i, col_i, row_f, col_f;
		if(move_count %2 == 0){
			row_i = (8- stoi(initial_row_in))*2 +1;
			
			row_f= (8-stoi(final_row_in))*2 +1;
		}
		else{
			row_i = (stoi(initial_row_in))*2;
			
			row_f= (stoi(final_row_in))*2;
		}
			col_f = coords[final_col_in];
			col_i = coords[initial_col_in];
		
		cout << row_f << col_f;
	
		Board[row_f][col_f] = Board[row_i][col_i];
		Board[row_i][col_i] = "   ";
		
	}
	
	bool is_pawn(string p){
		return (p == "P" || p == "p");
	}
	bool is_knight(string p){
		return (p == "N" || p == "n");
	}
	bool is_bishop(string p){
		return (p == "b" || p == "B");
	}
	bool is_queen(string p){
		return (p == "q" || p == "Q");
	}
	bool is_king(string p){
		return (p == "k" || p == "K");
	}
	bool is_rook(string p){
		return (p == "r" || p == "R");
	}
	
	
};

void print_start_message(){
	
	cout << "Welcome to C++hess, type 'start' to begin a game, type 'help' for instructions, type 'quit' to quit."<< endl;
	
	string command;

	
	while (command != "start"){
		cout << "->";
		cin >> command;
		if (command == "help"){
			cout << "Enter two sets of coordinates to move a piece, enter 'resign' in the first prompt to resign, otherwise you must make a move" << endl;
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
	int move_count = 0;
	print_start_message();
	b.init();
	b.insert_pieces(); 
	
	
	while(!checkmate){
		b.print();
	
		if(move_count %2 == 0){
			cout << "White to move" << endl;
		}
		else {
			cout << "Black to move" << endl;
			
		}
		cout << "Move piece at column: ";
			string initial_row, initial_col, final_row, final_col;
			cin >> initial_col;
			cout << "and row: ";
			cin >> initial_row;
			cout <<"to column: ";
			cin >> final_col;
			cout << "and row: ";
			cin >> final_row;
			
			
		b.move_piece(initial_row, initial_col,final_row, final_col, move_count);
		
			b.flip_board();
			move_count++;
	}

	
	

	return 0;
}

