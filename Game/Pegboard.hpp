//
// Created by phoenixflower on 6/18/20.
//

#ifndef LANDESSDEVCORE_PEGBOARD_HPP
#define LANDESSDEVCORE_PEGBOARD_HPP

//
//  Pegboard.h
//  Foundation
//
//  Created by James Landess on 12/27/13.
//  Copyright (c) 2013 LandessDev. All rights reserved.
//



#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <vector>
#include <list>
#include <bitset>
#include <memory>
namespace AI {
    namespace Games
    {
        // Copyright Daniel M. O'Brien (c) 2002
        // Copyright Daniel M. O'Brien (c) 2004
        //
        // Can be used for any legal purpose as long as original copyright above
        // is shown.
        //
        // Update for newer compiler includes
        //
        //
        // plays the triangular pegboard puzzle found at cracker barrel
        //
        // board usually starts out as:
        //
        //     0
        //    1 1
        //   1 1 1
        //  1 1 1 1
        // 1 1 1 1 1
        //
        // but can start with any single pin removed
        //
        // game ends with a single pin in the pegboard (i.e., a 1)
        // successive moves are showns as complete boards
        // a jump is like in checkers, pin jumps inline neighbor into empty hole
        // and "captures" the jumped over pin (neighbor)
        //
        // internal struct is built out of vector (rows) of bitset (columns)
        // indexing is as follows:
        //
        //             0,0
        //          1,0   1,1
        //       2,0   2,1   2,2
        //    3,0   3,1   3,2   3,3
        // 4,0   4,1   4,2   4,3   4,4
        //
        // objects are:
        //
        // PegBoard (the board itself with pins indicating position occupied)
        // Position (a pair of row/col position on the board, or a relative position)
        // Move (a pair of Positions, one absolute position, and other a direction
        //       to move toward)
        // GameTree (a tree starting from initial PegBoard that will contain all the
        //           winning successive board positions as children, representing
        //           the next logical board position from give one)
        //
        // Optionally a PegBoard can be displayed as a decimal number (boardCode)
        // by adding the bits making up that board.
        //
        // 2^14 = 0,0; 2^13 = 1,0; 2^12 = 1,1; ... 2^0 = 4,4; then summed
        //
        // thus a full board with pin 0,0 out is code: 16383
        // this is useful for analysing game board flows for patterns.
        // For example, staring with pin 0,0 out, there are 16128 solutions where
        // the last pin standing is 4,2.
        //



        //using namespace std;

        static const bool showBoardCode = false;

        static const int BOARDSIZE = 5;

        struct Position
        {
            Position() : row_(0), col_(0) {}
            Position(int row, int col) : row_(row), col_(col) {}

            bool move(const Position& incr);
            Position& operator+=(const Position& incr);
            Position& operator*=(int incr);
            static bool valid(const Position& pos);

            friend std::ostream& operator<<(std::ostream& os, const Position& pos);

            int row_;
            int col_;
        };


        ostream&
        operator<<(ostream& os, const Position& pos)
        {
            os << pos.row_ << ',' << pos.col_;
            return os;
        }


        bool
        Position::valid(const Position& pos)
        {
            if (pos.row_ >= BOARDSIZE)  return false;
            if (pos.row_ <  0)          return false;
            if (pos.col_ <  0)          return false;
            if (pos.col_ >= BOARDSIZE)  return false;
            if (pos.col_ > pos.row_)    return false;

            return true;
        }


        bool
        Position::move(const Position& incr)
        {
            Position current(*this);

            current += incr;
            if (!valid(current))
                return false;

            *this = current;

            return true;
        }


        Position&
        Position::operator+=(const Position& incr)
        {
            row_ += incr.row_;
            col_ += incr.col_;

            return *this;
        }


        Position&
        Position::operator*=(int incr)
        {
            row_ *= incr;
            col_ *= incr;

            return *this;
        }


        // valid movement graph
        static const Position MOVEMENT[] = {
                Position(-1,-1), Position(-1,+0),
                Position(+1,+0), Position(+1,+1),
                Position(+0,-1), Position(+0,+1)
        };
        static const int NUMMOVEMENTS = sizeof(MOVEMENT)/sizeof(Position);


        struct Move
        {
            Move(const Position& peg, const Position& incr) : peg_(peg), incr_(incr) {}

            friend ostream& operator<<(ostream& os, const Move& m);

            Position peg_;
            Position incr_;
        };


        ostream&
        operator<<(ostream& os, const Move& m)
        {
            os << "(" << m.peg_ << ":" << m.incr_ << ")";
            return os;
        }


        class GameTree;

        class PegBoard
        {
        public:
            PegBoard(int startPinRow = 0, int startPinCol = 0);

            void possibleMoves(vector<Move>& moves) const;
            //  bool play(const Move& currentMove, vector<Move>& goodMoves) const;
            void play(const Move& currentMove, GameTree& parent) const;
            void jump(const Move& move);

            friend ostream& operator<<(ostream& os, const PegBoard& p);

        private:
            void flip(const Position& peg) { board_[peg.row_][peg.col_].flip(); }
            bool occupied(const Position& pos) const
            { return board_[pos.row_][pos.col_]; }
            int  numPegs() const;

