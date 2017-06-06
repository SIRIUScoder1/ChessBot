#ifndef DEFS_H
#define DEFS_H
#include <stdio.h>
#include <stdlib.h>

// #define DEBUG
//    debugger

#ifndef DEBUG
#define ASSERT(n)
#else
#define ASSERT(n) \
        if(!(n)) {  \
                printf("%s - Failed",# n); \
                printf("On %s ",__DATE__);    \
                printf("At %s ",__TIME__);    \
                printf("In File %s ",__FILE__);   \
                printf("At Line %d\n",__LINE__);  \
                exit(1); }
#endif
typedef unsigned long long U64;



#define NAME "chess 1.0"
#define BOARD_SQUARES 120

#define MAXGAMEMOVES  2048    //max possible moves in a game storing the history of game
#define MAXPOSITIONMOVES 256   // max possible moves in a given position
//initial condition of the START board
#define MAXDEPTH 64
#define START_FEN "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"


enum {  EMPTY, wP, wKn, wB, wR, wQ, wK, bP, bKn, bB, bR, bQ, bK};

//Files

enum {FILE_A, FILE_B, FILE_C, FILE_D, FILE_E, FILE_F, FILE_G, FILE_H, FILE_NONE};
//Ranks

enum {RANK_1, RANK_2, RANK_3, RANK_4, RANK_5, RANK_6,RANK_7, RANK_8,RANK_NONE};

enum {WHITE, BLACK, BOTH};

//board
enum {
        A1 = 21, B1, C1, D1, E1, F1, G1, H1,
        A2 = 31, B2, C2, D2, E2, F2, G2, H2,
        A3 = 41, B3, C3, D3, E3, F3, G3, H3,
        A4 = 51, B4, C4, D4, E4, F4, G4, H4,
        A5 = 61, B5, C5, D5, E5, F5, G5, H5,
        A6 = 71, B6, C6, D6, E6, F6, G6, H6,
        A7 = 81, B7, C7, D7, E7, F7, G7, H7,
        A8 = 91, B8, C8, D8, E8, F8, G8, H8, NO_SQ,OFFBOARD
};

enum {FALSE, TRUE};

//    0 represents that cannot castle and 1 represents can castle that particular side
//        1 2 4 8
//        0 0 0 0
enum {WKCA = 1, WQCA = 2, BKCA = 4, BQCA = 8};

//  WHITE KING SIDE CASTLING, WHITE QUEEN SIDE CASTLING, BLACK KING SIDE CASTLING, BLACK QUEEN SIDE CASTLING

typedef struct
{
        int move; //storing all the information about the move
        int score;
}  S_MOVE;


// storng all possible moves in move list in array
typedef struct
{
        S_MOVE moves[MAXPOSITIONMOVES]; // storing the moves
        int count;  //number of moves on movelist
} S_MOVELIST;

typedef struct
{
        U64 posKey; //storing the position key if the alpha changes
        int move; //storing the move
}S_PVENTRY;

typedef struct
{
        S_PVENTRY *pTable;  //hash table
        int numEntries;
}S_PVTABLE;
//undo function for a move
typedef struct {

        int move; //current move played
        //below are statuses before the move was made
        int castlePermission; //if castle done then revert the bit
        int enPasSquare;  //if enpassent done the revert it
        int fiftyMoves;   //if fifty Move status revert to previous status
        U64 posKey; //revert the status

} S_UNDO;

//Board declarations

typedef struct {

        int pieces[BOARD_SQUARES];
        U64 pawns[3]; // 00100000 00000000 00000000 00000000 00000000 00000000 00000000 00000000

        int KingSquare[2];

        int side; //current side to move
        int enPasSquare; //if there is any one active
        int fiftyMoves; //for one side max plays fifty moves

        int play; // how many half moves in currnet search
        int histPlay; //total game moves played so far

        int castlePermission;
        U64 posKey; //unique key for each position generated

        int piecesNum[13]; //number of pieces on the board
        int bigPieces[2]; //any piece other than pawn   -- white, black
        int majPieces[2]; //rooks and queens numbers    -- white, black
        int minPieces[2]; //bishops and knights numbers -- white, black
        int material[2];   // values of material scores black and white

        S_UNDO history[MAXGAMEMOVES];

        //piece list
        int pList[13][10];

        S_PVTABLE PvTable[1];
        int PvArray[MAXDEPTH];

        int searchHistory[13][BOARD_SQUARES];  // everytime the move improves on alpha will reset this to 0
        // when a piece or move beats alpha we will increment(+1) and change the values in the array
        int searchKillers[2][MAXDEPTH]; //two moves that recently caused beta cuttoff (move ordering)

        //  plist[wN][0] = E1;
        // plist[wN][1] = D4;.......NO_square returned when last element
} S_BOARD;

typedef struct
{
        // how long the chess bot will think for
        int starttime;
        int stoptime;
        int depth;
        int depthset;
        int timeset;
        int movestogo;
        int infinite;

        long nodes; // count the number of position the bot visits in a search tree

        int quit;
        int stopped;  // stop seaching (signal sent through gui)
}S_SEARCHINFO;
/* GAME MOVE (shown is hexadecimal form)*/

// when we have bits for a number
// 1 2 4 8 16 32 64 128  we need upto seven bits to represent a MOVE
//F
/*  0000  0000  0000  0000  0000  0111  1111  */          //from square has 7 bits as 1 (0x7F) size of from square
/*  0000  0000  0000  0011  1111  1000  0000   */         //to square as next 7 bits as 1       // To >> 7   (shifting the 7 bits of To towards the right) sizeofTo (0x7F)
/*  0000  0000  0011  1100  0000  0000  0000   */         //  there are 12 pieces and captured pieces   (captured >> 14) size 0xF
/*  0000  0000  0100  0000  0000  0000  0000   */         // enpassent capture  0x40000
/*  0000  0000  1000  0000  0000  0000  0000   */         // pawn start      0x80000
/*  0000  1111  0000  0000  0000  0000  0000   */         // promoted piece >> 20, 0xF
/*  0001  0000  0000  0000  0000  0000  0000   */         // castle move    0x1000000

#define FROM(m) ((m) & 0x7F)
#define To(m) (((m) >> 7) & 0x7F)
#define CAPTURED(m) (((m) >> 14) & 0xF)
#define PROMOTED(m) (((m) >> 20) & 0xF)

#define MoveFlag 0x40000        // enpassent SQUARE MOVE
#define MoveFlagPawnStart 0x80000
#define MoveFlagCastle 0x1000000

#define MoveFlagCapture 0x7C000     //captered piece or not and which one is it
#define MoveFlagPromoted 0xF00000   // promoted pawn to which piece

#define NOMOVE 0



/* MACRO */
//for 64 square board it returns the 120 square number
#define FR2SQ(f,r) ((21 + (f)) + ((r)*10))
#define SQ64(sq120)(Square120ToSquare64[(sq120)])  //120 based index to 64 based index
#define SQ120(sq64) (Square64ToSquare120[(sq64)])   //64 based index to 120 based index
#define POP(b) PopBit(b)
#define CNT(b) CountBits(b)
#define CLRBIT(bb,sq) ((bb) &= ClearMask[(sq)])
#define SETBIT(bb,sq) ((bb) |= SetMask[(sq)])

//  Checking the piece which piece is it
#define IsBQ(p) (PieceBishopQueen[(p)])
#define IsRQ(p) (PieceRookQueen[(p)])
#define IsKn(p) (PieceKnight[(p)])
#define IsKi(p) (PieceKing[(p)])


/* Globals */
extern int Square120ToSquare64[BOARD_SQUARES];
extern int Square64ToSquare120[64];
extern U64 SetMask[64];
extern U64 ClearMask[64];
extern U64 PieceKeys[13][120];
extern U64 SideKey;
extern U64 CastleKeys[16];
extern char PceChar[];
extern char SideChar[];
extern char RankChar[];
extern char FileChar[];


extern int PieceBig[13];    //bigPieces
extern int PieceMaj[13];    //major pieces
extern int PieceMin[13];    //minor pieces
extern int PieceVal[13];    // value
extern int PieceCol[13];    //color
extern int PiecePawn[13];

extern int FileBrd[BOARD_SQUARES];
extern int RankBrd[BOARD_SQUARES];

// is a piece a knight, king, rook or queen, bishop or queen
extern int PieceKnight[13];
extern int PieceKing[13];
extern int PieceRookQueen[13];
extern int PieceBishopQueen[13];
extern int PieceSlides[13];

/* FUNCITONS */
//init.c
extern void initialAll();
extern void InitFileRankBrd();
//bitBoard.c
extern void PrintBitBoard(U64 bitb);
extern int PopBit(U64 *bb);
extern int CountBits(U64 b);

//hashkeys.c
extern U64 GeneratePosKey(const S_BOARD *pos);
//board.c
extern void ResetBoard(S_BOARD *pos);
extern int ParseFen(char *fen, S_BOARD *pos);
extern void PrintBoard(const S_BOARD *pos);
extern void UpdateListMaterial(S_BOARD *pos);
extern int CheckBoard(const S_BOARD *pos);

// attack.c functins

extern int SqAttacked(const int sq, const int side, const S_BOARD *pos);

//io.c
extern char *PrMove(const int move);
extern char *PrSq(const int sq);
extern void PrintMoveList(const S_MOVELIST *list);
extern int ParseMove(char *ptrChar, S_BOARD *pos);

//validate.c
extern int SqOnBoard(const int sq);
extern int SideValid(const int side);
extern int FileRankValid(const int fr);
extern int PieceValidEmpty(const int pce);
extern int PieceValid(const int pce);

//movegen.c
extern void GenerateAllMoves(const S_BOARD *pos, S_MOVELIST *list);
extern int MoveExists(S_BOARD *pos,const int move);

// makemove.c
extern int MakeMove(S_BOARD *pos, int move);
extern void TakeMove(S_BOARD *pos);

//perft.c
extern void PerftTest(int depth, S_BOARD *pos);

// search.c
extern void SearchPosition(S_BOARD *pos,S_SEARCHINFO *info);

// misc.c
extern int GetTimeMs();

//pvtable.c
extern void InitPvTable(S_PVTABLE*table);
extern void StorePvMove(const S_BOARD *pos, const int move);
extern int ProbePvTable(const S_BOARD *pos);
extern int GetPvLine(const int depth, S_BOARD *pos);

//evaluate.c
extern int EvalPosition(const S_BOARD *pos);
#endif
