//uci.c
#include <stdio.h>
#include "defs.h"
#include <string.h>

#define INPUTBUFFER 400 * 6

// go depth  whiteTime 180000 blackTime 1000000 binc 1000 winc 1000 movetime 1000 movestogo 40
void ParseGo(char* line, S_SEARCHINFO *info, S_BOARD *pos) {

        int depth = -1, movestogo = 30,movetime = -1;
        int time1 = -1, inc = 0;
        char *ptr = NULL;
        info->timeset = FALSE;

        if ((ptr = strstr(line,"infinite")))
        {
                ;
        }

        if ((ptr = strstr(line,"binc")) && pos->side == BLACK)
        {
                inc = atoi(ptr + 5);
        }

        if ((ptr = strstr(line,"winc")) && pos->side == WHITE)
        {
                inc = atoi(ptr + 5);
        }

        if ((ptr = strstr(line,"wtime")) && pos->side == WHITE)
        {
                time1 = atoi(ptr + 6);
        }

        if ((ptr = strstr(line,"btime")) && pos->side == BLACK)
        {
                time1 = atoi(ptr + 6);
        }

        if ((ptr = strstr(line,"movestogo")))
        {
                movestogo = atoi(ptr + 10);
        }

        if ((ptr = strstr(line,"movetime")))
        {
                movetime = atoi(ptr + 9);
        }

        if ((ptr = strstr(line,"depth")))
        {
                depth = atoi(ptr + 6);
        }

        if(movetime != -1)
        {
                time1 = movetime;
                movestogo = 1;
        }

        info->starttime = GetTimeMs();
        info->depth = depth;

        if(time1 != -1)
        {
                info->timeset = TRUE;
                time1 /= movestogo;
                time1 -= 50;
                info->stoptime = info->starttime + time1 + inc;
        }

        if(depth == -1)
        {
                info->depth = MAXDEPTH;
        }

        printf("time:%d start:%d stop:%d depth:%d timeset:%d\n",
               time1,info->starttime,info->stoptime,info->depth,info->timeset);
        SearchPosition(pos, info);
}

void ParsePosition(char *lineIn, S_BOARD *pos)
{
        lineIn += 9;
        char *ptrChar = lineIn;
        if(strncmp(lineIn,"startpos",8) == 0)
        {
                ParseFen(START_FEN,pos);
        }
        else
        {
                ptrChar = strstr(lineIn,"fen");
                if(ptrChar == NULL)
                {
                        ParseFen(START_FEN,pos);
                }
                else
                {
                        ptrChar += 4;
                        ParseFen(ptrChar,pos);
                }
        }
        ptrChar = strstr(lineIn,"moves");
        int move;
        if(ptrChar != NULL)
        {
                ptrChar += 6;
                while(*ptrChar)
                {
                        move = ParseMove(ptrChar,pos);
                        if(move == NOMOVE)
                                break;
                        MakeMove(pos,move);
                        pos->ply = 0;
                        while(*ptrChar && *ptrChar != ' ')
                                ptrChar++;
                        ptrChar++;
                }
        }
        PrintBoard(pos);
}

void Uci_Loop()
{
        setbuf(stdin,NULL);
        setbuf(stdout,NULL);

        char line[INPUTBUFFER];
        printf("id name %s\n",NAME);
        printf("id author KaranChessBot\n");
        printf("uciok\n");

        S_BOARD pos[1];
        S_SEARCHINFO info[1];
        InitPvTable(pos->PvTable);

        while(TRUE)
        {
                memset(&line[0],0,sizeof(line));
                fflush(stdout);
                if(!fgets(line,INPUTBUFFER,stdin))
                        continue;

                if(line[0] == '\n')
                        continue;

                if(!strncmp(line, "isready", 7))
                {
                        printf("readyok\n");
                        continue;
                }
                else if(!strncmp(line,"position",8))
                {
                        ParsePosition(line,pos);
                }
                else if(!strncmp(line,"ucinewgame",10))
                {
                        ParsePosition("position startpos\n",pos);
                }
                else if(!strncmp(line,"go",2))
                {
                        ParseGo(line,info,pos);
                }
                else if(!strncmp(line,"quit",4))
                {
                        info->quit = TRUE;
                        break;
                }
                else if(!strncmp(line,"uci",3))
                {
                        printf("id name %s\n",NAME);
                        printf("id author KaranChessBot\n");
                        printf("uciok\n");
                }
                if(info->quit)
                        break;
        }
        free(pos->PvTable->pTable);

}