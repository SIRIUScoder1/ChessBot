// attack.c

#include "stdio.h"
#include "defs.h"

//  knight direction, Rook direction, bishop direction, king direction
// Rook horizontally -1 to -10 or vertically 1 to 10

const int KnDir[8] = { -8, -19, -21, -12, 8, 19, 21, 12 };
const int RkDir[4] = { -1, -10, 1, 10 };
const int BiDir[4] = { -9, -11, 11, 9 };
const int KiDir[8] = { -1, -10, 1, 10, -9, -11, 11, 9 };

// the square which is checked for attack and the side who is attacking (attacking side)

int SqAttacked(const int sq, const int side, const S_BOARD *pos) {

        int pce,index,t_sq,dir;
        // t_sq temporary square

        ASSERT(SqOnBoard(sq));          // is square on the board
        ASSERT(SideValid(side));                // is side is valid black or white
        ASSERT(CheckBoard(pos));                // is check the whole board configuration

        //pawns the attacking side is white and the -11 and --9 for the direction of attack
        //pawns attack defined
        if(side == WHITE) {
                if(pos->pieces[sq-11] == wP || pos->pieces[sq-9] == wP) {
                        return TRUE;
                }
        } else {
                if(pos->pieces[sq+11] == bP || pos->pieces[sq+9] == bP) {
                        return TRUE;
                }
        }

        // knights attack
        for(index = 0; index < 8; ++index) {
                pce = pos->pieces[sq + KnDir[index]];
                if(pce != OFFBOARD && IsKn(pce) && PieceCol[pce]==side) {
                        return TRUE;
                }
        }

        // rooks, queens
        //rooks and queens attack
        //sliding pieces so iterate this till we hit a piece or we go offboard and finally
        // move the piece accordingly
        for(index = 0; index < 4; ++index) {
                dir = RkDir[index];                 //current direction
                t_sq = sq + dir;
                pce = pos->pieces[t_sq];                 //which is there
                // we traverse till we hit a piece or we go off
                while(pce != OFFBOARD)                 //while the piece is not offboard
                {
                        if(pce != EMPTY) {
                                if(IsRQ(pce) && PieceCol[pce] == side) {
                                        return TRUE;                                         // if found rook or queen return true
                                }
                                break;                                  // else found any other piece we break out of while loop
                        }
                        t_sq += dir;                         // if empty then we increament to another square
                        pce = pos->pieces[t_sq];                         // and what is present there
                }
        }

        // bishops, queens attack
        // same approach the way above approach is described for the sliding pieces
        for(index = 0; index < 4; ++index) {
                dir = BiDir[index];
                t_sq = sq + dir;
                pce = pos->pieces[t_sq];
                while(pce != OFFBOARD) {
                        if(pce != EMPTY) {
                                if(IsBQ(pce) && PieceCol[pce] == side) {
                                        return TRUE;
                                }
                                break;
                        }
                        t_sq += dir;
                        pce = pos->pieces[t_sq];
                }
        }

        //kings attack is same as knight attack

        for(index = 0; index < 8; ++index) {
                pce = pos->pieces[sq + KiDir[index]];
                if(pce != OFFBOARD && IsKi(pce) && PieceCol[pce]==side) {
                        return TRUE;
                }
        }

        return FALSE;

}
