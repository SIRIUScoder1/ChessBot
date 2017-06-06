//movegen.c

#include "stdio.h"
#include "defi.h"

/*
    movegen(board,list)
        loop all pieces
            -> slider loop each direction
                -> AddMove list->moves[list->count] = move;
                            list->count++;
 */

// array of structure of of the move
#define MOVE(f,t,ca,pro,f1)    ((f) | ((t) << 7) | ((ca) << 14) | ((pro) << 20) | (f1))
#define SQOFFBOARD(sq)  (FileBrd[(sq)] == OFFBOARD)

const int LoopSlidePce[8] = {wB, wR, wQ, 0, bB, bR, bQ,0};
const int LoopNonSlidePce[6] = {wKn,wK,0,bKn,bK,0};

const int LoopSlideIndex[2] = {0,4};
const int LoopNonSlideIndex[2] = {0,3};
// LoopSlideIndex[BLACK] = 4;
// LoopSlidePce[LoopSlideIndex[BLACK]] start
// LoopSlideIndex[WHITE] = 0;

const int pceDir[13][8] = {
        {0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0},
        {-8,-19,-21,-12,8,19,21,12},    //knight
        {-9,-11,11,9,0,0,0,0},  //bishop
        {-1,-10,1,10,0,0,0,0},  //rook
        {-1,-10,1,10,-9,-11,11,9},
        {-1,-10,1,10,-9,-11,11,9},
        {0,0,0,0,0,0,0},
        {-8,-19,-21,-12,8,19,21,12},
        {-9,-11,11,9,0,0,0,0},
        {-1,-10,1,10,0,0,0,0},
        {-1,-10,1,10,-9,-11,11,9},
        {-1,-10,1,10,-9,-11,11,9}
};
//moves for each piece
const int NumDir[13] = {
        0,0,8,4,4,8,8,0,8,4,4,8,8
};

// checking the legal move or not

int MoveExists(S_BOARD *pos,const int move)
{
        S_MOVELIST list[1];
        GenerateAllMoves(pos,list);

        int MoveNum = 0;
        for(MoveNum = 0; MoveNum < list->count; MoveNum++)
        {
                if(!MakeMove(pos,list->moves[MoveNum].move))
                {
                        continue;
                }
                TakeMove(pos);
                if(list->moves[MoveNum].move == move)
                        return TRUE;
        }
        return FALSE;
}

static void AddQuietMove(const S_BOARD *pos, int move, S_MOVELIST *list)
{
        list->moves[list->count].move = move;
        list->moves[list->count].score = 0;
        list->count++;
}


static void AddCaptureMove(const S_BOARD *pos, int move, S_MOVELIST *list)
{
        list->moves[list->count].move = move;
        list->moves[list->count].score = 0;
        list->count++;
}



static void AddEnPassantMove(const S_BOARD *pos, int move, S_MOVELIST *list)
{
        list->moves[list->count].move = move;
        list->moves[list->count].score = 0;
        list->count++;
}


static void AddWhitePawnCapMove(const S_BOARD *pos, const int from, const int to, const int cap, S_MOVELIST * list)
{
        ASSERT(PieceValidEmpty(cap));
        ASSERT(SqOnBoard(from));
        ASSERT(SqOnBoard(to));

        if(RankBrd[from] == RANK_7)
        {
                AddCaptureMove(pos, MOVE(from,to,cap,wQ,0),list);
                AddCaptureMove(pos, MOVE(from,to,cap,wR,0),list);
                AddCaptureMove(pos, MOVE(from,to,cap,wB,0),list);
                AddCaptureMove(pos, MOVE(from,to,cap,wKn,0),list);
        }
        else
        {
                AddCaptureMove(pos, MOVE(from,to,cap,EMPTY,0),list);
        }
}

static void AddWhitePawnMove(const S_BOARD *pos, const int from, const int to, S_MOVELIST * list)
{
        ASSERT(SqOnBoard(from));
        ASSERT(SqOnBoard(to));
        if(RankBrd[from] == RANK_7)
        {
                AddQuietMove(pos,MOVE(from,to,EMPTY,wQ,0),list);
                AddQuietMove(pos,MOVE(from,to,EMPTY,wR,0),list);
                AddQuietMove(pos,MOVE(from,to,EMPTY,wB,0),list);
                AddQuietMove(pos,MOVE(from,to,EMPTY,wKn,0),list);
        }
        else
        {
                AddQuietMove(pos,MOVE(from,to,EMPTY,EMPTY,0),list);
        }
}

static void AddBlackPawnCapMove(const S_BOARD *pos, const int from, const int to, const int cap, S_MOVELIST * list)
{
        ASSERT(PieceValidEmpty(cap));
        ASSERT(SqOnBoard(from));
        ASSERT(SqOnBoard(to));
        if(RankBrd[from] == RANK_2)
        {
                AddCaptureMove(pos, MOVE(from,to,cap,bQ,0),list);
                AddCaptureMove(pos, MOVE(from,to,cap,bR,0),list);
                AddCaptureMove(pos, MOVE(from,to,cap,bB,0),list);
                AddCaptureMove(pos, MOVE(from,to,cap,bKn,0),list);
        }
        else
        {
                AddCaptureMove(pos, MOVE(from,to,cap,EMPTY,0),list);
        }
}

