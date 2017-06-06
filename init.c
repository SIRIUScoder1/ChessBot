//init.c

#include "defi.h"
#include "stdlib.h"
#include "stdio.h"

//     0000 0000000000000000 0000000000000000 0000000000000000 111111111111111  suppose rand()
//     0000 0000000000000000 0000000000000000 111111111111111  0000000000000000 15 bits left shift
//     0000 0000000000000000 111111111111111 0000000000000000  0000000000000000 30 bits left shift
//     0000 111111111111111 0000000000000000 0000000000000000 0000000000000000  45 bits left shift
// last is & with 4 bits of random number and shift that by 60 so we fill the first 4 bits
// and then we add all these bits of numbers


#define RAND_64 ((U64)rand() | \
                 (U64)rand() << 15 | \
                 (U64)rand() << 30 | \
                 (U64)rand() << 45 | \
                 ((U64)rand() & 0xf) << 60 )
//initialization of all the things before starting the game

int Square120ToSquare64[BOARD_SQUARES];
int Square64ToSquare120[64];

U64 SetMask[64];
U64 ClearMask[64];

U64 PieceKeys[13][120];
U64 SideKey;
U64 CastleKeys[16];

int FileBrd[BOARD_SQUARES];
int RankBrd[BOARD_SQUARES];

void InitFileRankBrd()
{
        int index = 0;
        int file = FILE_A;
        int rank = RANK_1;
        int sq = A1;
        int sq64 = 0;

        for(index = 0; index < BOARD_SQUARES; index++)
        {
                FileBrd[index] = OFFBOARD;
                RankBrd[index] = OFFBOARD;
        }

        for(rank = RANK_1; rank <= RANK_8; rank++)
        {
                for(file = FILE_A; file <= FILE_H; file++)
                {
                        sq = FR2SQ(file,rank);
                        FileBrd[sq] = file;
                        RankBrd[sq] = rank;
                }
        }
        // printf("FileBrd\n");
        // for(index = 0; index < BOARD_SQUARES; index++)
        // {
        //         if(index % 10 == 0 && index != 0)
        //                 printf("\n");
        //         printf("%4d", FileBrd[index]);
        // }
        // printf("\nRankBrd\n");
        // for(index = 0; index < BOARD_SQUARES; index++)
        // {
        //         if(index % 10 == 0 && index != 0)
        //                 printf("\n");
        //         printf("%4d", RankBrd[index]);
        // }
}
void InitHashKeys()
{
        int index = 0;
        int index2 = 0;
        for(index = 0; index < 13; index++)
        {
                for(index2 = 0; index2 < 120; index2++)
                {
                        PieceKeys[index][index2] = RAND_64;
                }
        }
        SideKey = RAND_64;
        for(index = 0; index < 16; index++)
        {
                CastleKeys[index] = RAND_64;
        }
}
void InitBitMasks()
{
        int index = 0;

        for(index = 0; index < 64; index++)
        {
                SetMask[index] = 0ULL;
                ClearMask[index] = 0ULL;
        }
        for(index = 0; index < 64; index++)
        {
                SetMask[index] |= (1ULL << index);
                ClearMask[index] = ~SetMask[index];
        }
}
void InitSq120To64()
{
        int index = 0;
        int file = FILE_A;
        int rank = RANK_1;
        int sq = A1;
        int sq64 = 0;
        for(index = 0; index < BOARD_SQUARES; ++index)
                Square120ToSquare64[index] = 65;  //impossible value

        for(index = 0; index < 64; ++index)
                Square64ToSquare120[index] = 120;  //impossible value

        for(rank = RANK_1; rank <= RANK_8; ++rank)
        {
                for(file = FILE_A; file <= FILE_H; ++file)
                {
                        sq = FR2SQ(file,rank);
                        Square64ToSquare120[sq64] = sq;
                        Square120ToSquare64[sq] = sq64;
                        sq64++;
                }
        }
}
void initialAll()
{
        InitSq120To64();
        InitBitMasks();
        InitHashKeys();
        InitFileRankBrd();
}
