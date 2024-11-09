#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "types.h"

#define BASE_POINT -1
#define STARTING_POINT 0
#define HOME_COUNT 56 
#define HOMESTRAIGHT_COUNT 51

// Main function for the game
void game_main() 
{

    srand(time(0));      // Initialize random number generator with the current time

    struct player players[] = {
        { {{0, -1, -1, 0,0,"Y1"}, {1, -1, -1, 0,0,"Y2"}, {2,-1, -1, 0,0,"Y3"}, {3,-1, -1, 0,0,"Y4"}}, "Yellow",0, 0, 0,false,0 },

        { {{0, -1, -1, 0,0,"B1"}, {1, -1, -1, 0,0,"B2"}, {2, -1, -1, 0,0,"B3"}, {3, -1, -1, 0,0,"B4"}}, "Blue",1, 0, 0,false,0 },

        { {{0, -1, -1, 0,0,"R1"}, {1, -1, -1, 0,0,"R2"}, {2, -1, -1, 0,0,"R3"}, {3, -1, -1, 0,0,"R4"}}, "Red",2, 0, 0,false,0  },

        { {{0, -1, -1, 0,0,"G1"}, {1, -1, -1, 0,0,"G2"}, {2, -1, -1, 0,0,"G3"}, {3, -1, -1, 0,0,"G4"}}, "Green",3, 0, 0,false,0 }
        };


        int winners[4]={-1,-1,-1,-1};     // Array to track the winners, initialized to -1

        printf("The yellow player has four (04) pieces named Y1, Y2, Y3 and Y4.\n");
        printf("The blue player has four (04) pieces named B1, B2, B3 and B4.\n");
        printf("The red player has four (04) pieces named R1, R2, R3 and R4.\n");
        printf("The green player has four (04) pieces named G1, G2, G3 and G4.\n\n");

        repeatTheGame(players, winners);     // Start the game
        printTheWinners(players, winners);   // Print the final winners of the game

    }

// Function to simulate dice roll for a given player
int diceRoll(char playerName[10])
    {
           int index=(rand() % 6) + 1;         // Generate random number between 1 and 6
           printf("%s player rolled %d\n", playerName, index);
           return index;

    }

// Function to determine which player starts first    
int printFirstPlayer(struct player players[10])
    {
           int array[4];        // Array to store dice rolls for each player
           int highestRoll = 0;  
           int highestRollIndex = 0;
           int isMoreMax = 0;

    for(int i=0; i<=3; i++)     // Roll dice for each player
    {
            array[i]=diceRoll(players[i].playerName);
     }

        printf("\n");

        highestRoll = array[0];    // Determine the highest roll
        highestRollIndex = 0;

        for(int i=1; i<=3; i++){
            if(highestRoll < array[i])
            {
               highestRoll = array[i];
               highestRollIndex = i;
            }
        }
        
         // Check if more than one player has the highest roll
        for(int i=0; i<=3; i++){
            if(array[i] == highestRoll && highestRollIndex != i){
                isMoreMax=0;
                printf("More than one player got the same highest dice value.\n");
		        printf("Let's roll the dice again.\n\n");
                printFirstPlayer(players);     // Recurse if there's a tie
                isMoreMax=1;
                break;
            }
            isMoreMax=0;
        }

        
        // Announce the player who will start the game
       if (!isMoreMax) {
        printf(">>>>%s player has the highest number and will begin the game.<<<<\n\n", players[highestRollIndex].playerName);

        printf(">>The order of a single round is");

       
        // Loop through the next 4 players starting from maxIndex
        for (int i = 0; i <=3; i++) 
        {
            int currentPlayerIndex = (highestRollIndex + i) % 4;
            if (i < 3) {

                printf(" %s, ", players[currentPlayerIndex].playerName);
                } 
            else 
            {
                printf("and %s.<<\n\n", players[currentPlayerIndex].playerName);
            }
        }
       // Return the index of the player with the highest number
       return highestRollIndex;
       }
    }

//bool isSpecialCell(int location, int *loc_Array, int length) 
bool isSpecialCell(int location, int *special_Cell, int length) 
   {
    int i = 0;
    while (i < length) {
        if (location == special_Cell[i]) {
            return true;
        }
        i++;
    }
    return false;
    }

