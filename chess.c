//chess.c

#include "stdio.h"
#include "defs.h"
#include "stdlib.h"

#define WAC1 "r1b1k2r/ppppnppp/2n2q2/2b5/3NP3/2P1B3/PP3PPP/RN1QKB1R w KQkq - 0 1"
#define WAC2 "2rr3k/pp3pp1/1nnqbN1p/3pN3/2pP4/2P3Q1/PPB4P/R4RK1 w - -"
// #define PERFTFEN "n1n5/PPPk4/8/8/8/8/4Kppp/5N1N w - - 0 1"
#define PERFT "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1"
int main() {

        AllInit();
        Uci_Loop();
        // S_BOARD board[1];
        // InitPvTable(board->PvTable);
        //
        // S_MOVELIST list[1];
        // S_SEARCHINFO info[1];
        // ParseFen(WAC1,board);
        //
        // char input[6];
        // int Move = NOMOVE;
        // int PvNum = 0;
        // int Max = 0;
        // while(TRUE) {
        //         PrintBoard(board);
        //         printf("Please enter a move > ");
        //         fgets(input, 6, stdin);
        //
        //         if(input[0]=='q') {
        //                 break;
        //         } else if(input[0]=='t') {
        //                 TakeMove(board);
        //         } else if(input[0]=='s') {
        //                 info->depth = 6;
        //                 info->starttime = GetTimeMs();
        //                 info->stoptime = GetTimeMs()-200000;
        //                 SearchPosition(board, info);
        //         } else {
        //                 Move = ParseMove(input, board);
        //                 if(Move != NOMOVE) {
        //                         StorePvMove(board, Move);
        //                         MakeMove(board,Move);
        //                 } else {
        //                         printf("Move Not Parsed:%s\n",input);
        //                 }
        //         }
        //
        //         fflush(stdin);
        // }
        // free(board->PvTable->pTable);
        return 0;
}



// #include "stdio.h"
// #include "stdlib.h"
// #include "defi.h"
//
// // #define FEN0 "8/3q4/8/8/4Q3/8/8/8 w - - 0 2"
// // #define FEN5 "8/3q1p2/8/5P2/4Q3/8/8/8 w - - 0 2"
// // #define FEN1 "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1"
// // #define FEN2 "rnbqkbnr/pp1ppppp/8/2p5/4P3/8/PPPP1PPP/RNBQKBNR w KQkq c6 0 2"
// // #define FEN3 "rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R b KQkq - 1 2"
// // #define FEN4 "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1"
//
// #define PAWNMOVESW "rnbqkb1r/pp1p1pPp/8/2p1pP2/1P1P4/3P3P/P1P1P3/RNBQKBNR w KQkq e6 0 1"
// #define PAWNMOVESB "rnbqkbnr/p1p1p3/3p3p/1p1p4/2P1Pp2/8/PP1P1PpP/RNBQKB1R b KQkq e3 0 1"
// #define KNIGHTSKINGS "5k2/1n6/4n3/6N1/8/3N4/8/5K2 w - - 0 1"
// #define ROOKS "6k1/8/5r2/8/1nR5/5N2/8/6K1 b - - 0 1"
// #define QUEENS "6k1/8/4nq2/8/1nQ5/5N2/1N6/6K1 b - - 0 1"
// #define BISHOPS "6k1/1b6/4n3/8/1n4B1/1B3N2/1N6/2b3K1 b - - 0 1"
// #define CASTLE1 "r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1"
// // #define CASTLE2 "3rk2r/8/8/8/8/8/6p1/R3K2R b KQk - 0 1"
// #define CASTLE2 "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1"
// #define PERFTFEN1 "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1"
// #define PERFTFEN "n1n5/PPPk4/8/8/8/8/4Kppp/5N1N w - - 0 1"
//


// void ShowSqAtBySide(const int side, const S_BOARD *pos)
// {
//         int rank = 0;
//         int file = 0;
//         int sq = 0;
//
//         printf("\n \nSquares attacked by:%c\n",SideChar[side]);
//
//         for(rank = RANK_8; rank >= RANK_1; --rank)
//         {
//                 for(file = FILE_A; file <= FILE_H; file++)
//                 {
//                         sq = FR2SQ(file,rank);
//                         if(SqAttacked(sq,side,pos) == TRUE)
//                         {
//                                 printf("X");
//                         }
//                         else
//                         {
//                                 printf("-");
//                         }
//                 }
//                 printf("\n");
//         }
//         printf("\n\n");
// }

// void PrintBin(int move)
// {
//         int index = 0;
//         printf("As binary:\n");
//         for(index = 27; index >= 0; index--)
//         {
//                 if((1 << index) & move)
//                         printf("1");
//                 else
//                         printf("0");
//                 if(index != 28 && index % 4 == 0)
//                         printf(" ");
//         }
//         printf("\n");
// }
// int main()
// {
//         initialAll();
//         S_BOARD board[1];
//         S_MOVELIST list[1];
//
//
//         ParseFen(START_FEN,board);
//         // PerftTest(5,board);
//         char input[6];
//         int Move = NOMOVE;
//         while(TRUE)
//         {
//             // printf("yo1\n");
//             PrintBoard(board);
//             printf("Please enter a move > ");
//             scanf("%s",input);
//         //
//             if(input[0] == 'q')
//               break;
//             else if(input[0] == 't')
//             {
//                 TakeMove(board);
//             }
//             else if(input[0] == 'p')
//             {
//                 PerftTest(4,board);
//             }
//             else
//             {
//                   Move = ParseMove(input,board);
//                   if(Move != NOMOVE)
//                   {
//                       MakeMove(board,Move);
//                       // if(IsRepetition(board))
//                       // {
//                       //     printf("REP SEEN\n");
//                       // }
//                       // printf("yo2\n");
//                   }
//                   else
//                   {
//                       printf("Move Not Parsed:%s\n",input);
//                   }
//             }
// fflush(stdin);
// }
// PrintBoard(board);

