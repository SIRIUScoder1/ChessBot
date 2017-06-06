all:
	gcc chess.c init.c bitBoard.c hashkey.c board.c data.c attack.c io.c movegen.c validate.c makemove.c perft.c search.c misc.c pvtable.c evaluate.c -o chess