// Capture another player's piece if landed on the same location
void  capture_Other_Piece(struct player *players, int index, int pieceIndex ){
        int special_Cell[8] = {0, 2, 13, 15, 26, 28, 39, 41};   // Define special cells
        
    
        for(int i=0; i<=3; i++)      // Check all other players' pieces
        {   
            if(index!=i)             // Skip the current player's own pieces
            {           
            for(int j=0; j<=3; j++)
                { 
                    if(players[index].PI[pieceIndex].location==players[i].PI[j].location &&          // Check if the current piece can capture an opponent's piece
                        players[i].PI[j].distance < 52 &&
                        !isSpecialCell(players[index].PI[pieceIndex].location, special_Cell, 8)){

                        printf("%s piece %s lands on square L%d, captures %s piece %s, and returns it to the base.\n", 
                            players[index].playerName,
                            players[index].PI[pieceIndex].pieceName,
                            players[index].PI[pieceIndex].location,
                            players[i].playerName,
                            players[i].PI[j].pieceName);
                        
                        // Update captured piece's state
                        players[i].PI[j].distance=-1;
                        players[i].PI[j].location=-1;
                        players[i].PiecesCount_Board--;
                        
                        // Print updated status of captured player
                        printf("%s player now has %d/4 on pieces on the board and %d/4 pieces on the base.\n", 
                            players[i].playerName,
                            players[i].PiecesCount_Board,
                            4-players[i].PiecesCount_Board);
                        
                        break;
                    }
                } 
            }
        }
    }

// Move the player's piece based on dice value and direction
void moveChooseDirectionPlayer(struct player *players, int diceVal, int index, int *winners)
{
    bool isOnBoard = false;

    // Check if any piece is on the board
    for (int j = 0; j <= 3; j++) {
        if (players[index].PI[j].distance < HOMESTRAIGHT_COUNT && players[index].PI[j].distance > -1) {
            isOnBoard = true;
            break;
        }
    }

    // Attempt to move pieces
    for (int i = 0; i <= 3; i++) {
        if (players[index].PI[i].distance < HOMESTRAIGHT_COUNT && players[index].PI[i].distance != -1 && players[index].PiecesCount_Board > 0) {
        
            // Set direction for the player if not set already
            if (!players[index].directionSet) {
                players[index].direction = rand() % 2; // 0 for clockwise, 1 for counter-clockwise
                players[index].directionSet = true;   // Mark direction as set
            }

            // Assign direction to the current piece
            players[index].PI[i].clockwise = players[index].direction;

            int oldLocation = players[index].PI[i].location;

            // Move piece based on the direction (clockwise)
            if (players[index].PI[i].clockwise == 1)
            {
                // Clockwise movement
                players[index].PI[i].distance += diceVal;
                players[index].PI[i].location = (oldLocation + diceVal) % 52;
            }
            else 
            {
                // Counter-clockwise movement
                players[index].PI[i].distance += diceVal;
                players[index].PI[i].location = (oldLocation - diceVal + 52) % 52;

            }
            

            // Display move information
            printf("%s moves piece %s from location L%d to L%d by %d units in %s direction.\n",
                   players[index].playerName,
                   players[index].PI[i].pieceName,
                   oldLocation,
                   players[index].PI[i].location,
                   diceVal,
                   players[index].PI[i].clockwise ? "clockwise" : "counter-clockwise");

            // Check if a piece has been captured
           capture_Other_Piece(players, index, i);

            return;

        } else if (players[index].PI[i].distance < HOME_COUNT &&
                   players[index].PI[i].distance >= HOMESTRAIGHT_COUNT &&
                   players[index].PI[i].distance != -1 &&
                   !isOnBoard) {
            
            // Check if the piece can move to home
            if (56 - players[index].PI[i].distance == diceVal) {
                players[index].PI[i].location += diceVal;
                players[index].PI[i].distance += diceVal;
                winThePlayer(players, winners, index, i);
            }

            return;
        }
    }
}



void moveFromBaseToStart(struct player *player)
    {
    // Define starting locations for each piece based on player index
    int startingLocations[4][4] = {
        {2, 15, 28, 41},  // Yellow player (index 0)
        {15, 28, 41, 2},  // Blue player (index 1)
        {28, 41, 2, 15},  // Red player (index 2)
        {41, 2, 15, 28}   // Green player (index 3)
    };

    for (int i = 0; i <= 3; i++) {
        if ((*player).PI[i].distance == -1) {
            // Move piece to the starting point
            (*player).PiecesCount_Board++;
            (*player).PI[i].distance = 0;
            (*player).PI[i].location = startingLocations[(*player).index][i];

            // Display move information
            printf("%s player moves piece %s to the starting point.\n", (*player).playerName, (*player).PI[i].pieceName);
            printf("%s player now has %d/4 pieces on the board and %d/4 pieces on the base.\n", 
                   (*player).playerName,
                   (*player).PiecesCount_Board + (*player).winPiecesCount,
                   4 - (*player).PiecesCount_Board + (*player).winPiecesCount);
            break;
        }
    }
}