//
// GenerateAllMoves(board,list);
//
// int MoveNum = 0;
// int move = 0;

// PrintBoard(board);
// getchar();
// for(MoveNum = 0;MoveNum < list->count;MoveNum++)
// {
//     move = list->moves[MoveNum].move;
//     if(!MakeMove(board,move))
//     {
//         continue;
//     }
//   printf("\nMADE:%s\n",PrMove(move));
//   PrintBoard(board);
//   TakeMove(board);
//   printf("\nTAKEN;%s\n",PrMove(move));
//   PrintBoard(board);
//   getchar();
// }
// PrintMoveList(list);

// return 0;
// int move = 0;
// int from = A2;
// int to = H7;
// int cap = wR;
// int prom = wB;
// move = ((from) | (to << 7) | (cap << 14) | (prom << 20));
//
// printf("from:%d to:%d cap:%d prom:%d\n",FROM(move),To(move), CAPTURED(move), PROMOTED(move));
// printf("Algebriac from:%s\n",PrSq(from));
// printf("Algebriac to:%s\n",PrSq(to));
// printf("Algebriac move:%s\n",PrMove(move));

// S_BOARD board[1];
// ParseFen(FEN4,board);
// PrintBoard(board);
// ASSERT(CheckBoard(board));
//
// int move = 0;
//
// int from = 6; int to = 12; int captured = wR; int prom = bR;
// // From square
// move = ((6) | (12 << 7) | (captured << 14) | (prom << 20));
// printf("\ndec:%d hex:%X\n",move,move);
// PrintBin(move);
//
// printf("From:%d To:%d cap :%d prom :%d\n",
//        FROMSQ(move),TOSQ(move),CAPTURED(move), PROMOTED(move));
// printf("\n\n White Attacking:\n");
// ShowSqAtBySide(WHITE,board);
// printf("\n\n Black Attacking:\n");
// ShowSqAtBySide(BLACK,board);
// return 0;
// ASSERT(CheckBoard(board));
// printf("Forced asserts...\n");
// board->piecesNum[wP]--;
// board->posKey ^= SideKey;
// ASSERT(CheckBoard(board));
// printf("\nwP:\n");
//
//
// PrintBitBoard(board->pawns[WHITE]);
// printf("\nbP:\n");
// PrintBitBoard(board->pawns[BLACK]);
// printf("\nall p:\n");
// PrintBitBoard(board->pawns[BOTH]);
// ParseFen(FEN1,board);
// PrintBoard(board);
//
// ParseFen(FEN2, board);
// PrintBoard(board);
//
// ParseFen(FEN3, board);
// PrintBoard(board);

// return 0;
// int PieceOne = rand();
// int PieceTwo = rand();
// int PieceThree = rand();
// int PieceFour = rand();
//
// printf("PieceOne: %X\n", PieceOne);
// printf("PieceTwo: %X\n", PieceTwo);
// printf("PieceThree: %X\n", PieceThree);
// printf("PieceFour: %X\n", PieceFour);
//
// int Key = PieceOne ^ PieceTwo ^ PieceThree ^ PieceFour;
// int TempKey = PieceOne;
// TempKey ^= PieceThree;
// TempKey ^= PieceFour;
// TempKey ^= PieceTwo;

// int index = 0;
// U64 playBitBoard = 0ULL;
// for(index = 0; index < 64; index++)
// {
//         printf("Index:%d\n",index);
//         PrintBitBoard(ClearMask[index]);
//         printf("\n");
// }


// SETBIT(playBitBoard,61);
// PrintBitBoard(playBitBoard);
// U64 playBitBoard = 0ULL;
//
// playBitBoard |= (1ULL << SQ64(D2));
// playBitBoard |= (1ULL << SQ64(D3));
// playBitBoard |= (1ULL << SQ64(D4));
//
// int sq64 = 0;
// while(playBitBoard)
// {
//         sq64 = POP(&playBitBoard);
//         printf("popped:%d\n",sq64);
//         PrintBitBoard(playBitBoard);
// }
// printf("\n");
// PrintBitBoard(playBitBoard);
//
// int count = CNT(playBitBoard);
//
// printf("Count: %d\n",count);
//
// int index = POP(&playBitBoard);
// printf("index:%d\n",index);
// PrintBitBoard(playBitBoard);
// count = CNT(playBitBoard);
// printf("Count:%d\n",count);
// printf("start:\n\n");
// PrintBitBoard(playBitBoard);

// playBitBoard |= (1ULL << SQ64(D2));
// printf("D2 Added:\n\n");
// PrintBitBoard(playBitBoard);

// playBitBoard |= (1ULL << SQ64(G2));
// printf("G2 Added:\n\n");
// PrintBitBoard(playBitBoard);
// ASSERT(num == nuts);
// int index = 0;
// for(index = 0; index < BOARD_SQUARES; ++index)
// {
//         if(index % 10 == 0)
//                 printf("\n");
//         printf("%5d",Square120ToSquare64[index]);
// }
// printf("\n");
// printf("\n");
// for(index = 0; index < 64; ++index)
// {
//         if(index % 8 == 0)
//                 printf ("\n");
//         printf("%5d", Square64ToSquare120[index]);
// }
// printf("\n");
// }
