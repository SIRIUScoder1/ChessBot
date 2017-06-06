// makemove.c
#include "defi.h"
#include <stdio.h>
// hashing macro from the poskey for piece, castlePermission, side, enPasSquare
#define HASH_PCE(pce,sq) (pos->posKey ^= (PieceKeys[(pce)][(sq)]))
#define HASH_CA (pos->posKey ^= (CastleKeys[(pos->castlePermission)]))
#define HASH_SIDE (pos->posKey ^= (SideKey))
#define HASH_EP (pos->posKey ^= (PieceKeys[EMPTY][(pos->enPasSquare)]))

// each time we move a piece we basically be doing taking the castlePermission
// and doing a bitwise operation with this array and the square from

//  ca_perm &= castlePermission[from]

// so if a rook or a king moves it takes out the castle permission of the piece

const int CastlePerm[120] =
{
        15,15,15,15,15,15,15,15,15,15,
        15,15,15,15,15,15,15,15,15,15,
        15,13,15,15,15,12,15,15,14,15,
        15,15,15,15,15,15,15,15,15,15,
        15,15,15,15,15,15,15,15,15,15,
        15,15,15,15,15,15,15,15,15,15,
        15,15,15,15,15,15,15,15,15,15,
        15,15,15,15,15,15,15,15,15,15,
        15,15,15,15,15,15,15,15,15,15,
        15,7,15,15,15, 3,15,15,11,15,
        15,15,15,15,15,15,15,15,15,15,
        15,15,15,15,15,15,15,15,15,15,
};

static void ClearPiece(const int sq, S_BOARD *pos)
{
        ASSERT(SqOnBoard(sq));

        int pce = pos->pieces[sq]; //piece on the board

        ASSERT(PieceValid(pce)); // a valid piece

        int col = PieceCol[pce]; //color of piece
        int index = 0; // for looping
        int t_pceNum = -1;

        HASH_PCE(pce,sq); // ex-or the piece on the square so effectively hashing
                          // it out in posKey
        pos->pieces[sq] = EMPTY;             // making the piece at that position to empty
        pos->material[col] -= PieceVal[pce];             // also subtracting the piece value from the board

        if(PieceBig[pce])
        {
                pos->bigPieces[col]--;
                if(PieceMaj[pce])
                        pos->majPieces[col]--;
                else
                        pos->minPieces[col]--;
        }
        else
        {
                CLRBIT(pos->pawns[col],SQ64(sq));
                CLRBIT(pos->pawns[BOTH],SQ64(sq));
        }
        /*
           pos->pceNum[wP] == 3 looping through 0 to 3 (there are 3 white pawns present)
           pos->pList[wP][0] == sq0
           pos->pList[wP][1] == sq1
           pos->pList[wP][2] == sq2
           pos->pList[wP][3] == sq3
           pos->pList[wP][4] == sq4

           if sq == sq3 so t_pceNum = 3 because index is 3
         */
        for(index = 0; index < pos->piecesNum[pce]; index++)
        {
                if(pos->pList[pce][index] == sq)
                {
                        t_pceNum = index;
                        break;
                }
        }

        ASSERT(t_pceNum != -1);

        pos->piecesNum[pce]--;
        //  pos->pceNum[wP] = 4
        //pos->pList[wP][2] = po
        pos->pList[pce][t_pceNum] = pos->pList[pce][pos->piecesNum[pce]];

        /*
           pos->pceNum[wP] == 3 looping through 0 to 3 (there are 3 white pawns present)
           pos->pList[wP][0] == sq0
           pos->pList[wP][1] == sq1
           pos->pList[wP][2] == sq2
           pos->pList[wP][3] == sq4
         */
}

static void AddPiece(const int sq, S_BOARD *pos,const int pce)
{
        ASSERT(PieceValid(pce));
        ASSERT(SqOnBoard(sq));

        int col = PieceCol[pce]; // taking color of the piece to add

        HASH_PCE(pce,sq); // hash this piece in the poskey

        pos->pieces[sq] = pce;

        if(PieceBig[pce])
        {
                pos->bigPieces[col]++;
                if(PieceMaj[pce])
                        pos->majPieces[col]++;
                else
                        pos->minPieces[col]++;
        }
        else
        {
                SETBIT(pos->pawns[col],SQ64(sq));
                SETBIT(pos->pawns[BOTH],SQ64(sq));
        }

        pos->material[col] += PieceVal[pce];
        pos->pList[pce][pos->piecesNum[pce]++] = sq;
}

static void MovePiece(const int from, const int to, S_BOARD *pos)
{
        ASSERT(SqOnBoard(from));
        ASSERT(SqOnBoard(to));

        int index = 0;
        int pce = pos->pieces[from];
        int col = PieceCol[pce];
    #ifdef DEBUG
        int t_PieceNum = FALSE; // Assertion for the piece list
    #endif

        HASH_PCE(pce,from);
        pos->pieces[from] = EMPTY;
        HASH_PCE(pce,to);
        pos->pieces[to] = pce;

        if(!PieceBig[pce])
        {
                CLRBIT(pos->pawns[col],SQ64(from));
                CLRBIT(pos->pawns[BOTH],SQ64(from));
                SETBIT(pos->pawns[col],SQ64(to));
                SETBIT(pos->pawns[BOTH],SQ64(to));
        }

        for(index = 0; index < pos->piecesNum[pce]; index++)
        {
                if(pos->pList[pce][index] == from)
                {
                        pos->pList[pce][index] = to;
          #ifdef DEBUG
                        t_PieceNum = TRUE;
          #endif
                        break;
                }
        }
        ASSERT(t_PieceNum);
}

