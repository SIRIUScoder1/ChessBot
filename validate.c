// validate.c

#include "defs.h"

// ASSERT(SqOnBoard(Sq))

int SqOnBoard(const int sq)
{
        return FilesBrd[sq]==OFFBOARD ? 0 : 1;
}

int SideValid(const int side)
{
        return (side==WHITE || side == BLACK) ? 1 : 0;
}

int FileRankValid(const int fr)
{
        return (fr >= 0 && fr <= 7) ? 1 : 0;
}

int PieceValidEmpty(const int pce) // piece valid with empty
{
        return (pce >= EMPTY && pce <= bK) ? 1 : 0;
}

int PieceValid(const int pce)  //piece valid without empty
{
        return (pce >= wP && pce <= bK) ? 1 : 0;
}
