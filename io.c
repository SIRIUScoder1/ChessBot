//io.c
//printing square and square move algebriaclly

#include "stdio.h"
#include "defi.h"


// #define MOVE(f,t,ca,pro,f1)    ((f) | ((t) << 7) | ((ca) << 14) | ((pro) << 20) | (f1))
// #define SQOFFBOARD(sq)  (FileBrd[(sq)] == OFFBOARD)

// b2b4 moving from b2 to b4
// g2g8n moving and promoting the peice and n shows the the thing to which it is promoted to
char *PrSq(const int sq)   //square string
{
        static char SqStr[3];

        int file = FileBrd[sq];
        int rank = RankBrd[sq];

        sprintf(SqStr, "%c%c",('a' + file), ('1'+rank));
        // putting the two charaters in the square String array one after another
        // charater + file number and rank + rank number


        return SqStr;
}

char *PrMove(const int move)
{
        static char MvStr[6];

        // file from and the rank from
        // rank from and rank to
        int ff = FileBrd[FROM(move)];
        int rf = RankBrd[FROM(move)];
        int ft = FileBrd[To(move)];
        int rt = RankBrd[To(move)];

        //the piece we promoted to
        int promoted = PROMOTED(move);
        if(promoted)
        {
                char pchar = 'q';
                if(IsKn(promoted))
                {
                        pchar = 'n';
                }
                else if(IsRQ(promoted) && !IsBQ(promoted))
                {
                        pchar = 'r';
                }
                else if(!IsRQ(promoted) && IsBQ(promoted))
                {
                        pchar = 'b';
                }
                sprintf(MvStr,"%c%c%c%c%c", ('a'+ff), ('1'+rf), ('a'+ft),('1'+rt), pchar);
        }
        else
        {
                sprintf(MvStr,"%c%c%c%c", ('a'+ff), ('1'+rf), ('a'+ft),('1'+rt));
        }
        return MvStr;
}

//function that passes a move
// take a move g7g8q e1g1
int ParseMove(char *ptrChar, S_BOARD *pos)
{
      //checking a valid move
      if(ptrChar[1] > '8' || ptrChar[1] < '1')  return NOMOVE;
      if(ptrChar[3] > '8' || ptrChar[3] < '1')  return NOMOVE;
      if(ptrChar[0] > 'h' || ptrChar[0] < 'a')  return NOMOVE;
      if(ptrChar[2] > 'h' || ptrChar[2] < 'a')  return NOMOVE;

      int from = FR2SQ(ptrChar[0] - 'a',ptrChar[1]-'1');  // getting from and to squares
      int to = FR2SQ(ptrChar[2] - 'a', ptrChar[3] - '1');

      // printf("ptrChar:%s from:%d to:%d\n",ptrChar,from,to);
      ASSERT(SqOnBoard(from) && SqOnBoard(to));

      S_MOVELIST list[1];
      GenerateAllMoves(pos,list);
      int MoveNum = 0;
      int Move = 0; //actual move we are going to return
      int PromPce = EMPTY;  // promotion piece

      // loop through every single move and check which move has same from and to square

      for(MoveNum = 0;MoveNum < list->count;MoveNum++)
      {
          Move = list->moves[MoveNum].move;
          if(FROM(Move) == from && To(Move) == to)
          {
              PromPce = PROMOTED(Move);
              if(PromPce != EMPTY)
              {
                  if(IsRQ(PromPce) && !IsBQ(PromPce) && ptrChar[4] == 'r')
                    return Move;
                  else if(!IsRQ(PromPce) && IsBQ(PromPce) && ptrChar[4] == 'b')
                    return Move;
                  else if(IsRQ(PromPce) && IsBQ(PromPce) && ptrChar[4] == 'q')
                    return Move;
                  else if(IsKn(PromPce) && ptrChar[4] == 'n')
                    return Move;
                  continue;
              }
              return Move;
          }
      }
      return NOMOVE;
}
// generating the white moves in the given position
void PrintMoveList(const S_MOVELIST *list)
{
        int index = 0;
        int score = 0;
        int move = 0;
        //  taking a pointer to the move list walking through each move in the move list
        // and printing the move and the score
        printf("MoveList: %d\n",list->count);

        for(index = 0; index < list->count; ++index)
        {
                move = list->moves[index].move;
                score = list->moves[index].score;

                printf("Move:%d > %s (score:%d)\n",index+1,PrMove(move),score);
        }
        printf("MoveList Total %d Moves:\n\n", list->count);
}