int MakeMove(S_BOARD *pos,int move)
{
        ASSERT(CheckBoard(pos));

        int from = FROM(move);
        int to = To(move);
        int side = pos->side;

        ASSERT(SqOnBoard(from));
        ASSERT(SqOnBoard(to));
        ASSERT(SideValid(side));
        ASSERT(PieceValid(pos->pieces[from]));

        pos->history[pos->histPlay].posKey = pos->posKey;

        // if the move is enpassent move then the enpassent capture is WHITE
        // then we have to remove the black pawn which will be at the (to square -10)
        if(move & MoveFlag)
        {
                if(side == WHITE)
                        ClearPiece(to-10,pos);
                else
                        ClearPiece(to+10,pos);
        }
        // is it the castle move
        else if(move & MoveFlagCastle)
        {
                switch(to)
                {
                case C1:
                        MovePiece(A1,D1,pos);
                        break;
                case C8:
                        MovePiece(A8,D8,pos);
                        break;
                case G1:
                        MovePiece(H1,F1,pos);
                        break;
                case G8:
                        MovePiece(H8,F8,pos);
                        break;
                default: ASSERT(FALSE); break;
                }
        }

        // if the current is set we will hash out this enpassent square
        if(pos->enPasSquare != NO_SQ) HASH_EP;
        HASH_CA; // hash out ca


        pos->history[pos->histPlay].move = move;
        pos->history[pos->histPlay].fiftyMoves = pos->fiftyMoves;
        pos->history[pos->histPlay].enPasSquare = pos->enPasSquare;
        pos->history[pos->histPlay].castlePermission = pos->castlePermission;

        pos->castlePermission &= CastlePerm[from];
        pos->castlePermission &= CastlePerm[to];
        pos->enPasSquare = NO_SQ;

        HASH_CA;

        int captered = CAPTURED(move);
        pos->fiftyMoves++;

        if(captered != EMPTY)
        {
                ASSERT(PieceValid(captered));
                ClearPiece(to,pos);
                pos->fiftyMoves = 0; // when capture is made we make a fifty move count to zero
        }
        pos->histPlay++;
        pos->play++;

        //new enPasSquare capture

        if(PiecePawn[pos->pieces[from]])
        {
                pos->fiftyMoves = 0; // capture move
                if(move & MoveFlagPawnStart)
                {
                        if(side == WHITE)
                        {
                                pos->enPasSquare = from + 10;
                                ASSERT(RankBrd[pos->enPasSquare] == RANK_3);
                        }
                        else
                        {
                                pos->enPasSquare = from - 10;
                                ASSERT(RankBrd[pos->enPasSquare] == RANK_6);
                        }
                        HASH_EP;
                }
        }

        MovePiece(from,to,pos);
        int prPce = PROMOTED(move);
        // promotion piece
        if(prPce != EMPTY)
        {
                ASSERT(PieceValid(prPce) && !PiecePawn[prPce]); // valid piece to promoted to
                ClearPiece(to,pos);
                AddPiece(to,pos,prPce);
        }

        if(PieceKing[pos->pieces[to]])
        {
                pos->KingSquare[pos->side] = to;
        }

        pos->side ^= 1;
        HASH_SIDE;

        ASSERT(CheckBoard(pos));

        if(SqAttacked(pos->KingSquare[side],pos->side,pos))
        {
                TakeMove(pos);
                return FALSE;
        }
        return TRUE;
}


void TakeMove(S_BOARD *pos)
{
        ASSERT(CheckBoard(pos));

        pos->histPlay--;
        pos->play--;

        int move = pos->history[pos->histPlay].move; //getting the move that was made
        int from = FROM(move);
        int to = To(move);

        //  printf("Takemove:%s\n",PrMove(move));

        ASSERT(SqOnBoard(from));
        ASSERT(SqOnBoard(to));

        if(pos->enPasSquare != NO_SQ) HASH_EP;
        HASH_CA;

        pos->castlePermission = pos->history[pos->histPlay].castlePermission;
        pos->fiftyMoves = pos->history[pos->histPlay].fiftyMoves;
        pos->enPasSquare = pos->history[pos->histPlay].enPasSquare;

        if(pos->enPasSquare != NO_SQ) HASH_EP;
        HASH_CA;
        pos->side ^= 1;
        HASH_SIDE;

        if(MoveFlag & move) // enpassent capture
        {
                if(pos->side == WHITE)
                        AddPiece(to-10,pos,bP);
                else
                        AddPiece(to+10,pos,wP);
        }
        else if(MoveFlagCastle & move)
        {
                switch (to)
                {
                case C1: MovePiece(D1,A1,pos);  break;
                case C8: MovePiece(D8, A8, pos);  break;
                case G1: MovePiece(F1, H1, pos);  break;
                case G8: MovePiece(F8, H8, pos);  break;
                default: ASSERT(FALSE); break;
                }
        }

        MovePiece(to,from,pos);

        if(PieceKing[pos->pieces[from]])
        {
                pos->KingSquare[pos->side] = from;
        }
        int captured = CAPTURED(move);
        if(captured != EMPTY)
        {
                ASSERT(PieceValid(captured));
                AddPiece(to,pos,captured);
        }
        if(PROMOTED(move) != EMPTY)
        {
                ASSERT(PieceValid(PROMOTED(move)) && !PiecePawn[PROMOTED(move)]);
                ClearPiece(from,pos);
                AddPiece(from,pos,(PieceCol[PROMOTED(move)] == WHITE ? wP : bP));
        }
        ASSERT(CheckBoard(pos));
}
