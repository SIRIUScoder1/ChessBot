all:
	gcc chess.c uci.c evaluate.c pvtable.c init.c bitBoard.c hashkey.c board.c data.c attack.c io.c movegen.c validate.c makemove.c perft.c search.c misc.c -o chess
