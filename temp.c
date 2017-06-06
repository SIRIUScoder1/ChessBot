#include "stdio.h"
#include "defs.h"

int CheckBoard(const S_BOARD *pos)
{
        // go through the board and fill up the piece num, bigPieces, majPieces, minPieces, and material

        int t_pceNum[13] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};    //temp array
        int t_bigPce[2] = { 0, 0};
        int t_majPce[2] = { 0, 0};
        int t_minPce[2] = { 0, 0};
        int t_material[2] = { 0, 0};


        //check whether the values stored at pos is equal to the values that we fill

        int sq64,t_piece,t_pce_num,sq120,colour,pcount;
        //store the pawn bitboard

        U64 t_pawns[3] = {0ULL, 0ULL, 0ULL};

        t_pawns[WHITE] = pos->pawns[WHITE];
        t_pawns[BLACK] = pos->pawns[BLACK];
        t_pawns[BOTH] = pos->pawns[BOTH];

        // check piece lists
        for(t_piece = wP; t_piece <= bK; ++t_piece) {
                for(t_pce_num = 0; t_pce_num < pos->piecesNum[t_piece]; ++t_pce_num) {
                        sq120 = pos->pList[t_piece][t_pce_num];
                        ASSERT(pos->pieces[sq120]==t_piece);
                }
        }

        // check piece count and other counters
        for(sq64 = 0; sq64 < 64; ++sq64) {
                sq120 = SQ120(sq64);
                // looking at the piece in the square
                t_piece = pos->pieces[sq120];
                // increamenting the number of piece type

                t_pceNum[t_piece]++;
                //check colour of the piece

                colour = PieceCol[t_piece];
                // checking the  bigPieces, minPieces and majPieces

                if( PieceBig[t_piece] == TRUE) t_bigPce[colour]++;
                if( PieceMin[t_piece] == TRUE) t_minPce[colour]++;
                if( PieceMaj[t_piece] == TRUE) t_majPce[colour]++;

                t_material[colour] += PieceVal[t_piece];
        }

        // both temporary piece number array and pieces num is same
        for(t_piece = wP; t_piece <= bK; ++t_piece) {
                ASSERT(t_pceNum[t_piece]==pos->piecesNum[t_piece]);
        }

        // check bitboards count
        pcount = CNT(t_pawns[WHITE]);
        ASSERT(pcount == pos->piecesNum[wP]);
        pcount = CNT(t_pawns[BLACK]);
        ASSERT(pcount == pos->piecesNum[bP]);
        pcount = CNT(t_pawns[BOTH]);
        ASSERT(pcount == (pos->piecesNum[bP] + pos->piecesNum[wP]));

        //check bitboards squares
        //checking the 120 based and the matching the wP
        while(t_pawns[WHITE]) {
                sq64 = POP(&t_pawns[WHITE]);
                ASSERT(pos->pieces[SQ120(sq64)] == wP);
        }

        while(t_pawns[BLACK]) {
                sq64 = POP(&t_pawns[BLACK]);
                ASSERT(pos->pieces[SQ120(sq64)] == bP);
        }

        while(t_pawns[BOTH]) {
                sq64 = POP(&t_pawns[BOTH]);
                ASSERT( (pos->pieces[SQ120(sq64)] == bP) || (pos->pieces[SQ120(sq64)] == wP) );
        }

        ASSERT(t_material[WHITE]==pos->material[WHITE] && t_material[BLACK]==pos->material[BLACK]);
        ASSERT(t_minPce[WHITE]==pos->minPieces[WHITE] && t_minPce[BLACK]==pos->minPieces[BLACK]);
        ASSERT(t_majPce[WHITE]==pos->majPieces[WHITE] && t_majPce[BLACK]==pos->majPieces[BLACK]);
        ASSERT(t_bigPce[WHITE]==pos->bigPieces[WHITE] && t_bigPce[BLACK]==pos->bigPieces[BLACK]);

        ASSERT(pos->side==WHITE || pos->side==BLACK);
        ASSERT(GeneratePosKey(pos)==pos->posKey);

        ASSERT(pos->enPasSquare==NO_SQ || ( RanksBrd[pos->enPasSquare]==RANK_6 && pos->side == WHITE)
               || ( RanksBrd[pos->enPasSquare]==RANK_3 && pos->side == BLACK));

        ASSERT(pos->pieces[pos->KingSquare[WHITE]] == wK);
        ASSERT(pos->pieces[pos->KingSquare[BLACK]] == bK);

        return TRUE;
}