static void AddBlackPawnMove(const S_BOARD *pos, const int from, const int to, S_MOVELIST * list)
{
        ASSERT(SqOnBoard(from));
        ASSERT(SqOnBoard(to));

        if(RankBrd[from] == RANK_2)
        {
                AddQuietMove(pos,MOVE(from,to,EMPTY,bQ,0),list);
                AddQuietMove(pos,MOVE(from,to,EMPTY,bR,0),list);
                AddQuietMove(pos,MOVE(from,to,EMPTY,bB,0),list);
                AddQuietMove(pos,MOVE(from,to,EMPTY,bKn,0),list);
        }
        else
        {
                AddQuietMove(pos,MOVE(from,to,EMPTY,EMPTY,0),list);
        }
}
// moving pawn forward +10 in columns
//capture diagonally which is +9 or +11
//if the jump two squares in first move then +20 if on 2nd rank
// if they are on 7th rank and move to the 8th they promote so they are 4 possible
// promotions queen, knight, rook, bishop
void GenerateAllMoves(const S_BOARD *pos, S_MOVELIST *list)
{
        ASSERT(CheckBoard(pos));

        list->count = 0;

        int pce = EMPTY;
        int side = pos->side;
        int sq = 0;
        int t_sq = 0;
        int pceNum = 0;
        int dir = 0;    //direction
        int index = 0;
        int pceIndex = 0;

        // printf("\n\nSide:%d\n",side);

        if(side == WHITE)
        {
                // loop through each white pawn and then taking the square at which it is lying on..
                for(pceNum = 0; pceNum < pos->piecesNum[wP]; pceNum++)
                {
                        sq = pos->pList[wP][pceNum];
                        ASSERT(SqOnBoard(sq));
                        //simple move
                        if(pos->pieces[sq+10] == EMPTY)
                        {
                                AddWhitePawnMove(pos,sq,sq+10,list);
                                if(RankBrd[sq] == RANK_2 && pos->pieces[sq+20] == EMPTY)
                                {
                                        AddQuietMove(pos,MOVE(sq,(sq+20),EMPTY,EMPTY,MoveFlagPawnStart),list);
                                }
                        }
                        //capture (square not OFFBOARD and opposite colour piece is present)
                        if(!SQOFFBOARD(sq + 9) && PieceCol[pos->pieces[sq+9]] == BLACK)
                        {
                                AddWhitePawnCapMove(pos,sq,sq+9,pos->pieces[sq+9],list);
                        }
                        if(!SQOFFBOARD(sq + 11) && PieceCol[pos->pieces[sq+11]] == BLACK)
                        {
                                AddWhitePawnCapMove(pos,sq,sq+11,pos->pieces[sq+11],list);
                        }

                        //enpassent square capture
                        if(sq+9 == pos->enPasSquare)
                        {
                                AddCaptureMove(pos,MOVE(sq,sq+9,EMPTY,EMPTY,MoveFlag),list);
                        }
                        if(sq+11 == pos->enPasSquare)
                        {
                                AddCaptureMove(pos,MOVE(sq,sq+11,EMPTY,EMPTY,MoveFlag),list);
                        }
                }

                //CASTLING
                if(pos->castlePermission & WKCA)    // king side castling bit is enabled
                {
                        if(pos->pieces[F1] == EMPTY && pos->pieces[G1] == EMPTY)
                        {
                                if(!SqAttacked(E1,BLACK,pos) && (!SqAttacked(F1,BLACK,pos)))
                                {
                                        // printf("WKCA Movegen\n");
                                        AddQuietMove(pos,MOVE(E1,G1,EMPTY,EMPTY,MoveFlagCastle),list);
                                }
                        }
                }
                if(pos->castlePermission & WQCA)
                {
                        if(pos->pieces[D1] == EMPTY && pos->pieces[C1] == EMPTY && pos->pieces[B1] == EMPTY)
                        {
                                if(!SqAttacked(E1,BLACK,pos) && !SqAttacked(D1,BLACK,pos))
                                {
                                        AddQuietMove(pos,MOVE(E1,C1,EMPTY,EMPTY,MoveFlagCastle),list);
                                }
                        }
                }
        }
        else
        {
                // for black pawns moves
                for(pceNum = 0; pceNum < pos->piecesNum[bP]; pceNum++)
                {
                        sq = pos->pList[bP][pceNum];
                        ASSERT(SqOnBoard(sq));
                        //black is going -10 each time on moving
                        if(pos->pieces[sq-10] == EMPTY)
                        {
                                AddBlackPawnMove(pos,sq,sq-10,list);
                                if(RankBrd[sq] == RANK_7 && pos->pieces[sq - 20] == EMPTY)
                                {
                                        AddQuietMove(pos,MOVE(sq,(sq-20),EMPTY,EMPTY,MoveFlagPawnStart),list);
                                }

                                if(!SQOFFBOARD(sq-9) && PieceCol[pos->pieces[sq-9]] == WHITE)
                                {
                                        AddBlackPawnCapMove(pos,sq,sq-9,pos->pieces[sq-9],list);
                                }

                                if(!SQOFFBOARD(sq-11) && PieceCol[pos->pieces[sq-11]] == WHITE)
                                {
                                        AddBlackPawnCapMove(pos,sq,sq-11,pos->pieces[sq-11],list);
                                }

                                if(sq-9 == pos->enPasSquare)
                                {
                                        AddCaptureMove(pos,MOVE(sq,sq-9,EMPTY,EMPTY,MoveFlag),list);
                                }
                                if(sq-11 == pos->enPasSquare)
                                {
                                        AddCaptureMove(pos,MOVE(sq,sq-11,EMPTY,EMPTY,MoveFlag),list);
                                }
                        }
                }

                //CASTLING
                if(pos->castlePermission & BKCA)    // king side castling bit is enabled
                {
                        if(pos->pieces[F8] == EMPTY && pos->pieces[G8] == EMPTY)
                        {
                                if(!SqAttacked(E8,WHITE,pos) && (!SqAttacked(F8,WHITE,pos)))
                                {
                                        // printf("BKCA Movegen\n");
                                        AddQuietMove(pos,MOVE(E8,G8,EMPTY,EMPTY,MoveFlagCastle),list);

                                }
                        }
                }
                if(pos->castlePermission & BQCA)
                {
                        if(pos->pieces[D8] == EMPTY && pos->pieces[C8] == EMPTY && pos->pieces[B8] == EMPTY)
                        {
                                if(!SqAttacked(E8,WHITE,pos) && !SqAttacked(D8,WHITE,pos))
                                {
                                        // printf("BQCA movegen\n");
                                        AddQuietMove(pos,MOVE(E8,C8,EMPTY,EMPTY,MoveFlagCastle),list);

                                }
                        }
                }
        }
        // Loop for slide pieces
        pceIndex = LoopSlideIndex[side];
        pce = LoopSlidePce[pceIndex++];

        while(pce != 0)
        {
                ASSERT(PieceValid(pce));
                // printf("sliders pceIndex: %d pce: %d\n",pceIndex,pce);

                for(pceNum = 0; pceNum < pos->piecesNum[pce]; pceNum++)
                {
                        sq = pos->pList[pce][pceNum];
                        ASSERT(SqOnBoard(sq));
                        // printf("Piece:%c on %s\n",PceChar[pce],PrSq(sq));

                        for(index = 0; index < NumDir[pce]; index++)
                        {
                                dir = pceDir[pce][index];
                                t_sq = sq + dir;    //target square

                                while(!SQOFFBOARD(t_sq))
                                {
                                        // BLACK ^ 1 == WHITE   WHITE ^ 1 == BLACK
                                        if(pos->pieces[t_sq] != EMPTY)
                                        {
                                                if(PieceCol[pos->pieces[t_sq]] == (side ^ 1))
                                                {
                                                        // printf("\t\tCapture on %s\n",PrSq(t_sq));
                                                        AddCaptureMove(pos,MOVE(sq,t_sq,pos->pieces[t_sq],EMPTY,0),list);
                                                }
                                                break;
                                        }
                                        // printf("\t\tNormal on %s\n",PrSq(t_sq));
                                        AddQuietMove(pos,MOVE(sq,t_sq,EMPTY,EMPTY,0),list);
                                        t_sq += dir;
                                }
                        }
                }
                pce = LoopSlidePce[pceIndex++];
        }
        //no sliders loop
        pceIndex = LoopNonSlideIndex[side];
        pce = LoopNonSlidePce[pceIndex++];
        while(pce != 0)
        {
                ASSERT(PieceValid(pce));
                // printf("non sliders pceIndex %d pce:%d\n",pceIndex,pce);

                for(pceNum = 0; pceNum < pos->piecesNum[pce]; pceNum++)
                {
                        sq = pos->pList[pce][pceNum];
                        ASSERT(SqOnBoard(sq));
                        // printf("Piece:%c on %s\n",PceChar[pce],PrSq(sq));

                        for(index = 0; index < NumDir[pce]; index++)
                        {
                                dir = pceDir[pce][index];
                                t_sq = sq + dir;    //target square

                                if(SQOFFBOARD(t_sq))
                                {
                                        continue;
                                }
                                // BLACK ^ 1 == WHITE   WHITE ^ 1 == BLACK

                                if(pos->pieces[t_sq] != EMPTY)
                                {
                                        if(PieceCol[pos->pieces[t_sq]] == side ^ 1)
                                        {
                                                // printf("\t\tCapture on %s\n",PrSq(t_sq));
                                                AddCaptureMove(pos,MOVE(sq,t_sq,pos->pieces[t_sq],EMPTY,0),list);
                                        }
                                        continue;
                                }
                                // printf("\t\tNormal on %s\n",PrSq(t_sq));
                                AddQuietMove(pos,MOVE(sq,t_sq,EMPTY,EMPTY,0),list);
                        }
                }
                pce = LoopNonSlidePce[pceIndex++];
        }

}