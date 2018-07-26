#include "stdafx.h"
#include "CppUnitTest.h"
#include "../C-hess/Board.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1{		
	TEST_CLASS(UnitTest1){
	public:
		
		TEST_METHOD(Open_Game){
			ChessBoard b;
			b.init();
			b.insert_pieces();

			string blank = "   ";

			Move move = b.create_move("e2", "e4");

			Assert::AreEqual(move.prev_row, 13);
			Assert::AreEqual(move.prev_col , 9);
			Assert::AreEqual(move.curr_row , 9);
			Assert::AreEqual(move.curr_col , 9);
			Assert::AreEqual(move.prev_char, blank);

			b.move_piece("e2", "e4");

			move = b.create_move("e7", "e5");

			Assert::AreEqual(move.prev_row , 13);
			Assert::AreEqual(move.prev_col , 7);
			Assert::AreEqual(move.curr_row , 9);
			Assert::AreEqual(move.curr_col , 7);
			Assert::AreEqual(move.prev_char , blank);

			b.move_piece("e7", "e5");
			
			Assert::AreEqual(b.draw_move_count, 2);

		}
		TEST_METHOD(Zukertort_Opening_Black_Mustang_Defense) {
			ChessBoard b;
			b.init();
			b.insert_pieces();
			string blank = "   ";

			Move move = b.create_move("g1", "f3");

			Assert::AreEqual(move.prev_row, 15);
			Assert::AreEqual(move.prev_col, 13);
			Assert::AreEqual(move.curr_row, 11);
			Assert::AreEqual(move.curr_col, 11);
			Assert::AreEqual(move.prev_char, blank);

			b.move_piece("g1", "f3");

			move = b.create_move("b8", "c6");

			Assert::AreEqual(move.prev_row, 15);
			Assert::AreEqual(move.prev_col, 13);
			Assert::AreEqual(move.curr_row, 11);
			Assert::AreEqual(move.curr_col, 11);
			Assert::AreEqual(move.prev_char, blank);

			b.move_piece("b8", "c6");

		}
	};
}
