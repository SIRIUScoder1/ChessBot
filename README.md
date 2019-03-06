# ChessBot using AI Algorithms

## Introduction

ChessBot is a game engine which has the capability of playing chess with a human and with another game engine. This game engine uses various algorithms like Alpha-Beta Pruning, PositionKey Algorithm, Best First Search etc. and data structures like BitBoards and MoveTrees.

### Architecture
- #### DESIGN AND INITIALIZATION OF THE BOARD
  The most important phase for implementing a game engine is to design the structure of the game and list all the possible rules or configurations required before actually implementing the AI engine logic. So, in this module, definition the board structure for the game (chess) and design a platform for performing moves, storing each board configuration on performing a single move and then generation of new moves based on existing board configuration.
  For making the generation of moves simpler, a 120 based linear array is chosen.
  This array is displayed in the form of two-dimensional matrix in figure as shown
  
  (a) With off board squares &nbsp;&nbsp; &nbsp; &nbsp; &nbsp; 
  &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;
  &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;
  &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;
  (b) Without off board squares
  
  ![image](https://github.com/SIRIUScoder1/ChessBot/blob/master/images/chessBot1.png)
  
  Figure: 120 Board based representation
  
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
  
- #### GENERATION OF POSITION KEY
    During the progress of the game, each board configuration is stored in a unique poskey (Position Hash key). This position keys along with the move performed from which square to which square is stored in a simple list. This list has allowed storing all the performed moves in the game and the option to UNDO or REDO a move easily. Also, it also helps to analyze the game playing style of the player. Board configuration is also stored in another form, called Forsyth-Edward Notation(FEN). This notation helps in describing a particular board position of a chess game. This provides all the necessary information to restart a game from a particular position. It basically defines a particular game position uses only the ASCII character set. 
 
  The FEN for the starting position is given as:
  rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1

  After board design structuring, the other important part of game engine is to define various attack move rules for each piece. For defining the attacks, functions MakeMove function, SquareAttacked function etc. are used. MakeMove function, basically define movement for each type of piece present on the chess board and the position for each chess piece is taken from the piece Lists. 
All possible moves are generated to every possible square it can go on the board for each type of piece. Whenever a certain move is performed, SquareAttacked function will take the from square from which the move was performed and also take in the board configuration. The function will find out which square is attacked, the from square, the to square, the score, the type of move, and piece captured will be stored in a 32-bit integer. After all this, the next most important task which is covered in next module is the Perft Testing, which tests the possible moves on a specific layer.

### Implementation
- #### COMPUTATION OF ALL POSSIBLE MOVES
  Before starting implementing moveGeneration function, the first judgment which is tobe made is whether a particular square is attacked or not. So IsSquareAttacked actually takes side, the square which is to be checked and board configuration as argument. It checks for all the nearby pieces squares which can attack that particular square. It will loop through every opposite color piece on the board and check whether that piece is attacking the square or not. In the move generation function the basic logic which is followed is

  MoveGen(board, moveList)
    Loop all pieces
     - Slider pieces loop through each direction and add move AddMove to list->moves[list->count] = move;list->count++;
        Non-Slider pieces loop through each move style and add move AddMove to list->moves[list->count] = move;list->count++
     - White pawns move
      AddMove to list->moves[list->count] = move;list->count++;
     - Black pawns move
      AddMove to list->moves[list->count] = move;list->count++;

    So, in move generation, all possible moves are generated by looping through all the positions and these moves are then put in the movelist. Another important aspect of move generation is whether to make a move or not or take back a move. There is an important aspect which influence the makeMove function and that is the castlePer-mission. So, the castlePermissions are always taken care of and based on which the castling moved should be performed or not is judged. MakeMove function is divided into couple of small functions namely, ClearPiece function, AddMovePiece function. If there is a capture of certain piece then it will take that piece, remove that piece from the board and set that square as EMPTY and also remove it from the position key, material score and piece lists etc.
    During move generation, for checking to what depth how many legal moves are there a testing should be done which is called Perft Testing. Perft Test is basically a debugging function to walk the move generation tree of strictly legal moves to count all the leaf nodes of a certain depth, which can be compared to predetermined values and used to isolate bugs. In perft, nodes are only counted at the end after the last make move. Thus "higher" terminal nodes (e.g. mate or stalemate) are not counted, instead the number of move paths of a certain depth. Perft ignores draws by repetition, by the fifty-move rule and by insufficient material. By recording the amount of time consumed in every iteration, it's possible to compare the performance of different move generators or the same generator on different machines, though this must be done with caution since there are variations to perft.
  
- #### SELECTING BEST POSSIBLE MOVE
  Once the moves have been evaluated depth wise then it is the computation of the best move. The best move is evaluated based on the score the particular move can get. For that all possible moves are generated and the alpha-beta pruning on the moves is done. It is an improvement in the minimax search algorithm which eliminates searching of large parts of the game tree by applying a branch and bound technique. In this one can find a better move out of all the possible moves. If one has found a good move and then alternatives are then discarded. The algorithm has two variables which are being monitored constantly, alpha and beta, representing the minimum score that maximizing player is possibly attain and the maximum score that the minimizing player can attain. In the following example, if it is white's turn and searching is done till depth 2, then all of the white's moves, and all black's responses to these moves. The first move is picked of white's possible moves and let it call possible Move1 and then consider the move and every possible move result for Move1. When considering the first possible counter-move by black, it is discovered that playing this result in Rook won by black. This situation can be safely ignored by all the other black's possible responses to possible move2 because already possible move 1 is better, it does not matter how worse the possible move 2. Another possible response wins a queen, but it does not matter because it can achieve at least an even game by playing possible move1.
  ![image](https://github.com/SIRIUScoder1/ChessBot/blob/master/images/chessBot2.png)
  Figure: Alpha Beta Pruning
  
  The complication is when a search depth is 3 or greater is performed, because both players can make choices which affect the game tree. Now the maintenance of both lower and upper bound (Alpha and Beta) is required. Maintenance of a lower bound is a must because if a move is very bad then we don't consider it and also the maintenance of upper bound is a major thing, because if move at depth 3 or higher leads to a good position, then other won't allow that, because there was already a better move higher up in game tree that will avoid this situation. One player's lower bound is the other player's upper bound.
  
 - #### HANDLING MATERIAL COUNT
    In game programming especially when it comes to games like chess there is way to store a sequence of moves which the particular game engine considers as the best and plans it to play as the game progresses. This is called the principal variation and moves are stored in a table format known as PV table or principal variation table. In this all the nodes of the game tree are included and as an iterative deepening is done it is very important to have a particular type of move ordering which will play the PV, collected during each iteration. Generally it’s a good practice to print the PV table every time it changes or a new depth is reached because it helps analyzing the game situation and flow during the course of the game.
 
 - #### SELECTION OF MOST OPTIMAL MOVES
    For an alpha-beta to function properly, the best moves are needed to be searched first. This is true for Principal Variation nodes and the probable cut-nodes. The main motive is to be as close to the minimum tree as possible. 
    The Cut-nodes are, the best move is not always the cheapest rejection. When inside the iterative deepening framework, it is tried that the principal variation of the previous one is as leftmost path of the next on, which a triangular PV table applies explicitly. 
    Earlier the alpha best value is reached more the further search becomes less costly because the search window becomes very less. Though the principal variation requires to search better moves thatâAZs why researching is done regularly. Move ordering can be controlled to the certain extent if the draft and the ply from the root are considered. The more closer to the root, less is the horizon effect, to better score evaluation, storing of moves becomes easy and game flow can be analyzed.
    There is another type of move which is called the killer moves. These are quiet moves which are generally played whenever the player is having a beta-cutoff and the sibling node is cut, or any other earlier branch in the tree which has the same ply dis-tance to the root. The main goal is to move immediately direct just after a possible available move from the substitution table and the winning capture moves. This is the simplest and quiet efficient move ordering technique.
    

 ## Screenshots:
  ### Game play logs
  ![image](https://github.com/SIRIUScoder1/ChessBot/blob/master/images/output1.png) 
  ![image](https://github.com/SIRIUScoder1/ChessBot/blob/master/images/output2.png)
  ![image](https://github.com/SIRIUScoder1/ChessBot/blob/master/images/output3.png)
  ![image](https://github.com/SIRIUScoder1/ChessBot/blob/master/images/output4.png)
  ### Chess Engine logs
  ![image](https://github.com/SIRIUScoder1/ChessBot/blob/master/images/output5.png)
  ![image](https://github.com/SIRIUScoder1/ChessBot/blob/master/images/output6.png)
  ![image](https://github.com/SIRIUScoder1/ChessBot/blob/master/images/output7.png)
  ![image](https://github.com/SIRIUScoder1/ChessBot/blob/master/images/output8.png)
  