            vector< bitset<BOARDSIZE> > board_;
        };


        class GameTree
        {
        public:
            GameTree(const PegBoard& p) : currentBoard_(p) {}

            void append(GameTree* g) { children_.push_back(g); }
            int numChildren() const { return children_.size(); }
            bool walk() { return GameTree::walk(*this); }

        private:
            static bool walk(GameTree& parent);
            PegBoard currentBoard_;
            list<GameTree*> children_;
        };


        // to iterate is human, to recurse is divine
        bool
        GameTree::walk(GameTree& parent)
        {
            cout << parent.currentBoard_ << endl;

            list<GameTree*>::iterator firstChild = parent.children_.begin();
            if (firstChild == parent.children_.end())
            {
                cout << "========================" << endl;
                return true;
            }

            if (GameTree::walk(*(*firstChild)))
            {
                delete *firstChild;
                parent.children_.erase(firstChild);
            }

            return (parent.children_.begin() == parent.children_.end());
        }


        PegBoard::PegBoard(int startPinRow, int startPinCol)
        {
            for (int i = 0; i < BOARDSIZE; ++i)
            {
                bitset<BOARDSIZE> b;
                for (int j = 0; j <= i; ++j)
                {
                    b[j] = true;
                }
                board_.push_back(b);
            }
            board_[startPinRow][startPinCol] = false;
        }


        void
        PegBoard::possibleMoves(vector<Move>& moves) const
        {
            moves.clear();

            for (int i = 0; i < BOARDSIZE; ++i)
            {
                for (int j = 0; j <= i; ++j)
                {
                    Position hole(i,j);

                    if (occupied(hole))
                        continue;

                    for (int m = 0; m < NUMMOVEMENTS; ++m)
                    {
                        Position move(hole);
                        if (move.move(MOVEMENT[m]) && occupied(move) &&
                            move.move(MOVEMENT[m]) && occupied(move))
                        {
                            Position direction(MOVEMENT[m]);
                            direction *= -1;
                            moves.push_back(Move(move, direction));
                        }
                    }
                }
            }
        }


        int
        PegBoard::numPegs() const
        {
            int numPegs = 0;

            for (int i = 0; i < BOARDSIZE; ++i)
            {
                for (int j = 0; j <= i; ++j)
                {
                    if (occupied(Position(i,j)))
                        ++numPegs;
                }
            }

            return numPegs;
        }


        void
        PegBoard::jump(const Move& move)
        {
            Position peg(move.peg_);

            flip(peg);
            peg += move.incr_;
            flip(peg);
            peg += move.incr_;
            flip(peg);
        }


#if 0
        // to iterate is human, to recurse is divine
        bool
        PegBoard::play(const Move& currentMove, vector<Move>& goodMoves) const
        {
            PegBoard nextBoard(*this);
            nextBoard.jump(currentMove);

            if (nextBoard.numPegs() == 1)
            {
                goodMoves.push_back(currentMove);
                return true;
            }

            vector<Move> moves;
            nextBoard.possibleMoves(moves);

            for (int i = 0; i < moves.size(); ++i)
            {
                if (nextBoard.play(moves[i], goodMoves))
                {
                    goodMoves.push_back(currentMove);
                    return true;
                }
            }

            return false;
        }
#endif


        // to iterate is human, to recurse is divine
        void
        PegBoard::play(const Move& currentMove, GameTree& parent) const
        {
            PegBoard nextBoard(*this);
            nextBoard.jump(currentMove);

            auto_ptr<GameTree> node(new GameTree(nextBoard));

            if (nextBoard.numPegs() == 1)
            {
                parent.append(node.release());
                return;
            }

            vector<Move> moves;
            nextBoard.possibleMoves(moves);

            for (int i = 0; i < moves.size(); ++i)
            {
                nextBoard.play(moves[i], *node);
            }

            if (node->numChildren() > 0)
            {
                parent.append(node.release());
            }
        }


        ostream&
        operator<<(ostream& os, const PegBoard& p)
        {
            int boardCode = 0;
            for (int i = 0; i < BOARDSIZE; ++i)
            {
                os << setfill(' ') << setw(BOARDSIZE-i) << " ";
                for (int j = 0; j <= i; ++j)
                {
                    os << p.board_[i][j] << ' ';

                    boardCode <<= 1;
                    if (p.board_[i][j]) boardCode |= 1;
                }
                os << endl;
            }
            if (showBoardCode)
                os << boardCode << endl;

            return os;
        }

        /**
        int
        main(int argc, char* argv[])
        {
            int startPinRow = 0;
            int startPinCol = 0;

            if (argc == 3)
            {
                startPinRow = atoi(argv[1]);
                startPinCol = atoi(argv[2]);
            }

            PegBoard board(startPinRow, startPinCol);

            vector<Move> moves;
            board.possibleMoves(moves);

            GameTree root(board);
            for (int i = 0; i < moves.size(); ++i)
            {
                board.play(moves[i], root);
            }

            while (!root.walk()) ;
        }
         */
    }
}

/**
 *
 */
#endif //LANDESSDEVCORE_PEGBOARD_HPP
