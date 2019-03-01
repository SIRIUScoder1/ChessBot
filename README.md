# ChessBot using AI Algorithms

#### Introduction

ChessBot is a game engine which has the capability of playing chess with a human and with another game engine. This game engine uses various algorithms like Alpha-Beta Pruning, PositionKey Algorithm, Best First Search etc. and data structures like BitBoards and MoveTrees.

#### Architecture
- ##### DESIGN AND INITIALIZATION OF THE BOARD
  The most important phase for implementing a game engine is to design the structure of the game and list all the possible rules or configurations required before actually implementing the AI engine logic. So, in this module, definition the board structure for the game (chess) and design a platform for performing moves, storing each board configuration on performing a single move and then generation of new moves based on existing board configuration.
  For making the generation of moves simpler, a 120 based linear array is chosen.
  This array is displayed in the form of two-dimensional matrix in figure as shown
  
  (a) With off board squares &nbsp;&nbsp; &nbsp; &nbsp; &nbsp; 
  &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;
  &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;
  &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;
  (b) Without off board squares
  
  ![image](https://github.com/SIRIUScoder1/ChessBot/blob/master/images/chessBot1.png)
  
  Figure 2.1: 120 Board based representation
  
  The gray squares represent the OFFBOARD squares which means that if there is a piece on one of these squares, then it is not on the chessboard. 120-based array was basically chosen due to knight's move. Suppose the knight is on square number 24 in the 120-based array. It can move to 6 possible locations namely to squares 3, 5, 32,
36, 43, 45 of which squares 3 and 5 OFFBOARD squares. So, these squares will not come in the possible generated moves and across the both sides there is single grey column because even though it's shown as two-dimensional grid, but in reality it is a linear array. Even if there is a knight on a certain square (Suppose square 38), one of its
possible moves will be to square number 20. 

  Overall in the board structure there are certain things which need to be stored. So a
structure board is defined which consists of:
 
  -	A linear array equal to number of squares on the board (120) which will store the type of piece on each square
  - A 64bit-linear array of size three, to store the pawn configuration on the board in the form of BitBoards. The first bit stores the WHITE pawns configuration, second bit the BLACK pawns configuration and third one stores configuration of BOTH white and black pawns.
  - Fifty move rule
  - Side to move
  - Kings Square array of size two for BLACK and WHITE
  - Number of half moves played
  - History of Play
  - Position Key: Store the configuration of the whole board in a unique hash key.
  - Piece Lists: Stores the squares on which the particular piece is present.
  - Storing number of BigPieces, MajorPieces, MinorPieces.
  - Material count for calculating the score of the player and evaluating the position of the player.
  - Castling Permissions for the player
  
