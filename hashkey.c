//hashkey.c
#include "stdlib.h"
#include "defi.h"
#include "stdio.h"
U64 GeneratePosKey(const S_BOARD *pos)
{
        int sq = 0;
        U64 finalKey = 0;
        int piece  = EMPTY;
        //pieces
        //loop all the squares on the loop and set piece equal to value stored
        // at that squares
        for(sq = 0; sq < BOARD_SQUARES; ++sq)
        {
                piece = pos->pieces[sq];
                if(piece != NO_SQ && piece != EMPTY && piece != OFFBOARD)
                {
                        ASSERT(piece >= wP && piece<= bK);
                        finalKey ^= PieceKeys[piece][sq];
                }
        }
        if(pos->side == WHITE)
        {
                finalKey ^= SideKey;
        }
        if(pos->enPasSquare != NO_SQ)
        {
                ASSERT(pos->enPasSquare >= 0 && pos->enPasSquare < BOARD_SQUARES);
                finalKey ^= PieceKeys[EMPTY][pos->enPasSquare];
        }

        ASSERT(pos->castlePermission >= 0 && pos->castlePermission <= 15)
        finalKey ^= CastleKeys[pos->castlePermission];
        return finalKey;
}
