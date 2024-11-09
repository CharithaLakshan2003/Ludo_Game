#ifndef TYPES_H
#define TYPES_H


struct piece
{
    int index;                // Unique index of the piece
    int location;             // Current location of the piece on the board
    int distance;             // Distance moved by the piece
    int capureCount;          // Number of times the piece has captured another piece
    int clockwise;            // Direction of movement (1 for clockwise, 0 for counterclockwise)
    char pieceName[3];        // Name of the piece (e.g., "Y1", "Y2")
};

struct player 
{

    struct piece PI[4];       // Array of 4 pieces belonging to the player                                     
    char playerName[7];       // Name of the player (e.g -: Yellow)
    int index;                // Unique index of the player
    int PiecesCount_Board;    // Number of pieces currently on the board
    int winPiecesCount;       // Number of pieces that have reached the winning position
    bool directionSet;        // Flag to check if the direction is set for the player
    int direction;            // Direction of movement (1 for clockwise, 0 for counterclockwise)
   
};



int diceRoll(char playerName[10]);
int printFirstPlayer(struct player *players);
void capture_Other_Piece(struct player *players, int index, int pieceId );
void moveChooseDirectionPlayer(struct player *players, int diceVal, int index, int  *winners);
void moveFromBaseToStart(struct player *player);
void winThePlayer(struct player *players, int *winners, int index, int i);
void playerMovement(struct player *players, int diceVal, int index, int *winners);
void repeatTheGame(struct player *players, int *winners);
void displayPlayerPieceStates(struct player *players); 
void printTheWinners(struct player *players, int *winners);
void game_main();

#endif
