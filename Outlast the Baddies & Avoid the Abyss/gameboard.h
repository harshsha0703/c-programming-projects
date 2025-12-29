#ifndef _GAMEBOARD_H
#define _GAMEBOARD_H

#include <cstdlib>
#include <iostream>
#include <string>
#include <ctime>
#include <stdexcept>

#include "boardcell.h"
#include "grid.h"

using namespace std;

class GameBoard {
	private: 
	    Grid<BoardCell*> board;
        size_t numRows;
        size_t numCols;
        size_t HeroRow; // Hero's position row
	    size_t HeroCol; // Hero's position column
        int numMonsters;
        int numSuperMonsters;
        int numAbysses;
        int numBats;
        bool wonGame; // false, unless the Hero reached the exit successfully
        bool heroAlive;

		
	public: 
		/* default constructor */
        GameBoard() {
            numMonsters = 4;
            numSuperMonsters = 2;
            numAbysses = 50;
            numBats = 2;
            wonGame = false;
            
            this -> numRows = 15;
            this -> numCols = 40;
            
            Grid<BoardCell*> boardnew(numRows, numCols);
            board = boardnew;
            
            blankBoard();
        }
        
        /* param constructor */
        GameBoard(size_t numRows, size_t numCols) {
            numMonsters = 4;
            numSuperMonsters = 2;
            numAbysses = 20;
            numBats = 3;
            wonGame = false;
            
            this -> numRows = numRows;
            this -> numCols = numCols;
            
            Grid<BoardCell*> boardnew(numRows, numCols);
            board = boardnew;
            
            blankBoard();
        }
        
        /* destructor */
        virtual ~GameBoard() {
            for (size_t row = 0; row < board.numrows(); row++) {
                for (size_t col = 0; col < board.numcols(row); col++) {
                    delete board(row, col);
                }
            }
        }

        void blankBoard() {
            for (size_t row = 0; row < board.numrows(); row++) {
                for (size_t col = 0; col < board.numcols(row); col++) {
                    board(row, col) = new Nothing(row,col);
                }
            }
        }

        char getCellDisplay(size_t r, size_t c) {
            return board(r,c)->display();
        }

        void setCell(BoardCell* myCell, size_t r, size_t c) {
            board(r,c) = myCell;
        }
    
        void freeCell(size_t r, size_t c) {
            delete board(r,c);
        }

        // fills board with by randomly placing...
        //  - Hero (H) in the first three columns
        //  - EscapeLadder (*) in last three columns
        //  - 3 vertical Walls (+), each 1/2 of board height, in middle segment
        //  - Abyss cells (#), quantity set by numAbysses, in middle segment
        //  - Baddies [Monsters (m), Super Monsters (M), & Bats (~)] in middle segment;
        //    number of Baddies set by numMonsters, numSuperMonsters, & numBats
        void setupBoard(int seed) {
            srand(seed);
            size_t r,c;

            r = rand() % numRows;
            c = rand() % 3;
            delete board(r,c);
            board(r,c) = new Hero(r,c);
            HeroRow = r;
            HeroCol = c;

            r = rand() % numRows;
            c = numCols - 1 - (rand() % 3);
            delete board(r,c);
            board(r,c) = new EscapeLadder(r,c);
            
            int sizeMid = numCols - 6;

            c = 3 + (rand() % sizeMid);
            for (r = 0; r < numRows/2; ++r) {
                delete board(r,c);
                board(r,c) = new Wall(r,c);
            }
            size_t topc = c;

            while (c == topc || c == topc-1 || c == topc+1) {
                c = 3 + (rand() % sizeMid);
            }
            for (r = numRows-1; r > numRows/2; --r) {
                delete board(r,c);
                board(r,c) = new Wall(r,c);           
            }
            size_t botc = c;

            while (c == topc || c == topc-1 || c == topc+1 || c == botc || c == botc-1 || c == botc+1) {
                c = 3 + (rand() % sizeMid);
            }
            for (r = numRows/4; r < 3*numRows/4; ++r) {
                delete board(r,c);
                board(r,c) = new Wall(r,c);
            }

            for (int i = 0; i < numMonsters; ++i) {
                r = rand() % numRows;
                c = 3 + (rand() % sizeMid);
                while (board(r,c)->display() != ' ') {
                    r = rand() % numRows;
                    c = 3 + (rand() % sizeMid);
                }
                delete board(r,c);
                board(r,c) = new Monster(r,c);  
                board(r,c)->setPower(1);        
            }

            for (int i = 0; i < numSuperMonsters; ++i) {
                r = rand() % numRows;
                c = 3 + (rand() % sizeMid);
                while (board(r,c)->display() != ' ') {
                    r = rand() % numRows;
                    c = 3 + (rand() % sizeMid);
                }
                delete board(r,c);
                board(r,c) = new Monster(r,c); 
                board(r,c)->setPower(2);               
            }

            for (int i = 0; i < numBats; ++i) {
                r = rand() % numRows;
                c = 3 + (rand() % sizeMid);
                while (board(r,c)->display() != ' ') {
                    r = rand() % numRows;
                    c = 3 + (rand() % sizeMid);
                }
                delete board(r,c);
                board(r,c) = new Bat(r,c); 
            }

            for (int i = 0; i < numAbysses; ++i) {
                r = rand() % numRows;
                c = 3 + (rand() % sizeMid);
                while (board(r,c)->display() != ' ') {
                    r = rand() % numRows;
                    c = 3 + (rand() % sizeMid);
                }
                delete board(r,c);
                board(r,c) = new Abyss(r,c);              
            }
        }