// Function to handle a player's piece reaching the home 
void winThePlayer(struct player *players, int *winners, int index, int i){
        
        // Check if the piece has reached the home
        if(players[index].PI[i].distance >= HOME_COUNT)
	    {
            players[index].winPiecesCount++;
            players[index].PiecesCount_Board--;
            printf(">> %s piece reached to the Home . <<\n", players[index].PI[i].pieceName);
            
            //Check if all pieces have reached the home
            if(players[index].winPiecesCount>=4)
	        {
                printf(">>>>> %s player's all pieces Reached to the Home!!!. <<<<<\n", players[index].playerName);
                for(int i=0; i<4; i++)

		        {
                    if(winners[i]==-1)
		            {
                        winners[i]=index;
                        break;
                    }
                }
            }
        }

        //Stop iterating after winned 3rd player
        if(winners[2] != -1){
            bool isFourthPlayer=false;
            for(int a=0; a<4; a++){
                for(int b=0; b<3; b++){
                    if(winners[b]==a){
                        isFourthPlayer=true;
                        break;
                    }
                }
                if(!isFourthPlayer){
                    winners[3]=a;
                    break;
                }
                isFourthPlayer=false;
            }
        }
    }

// Function to manage the movement of a player's pieces
void playerMovement(struct player *players, int diceVal, int index, int *winners) {
    struct player *currentPlayer = &players[index];
    
    // Check if a piece can be moved from base to start
    if (diceVal == 6 && 
        (currentPlayer->PiecesCount_Board + currentPlayer->winPiecesCount) < 4) {
        moveFromBaseToStart(currentPlayer);
        return;
    }

    // If the player has pieces on the board, choose a direction to move
    if (currentPlayer->PiecesCount_Board > 0) {
        moveChooseDirectionPlayer(players, diceVal, index, winners);
    }
}

// Main game loop to repeat the game until all players finish
void repeatTheGame(struct player *players, int *winners) 
   {
    
    // Determine the first player to start
    int firstPlayer = printFirstPlayer(players);
    int currentPlayer = firstPlayer;

     while (1) {
        int diceVal;
        int consecutiveSixes = 0;
        while (consecutiveSixes < 3)      // Handle dice rolls for the current player
        {
            if (players[currentPlayer].winPiecesCount < 4) {
                diceVal = diceRoll(players[currentPlayer].playerName);
                playerMovement(players, diceVal, currentPlayer, winners);
                printf("\n");

                if (diceVal != 6)       // If dice is not 6, move to the next player
                {     
                    break;
                }

                consecutiveSixes++;
            } else {
                break;
            }
        }

        // Move to the next player
        currentPlayer = (currentPlayer + 1) % 4;

        // Check if all winners are decided
        if (winners[3] != -1) {
            break;
        }

        // Print piece states after each round
        if (currentPlayer == firstPlayer) {
            displayPlayerPieceStates(players);
        }
    }
    }


// Function to display the current state of each player's pieces
void displayPlayerPieceStates(struct player *players)
{
    for (int playerIndex = 0; playerIndex < 4; playerIndex++)
    {
        int piecesOnBoard = players[playerIndex].PiecesCount_Board + players[playerIndex].winPiecesCount;
        int piecesOnBase = 4 - piecesOnBoard;
        
        // Display number of pieces on the board and in the base for each player
        printf("\n\n%s player has %d/4 pieces on the board and %d/4 pieces in the base.\n",
               players[playerIndex].playerName,
               piecesOnBoard,
               piecesOnBase);

        printf("====================================\n");
        printf("Locations of pieces for %s:\n", players[playerIndex].playerName);
        printf("====================================\n");
        
        // Display location of each piece
        for (int pieceIndex = 0; pieceIndex < 4; pieceIndex++)
        {
            printf("Piece %s -> ", players[playerIndex].PI[pieceIndex].pieceName);

            int distance = players[playerIndex].PI[pieceIndex].distance;

            if (distance == BASE_POINT)          // Determine the location of the piece based on the distance
            {
                printf("Base");
            }
            else if (distance >= HOMESTRAIGHT_COUNT && distance < HOME_COUNT)
            {
                printf("%d cells to reach home.", HOME_COUNT - distance);
            }
            else if (distance >= HOME_COUNT)
            {
                printf("Home");
            }
            else
            {
                printf("Location %d", players[playerIndex].PI[pieceIndex].location);
            }

            printf("\n");
        }
    }
    printf("\n");
}

// Function to print the winners in order
void printTheWinners(struct player *players, int *winners)
    {
        for(int i=0; i<=3; i++)
        {
            printf("%d place >>> %s player <<<", i+1, players[winners[i]].playerName);     // Display the rank and the player's name
            printf("\n");
        }
        printf("\n\n");
        printf("**** %s player wins!!! ****",players[winners[0]].playerName);     // Announce the winner (the first player in the winners array)
    }

