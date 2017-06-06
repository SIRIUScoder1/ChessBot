//board.c

#include "stdio.h"
#include "defi.h"

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

        ASSERT(pos->enPasSquare==NO_SQ || ( RankBrd[pos->enPasSquare]==RANK_6 && pos->side == WHITE)
               || ( RankBrd[pos->enPasSquare]==RANK_3 && pos->side == BLACK));

        ASSERT(pos->pieces[pos->KingSquare[WHITE]] == wK);
        ASSERT(pos->pieces[pos->KingSquare[BLACK]] == bK);

        return TRUE;
}


void UpdateListMaterial(S_BOARD *pos)
{
        int piece,sq, index, colour;
        for(index = 0; index < BOARD_SQUARES; index++)
        {
                sq = index;
                piece = pos->pieces[index];
                if(piece != OFFBOARD && piece != EMPTY)
                {
                        colour = PieceCol[piece];
                        if(PieceBig[piece] == TRUE)
                                pos->bigPieces[colour]++;
                        if(PieceMin[piece] == TRUE)
                                pos->minPieces[colour]++;
                        if(PieceMaj[piece] == TRUE)
                                pos->majPieces[colour]++;
                        pos->material[colour] += PieceVal[piece];

                        //piece list
                        //plist[wp][0]; First white pawn white pawn a 0 index
                        //plist[wp][1]; //incrementing the pos->piecesNum[piece]
                        pos->pList[piece][pos->piecesNum[piece]] = sq;
                        pos->piecesNum[piece]++;

                        if(piece == wK)
                                pos->KingSquare[WHITE] = sq;
                        if(piece == bK)
                                pos->KingSquare[BLACK] = sq;

                        if(piece == wP)
                        {
                                SETBIT(pos->pawns[WHITE],SQ64(sq)); // setting bit for pawns and also converting the sq in 64 indexing from 120 indexing
                                SETBIT(pos->pawns[BOTH],SQ64(sq));
                        }
                        else if(piece == bP)
                        {
                                SETBIT(pos->pawns[BLACK],SQ64(sq));
                                SETBIT(pos->pawns[BOTH],SQ64(sq));
                        }
                }
        }
}


int ParseFen(char *fen, S_BOARD *pos)
{
        ASSERT(fen != NULL);
        ASSERT(pos != NULL);

        int rank = RANK_8;
        int file = FILE_A;
        int piece = 0;
        int count = 0;
        int i = 0;
        int sq64 = 0;
        int sq120 = 0;
        ResetBoard(pos);
        while((rank >= RANK_1) && *fen)
        {
                count = 1;
                switch(*fen)
                {
                case 'p': piece = bP; break;
                case 'r': piece = bR; break;
                case 'n': piece = bKn; break;
                case 'b': piece = bB; break;
                case 'k': piece = bK; break;
                case 'q': piece = bQ; break;
                case 'P': piece = wP; break;
                case 'R': piece = wR; break;
                case 'N': piece = wKn; break;
                case 'B': piece = wB; break;
                case 'K': piece = wK; break;
                case 'Q': piece = wQ; break;

                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                case '8':
                        piece = EMPTY;
                        count = *fen - '0';
                        break;
                case '/':
                case ' ':
                        rank--;
                        file = FILE_A;
                        fen++;
                        continue;
                default:
                        printf("FEN error\n");
                        return -1;
                }
                //  FILE_A FILE_B FILE_C FILE_D FILE_E RANK_4
                //  4RKQP/
                for(i = 0; i < count; i++)
                {
                        sq64 = rank * 8 + file;
                        sq120 = SQ120(sq64);
                        if(piece != EMPTY)
                        {
                                pos->pieces[sq120] = piece;
                        }
                        file++;
                }
                fen++;
        }
        ASSERT(*fen == 'w' || *fen == 'b');
        pos->side = (*fen == 'w') ? WHITE : BLACK;
        fen += 2;

        for(i = 0; i < 4; i++)
        {
                if(*fen == ' ')
                        break;
                switch(*fen)
                {
                case 'K': pos->castlePermission |= WKCA; break;
                case 'Q': pos->castlePermission |= WQCA; break;
                case 'k': pos->castlePermission |= BKCA; break;
                case 'q': pos->castlePermission |= BQCA; break;
                default:
                        break;
                }
                fen++;
        }
        fen++;

        ASSERT(pos->castlePermission >= 0 && pos->castlePermission <= 15)

        //checking the enpassent square whether its set or not
        if(*fen != '-')
        {
                file = fen[0] - 'a';
                rank = fen[1] - '1';
                ASSERT(file>= FILE_A && file <= FILE_H);
                ASSERT(rank>= RANK_1 && rank <= RANK_8);

                pos->enPasSquare = FR2SQ(file,rank);
        }

        pos->posKey = GeneratePosKey(pos);
        UpdateListMaterial(pos);
        return 0;
}
//reset all values on the board to 0
void ResetBoard(S_BOARD *pos)
{
        int index = 0;
        for(index = 0; index < BOARD_SQUARES; index++)
        {
                pos->pieces[index] = OFFBOARD;
        }
        for(index = 0; index < 64; index++)
        {
                pos->pieces[SQ120(index)] = EMPTY;
        }

        for(index = 0; index < 2; index++)
        {
                pos->bigPieces[index] = 0;
                pos->majPieces[index] = 0;
                pos->minPieces[index] = 0;
                pos->material[index] = 0;   //initially setting to zero for every piece
        }
        for(index = 0;index < 3;index++)
        {
            pos->pawns[index] = 0ULL;
        }

        for(index = 0; index < 13; index++)
        {
                pos->piecesNum[index] = 0;
        }

        pos->KingSquare[WHITE] = pos->KingSquare[BLACK] = NO_SQ;

        pos->side = BOTH;
        pos->enPasSquare = NO_SQ;
        pos->fiftyMoves = 0;

        pos->play = 0;
        pos->histPlay = 0;

        pos->castlePermission = 0;

        pos->posKey = 0ULL;     //current hashkeys
        InitPvTable(pos->PvTable);
}

void PrintBoard(const S_BOARD *pos)
{
        int sq,file,rank,piece;
        printf("\n Game Board: \n\n");

        for(rank = RANK_8; rank >= RANK_1; rank--) {
                printf("%d  ",rank+1);
                for(file = FILE_A; file <= FILE_H; file++) {
                        sq = FR2SQ(file,rank);
                        piece = pos->pieces[sq];
                        printf("%3c",PceChar[piece]);
                }
                printf("\n");
        }
        printf("\n  ");
        for(file = FILE_A; file <= FILE_H; file++)
        {
                printf("%3c", 'a'+file);
        }
        printf("\n");
        printf("side: %c\n",SideChar[pos->side]);
        printf("enPasSquare: %d\n",pos->enPasSquare);
        printf("castlePermission: %c %c %c %c\n",
                pos->castlePermission & WKCA ? 'K' : '-',
               pos->castlePermission & WQCA ? 'Q' : '-',
               pos->castlePermission & BKCA ? 'k' : '-',
               pos->castlePermission & BQCA ? 'q' : '-');
        printf("posKey:%llX\n", pos->posKey);
}