        // neatly displaying the game board 
		void display( ) {
            cout << '-';
            for (size_t col = 0; col < board.numcols(0); col++) {
                cout << '-';
            }
            cout << '-';
            cout << endl;
            for (size_t row = 0; row < board.numrows(); row++) {
                cout << '|';
                for (size_t col = 0; col < board.numcols(row); col++) {
                    cout << board(row,col)->display();
                }
                cout << '|';
                cout << endl;
            }
            cout << '-';
            for (size_t col = 0; col < board.numcols(0); col++) {
                cout << '-';
            }
            cout << '-';
            cout << endl;
            
        }
		
        bool getWonGame() {
            return wonGame;
        }
        
        // distributing total number of monsters so that 
        //  ~1/3 of num are Super Monsters (M), and
        //  ~2/3 of num are Regular Monsters (m)
        void setNumMonsters(int num) {
            numSuperMonsters = num/3;
            numMonsters = num - numSuperMonsters;
        }

        void setNumAbysses(int num) {
            numAbysses = num;
        }

        void setNumBats(int num) {
            numBats = num;
        }

        size_t getNumRows() {
            return numRows;
        }

        size_t getNumCols() {
            return numCols;
        }

        
        //---------------------------------------------------------------------------------
        // void getHeroPosition(size_t& row, size_t& col)
        //
        // getter for Hero's position, which are private data members
        //      int HeroRow;
	    //      int HeroCol;
        // note: row and col are passed-by-reference
        //---------------------------------------------------------------------------------
        void getHeroPosition(size_t& row, size_t& col) {

            row = HeroRow; // It will return Hero's current row
            col = HeroCol; // It will return Hero's current column
        }

        
        //---------------------------------------------------------------------------------
        // void setHeroPosition(size_t row, size_t col)
        //
        // setter for Hero's position, which are private data members
        //      int HeroRow;
	    //      int HeroCol;
        //---------------------------------------------------------------------------------
        void setHeroPosition(size_t row, size_t col) {
            // It will update Hero's row and column position
            HeroRow = row; 
            HeroCol = col;
        
        }

        
        //---------------------------------------------------------------------------------
        // findHero()
        //
        // updater for Hero's position, which are private data members
        //      int HeroRow;
	    //      int HeroCol;
        // this function should find Hero in board and update
        //      HeroRow and HeroCol with the Hero's updated position;
        // if Hero cannot be found in board, then set Hero's position to (-1,-1)
        //---------------------------------------------------------------------------------
        void findHero() {
            heroAlive = false;
            
            // It will loop through all the rows of the board
            for (size_t row = 0; row < board.numrows(); ++row){
                for (size_t col = 0; col < board.numcols(row); ++col){

                     // It will check if the cell at (row, col) contains the hero
                    if (board(row, col)->isHero()){
                        setHeroPosition(row, col);
                        heroAlive = true;
                        return;
                    }
                }
            }
            // If the hero is not found after scanning the entire board
            setHeroPosition(-1, -1);
        
        }


