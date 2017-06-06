//search.c
#include <stdio.h>
#include "defi.h"

static void CheckUp()
{
    // going to check if time up or interrupt from the GUI
}
static int IsRepetition(const S_BOARD *pos)
{
    int index = 0;
    // loop through all the moves played in the history
    // checking the last fifty moves in the game
    for(index = (pos->histPlay - pos->fiftyMoves);index < pos->histPlay-1;index++)
    {
        ASSERT(index >= 0 && index < MAXGAMEMOVES);
        if(pos->posKey == pos->history[index].posKey) // so if history and the played then return true
        {
            return TRUE;
        }
    }
    return FALSE;
}

static void ClearForSearch(S_BOARD *pos, S_SEARCHINFO *info)
{

}

static int AlphaBeta(int alpha, int beta, int depth, S_BOARD *pos, S_SEARCHINFO *info, int DoNull)
{
    return 0;
}

static int Quienscence(int alpha,int beta, S_BOARD *pos, S_SEARCHINFO *info)
{
    return 0;
}

void SearchPosition(S_BOARD *pos, S_SEARCHINFO *info)
{
    // deeping of the search (iterative deepning)

}