        //---------------------------------------------------------------------------------
        // bool makeMoves(char HeroNextMove)
        // 
        // This is the primary gameplay operation for a single round of the game. 
        // A LOT happens in this function... 
        // General steps:
        //  - Allow user to input their next move 
        //  - Get the attempted move position for the Hero
        //  - Move the hero, catching/resolving any potential collision exceptions...
        //      - attempted move out-of-bounds: change row &/or col to stay on board
        //      - attempted move into a barrier: change row &/or col to stay off barrier
        //      - attempted move to the exit: remove hero from board, hero escaped!
        //      - attempted move into a hole: remove hero from board, hero did not escape
        //      - attempted move to a baddie: remove hero from board, hero did not escape
        //      - attempted move to an empty space: actual move is the attempted move
        //  - For each baddie (regular Monster, super Monster, or Bat) on the board...
        //      - check that this baddies hasn't already moved this round
        //      - get its attempted move position
        //      - move the baddie, catching/resolving any potential collision exceptions...
        //          - attempted move out-of-bounds: change row &/or col to stay on board
        //          - attempted move into a barrier: change row &/or col to stay off barrier
        //          - attempted move to the exit: change row &/or col to stay off exit
        //          - attempted move into a hole: remove baddie from board
        //          - attempted move to hero: remove hero from board, hero did not escape
        //          - attempted move to a baddie: ignore attempted move, no position change
        //          - attempted move to an empty space: actual move is the attempted move 
        // 
        // Note: all baddies (and the hero) fall into holes if their attempted 
        //       move ENDs on a hole (i.e. even Bats are drawn into the 
        //       Abyss if their attempted move takes them to an Abyss cell); 
        //       BUT, baddies can travel over holes, as long as their attempted 
        //       move does NOT END on a hole; this only applies, in practice, 
        //       to super monsters and bats, since their step sizes can be more 
        //       than 1 (monsters and the hero are limited to steps of size 1)
        //
        // Note: also, whereas baddies (and the hero) can never move onto a 
        //       barrier (i.e. a wall), they can step over barriers as long
        //       as the attempted move does NOT END on a barrier; this only 
        //       applies, in practice, to super monsters and bats, since their 
        //       step sizes can be more than 1 (monsters and the hero are limited 
        //       to steps of size 1)
        //
        // Note: to prevent a single baddie from making multiple moves
        //       during a single round of the game, whenever a baddie 
        //       has moved, it should be marked as "already moved" in 
        //       some way; this can be done using the [moved] data member
        //       of the BoardCell base class, which has setter/getter 
        //       functions provided. The [moved] data members must be 
        //       reset for all BoardCells at the beginning of each round.
        //
        // Note: the [myRow] and [myCol] data members for BoardCell derived
        //       class objects must be updated whenever a move is made; 
        //       additionally, [HeroRow] and [HeroCol] data members for the 
        //       GameBoard must be updated whenever the Hero has moved, 
        //       which can be done easily with a call to findHero()
        //
        // Note: many actual moves made by non-static board cell objects 
        //       (i.e. Heros, Monsters, Bats) involve constructing and/or 
        //       destructing objects; be careful with memory management; 
        //       specifically, make sure to free (delete) the memory for 
        //       BoardCell derived class objects when you are done with it
        //
        // return true if Hero is still on board at the end of the round
        // return false if Hero is NOT on board at the end of the round
        //---------------------------------------------------------------------------------
        bool makeMoves(char HeroNextMove) {
            // It will reset the 'moved' status for all cells on the board before making any moves
            for (size_t row = 0; row < numRows; ++row){
                for (size_t col = 0; col < numCols; ++col){
                    board(row, col)->setMoved(false);
                }
            }
            size_t newR = HeroRow, newC = HeroCol;
            board(HeroRow,HeroCol)->setNextMove(HeroNextMove);
            board(HeroRow,HeroCol)->attemptMoveTo(newR,newC,HeroRow,HeroCol);
            // Check if the new position is out-of-bounds
            try {
                if (newC >= numCols || newR >= numRows) { 
                    throw runtime_error("Hero trying to move out-of-bounds");
                } 
            }
            catch (runtime_error& excpt) {
                cout << excpt.what() << endl;
                // Adjust the hero's position to stay within bounds
                if (newR >= numRows){
                    newR = numRows - 1;
                }
                if (newC >= numCols) {
                    newC = numCols - 1;
                }    
                cout << "Hero position changed to stay in-bounds" << endl;
            }
            // Check if the hero is attempting to move into a barrier
            try {
                if (board(newR, newC)->isBarrier()) {
                    throw runtime_error("Hero attempting move into a wall");
                }
            } catch (runtime_error& excpt) {
                cout << excpt.what() << endl;
                if (newR != HeroRow && newC != HeroCol) {
                    
                    // Try moving in only one direction to avoid the barrier
                    size_t changedR = newR, changedC = HeroCol;
                    if (board(changedR, changedC)->isBarrier()) {
                        changedR = HeroRow;
                        changedC = newC;
                    }
                    if (board(changedR, changedC)->isBarrier()) {
                    newR = HeroRow;
                    newC = HeroCol;
                    } else {
                    newR = changedR;
                    newC = changedC;
                     }
                } else {
                newR = HeroRow;
                newC = HeroCol;
                }
            cout << "Hero position adjusted to avoid wall" << endl;
            }
            // Check if the hero has reached the exit
            if (board(newR, newC)->isExit()) {
                cout << "Hero has reached the EscapeLadder! Hero wins!" << endl;
                delete board(HeroRow, HeroCol);// Remove the hero from the current position
                board(HeroRow, HeroCol) = new Nothing(HeroRow, HeroCol);
                wonGame = true;
                return false;
            }

             // Check if the hero has fallen into a hole
            if (board(newR, newC)->isHole()) {
                cout << "Hero has fallen into the Abyss! Game over." << endl;
                delete board(HeroRow, HeroCol);
                board(HeroRow, HeroCol) = new Nothing(HeroRow, HeroCol);
                heroAlive = false;// Mark the hero as not alive
                return false;
            }

            // Check if the hero has encountered a baddie
            if (board(newR, newC)->isBaddie()) {
                cout << "Hero encountered a Baddie! Game over." << endl;
                delete board(HeroRow, HeroCol);
                board(HeroRow, HeroCol) = new Nothing(HeroRow, HeroCol);
                heroAlive = false;
                return false;
            }
             // If the new position is empty, move the hero there
            if (board(newR, newC)->isSpace()) {
                delete board(newR, newC);
                // Move the hero to the new position
                board(newR, newC) = board(HeroRow, HeroCol);
                board(newR, newC)->setPos(newR, newC);
                // Set the old position to empty
                board(HeroRow, HeroCol) = new Nothing(HeroRow, HeroCol);
            }
            board(newR, newC)->setMoved(true);
            HeroRow = newR;
            HeroCol = newC;
            
            // It will iterate over the board to move all baddies
            for (size_t row = 0; row < numRows; ++row) {
                for (size_t col = 0; col < numCols; ++col) {
                    BoardCell* cell = board(row, col);
                    // Check if the cell contains a baddie that hasn't moved yet
                    if (cell->isBaddie() && !cell->getMoved()) {
                        size_t newBaddieR, newBaddieC;
                        cell->setNextMove(' ');
                        cell->attemptMoveTo(newBaddieR, newBaddieC, HeroRow, HeroCol);
                        if (newBaddieR >= numRows) {
                            newBaddieR = row; 
                        }
                        if (newBaddieC >= numCols) {
                            newBaddieC = col;
                        }
                        // It will check if the new position is out-of-bounds
                        try {
                            if (newBaddieR >= numRows || newBaddieC >= numCols) {
                                throw runtime_error("Baddie trying to move out-of-bounds");
                            }
                        } catch (runtime_error& excpt) {
                            cout << excpt.what() << endl;
                            newBaddieR = row;
                            newBaddieC = col;
                            cout << "Baddie stays in place due to out-of-bounds move" << endl;
                            cell->setMoved(true);
                            continue;
                        }
                        // Check if the baddie is attempting to move into a barrier or exit
                        try {
                            if (board(newBaddieR, newBaddieC)->isBarrier() || board(newBaddieR, newBaddieC)->isExit()) {
                                throw runtime_error("Baddie attempting to move into a wall or exit");
                            }
                        } catch (runtime_error& excpt) {
                            cout << excpt.what() << endl;
                            newBaddieR = row;
                            newBaddieC = col;
                            cout << "Baddie stays in place to avoid wall or exit" << endl;
                            cell->setMoved(true);
                            continue;
                        }
                        // If the baddie falls into a hole
                        if (board(newBaddieR, newBaddieC)->isHole()) {
                            cout << "Baddie has fallen into the Abyss at (" << newBaddieR << "," << newBaddieC << ")!" << endl;
                            delete board(row, col);
                            board(row, col) = new Nothing(row, col);
                            continue;
                        }
                        // If the baddie encounters the hero
                        if (board(newBaddieR, newBaddieC)->isHero()) {
                            cout << "Baddie has found the Hero at (" << newBaddieR << "," << newBaddieC << ")! Game Over" << endl;
                            delete board(newBaddieR, newBaddieC);
                            board(newBaddieR, newBaddieC) = cell; 
                            board(newBaddieR, newBaddieC)->setPos(newBaddieR, newBaddieC);
                            board(row, col) = new Nothing(row, col); 
                            cell->setMoved(true);
                            heroAlive = false;
                            return false; 
                        }
                        // If the baddie encounters another baddie
                        if (board(newBaddieR, newBaddieC)->isBaddie()) {
                            cout << "Baddie collision at (" << newBaddieR << "," << newBaddieC << ")! Staying in place" << endl;
                            cell->setMoved(true);
                            continue;
                        }
                        // If the baddie moves into an empty space
                        if (board(newBaddieR, newBaddieC)->isSpace()) {
                            delete board(newBaddieR, newBaddieC);
                            board(newBaddieR, newBaddieC) = cell;
                            board(newBaddieR, newBaddieC)->setPos(newBaddieR, newBaddieC);
                            board(row, col) = new Nothing(row, col);
                            cell->setMoved(true);
                            continue;
                        }
                        // Mark the baddie as having moved
                        cell->setMoved(true);
                    }
                }
            }
            // It will update the hero's status on the board
            findHero();
            return heroAlive;
        }
};

#endif //_GAMEBOARD_H
