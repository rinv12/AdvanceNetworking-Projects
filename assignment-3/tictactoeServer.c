/**********************************************************/
/* This program is a 'pass and play' version of tictactoe */
/* Two users, player 1 and player 2, pass the game back   */
/* and forth, on a single computer                        */
/**********************************************************/

/* include files go here */
#include <stdio.h>
#include <strings.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

/* #define section, for now we will define the number of rows and columns */
#define ROWS  3
#define COLUMNS  3


/* C language requires that you predefine all the routines you are writing */
int checkwin(char board[ROWS][COLUMNS]);
void print_board(char board[ROWS][COLUMNS]);
int tictactoe();
int initSharedState(char board[ROWS][COLUMNS]);

int main(int argc, char *argv[])
{
    int sd, rco;             //socket description
    int connected_sd;   //socket description
    struct sockaddr_in server_address;
    struct sockaddr_in from_address;
    char dest[1000];
    char buffer[259];
    int flags = 0;
    socklen_t fromLength;

    sd = socket (AF_INET, SOCK_STREAM, 0);  //for stream


    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(atoi(argv[1]));
    server_address.sin_addr.s_addr = INADDR_ANY;

  if(argc < 2 || argc > 2){
    printf("Invalid arguments. Start with: ./tictactoe <port>\n");
    exit(1);
  }


  // Setting socket bind and initializing variables
  if(bind(sd, (struct sockaddr *)&server_address, sizeof(server_address)) < 0){
    printf("Binding failed.");
    exit(1);
  }
  // Wait and listen for new connections. If listen returns -1, an error occured
  if(listen(sd, 5) < 0){
    printf("Listening failed");
    exit(1);
  }
  // Test an incoming connection, if accept returns -1, an error occured
  connected_sd = accept(sd, (struct sockaddr *) &from_address, &fromLength);
  if(connected_sd < 0){
    printf("Accept error");
    exit(1);
  }
  printf("Connected sd: %i\n", connected_sd);
  // If an incoming connection was received and no error occured, continue with execution.
  int rc;
  char board[ROWS][COLUMNS]; 
  // printf("made connection on: %i\n", connected_sd);
  printf("Client connected. Starting game...\n");
  rc = initSharedState(board); // Initialize the 'game' board
  rc = tictactoe(board, connected_sd); // call the 'game' 
  return 0; 
}


int tictactoe(char board[ROWS][COLUMNS], int sockd)
{
  // After a move, the client needs to update the server, or the server needs to update the client. Basically, send the bytes of the board
  // over the network so that they can be read?


  /* this is the meat of the game, you'll look here for how to change it up */
  int player = 1; // keep track of whose turn it is
  int i, choice, rc, rco;  // used for keeping track of choice user makes
  int row, column;
  char recvd[10];
  char tosend, torecv;
  char mark;      // either an 'x' or an 'o'
  /* loop, first print the board, then ask player 'n' to make a move */

  do{
    if(sockd < 0){
      printf("Connection terminated...");
    }
    player = (player % 2) ? 1 : 2;  // Mod math to figure out who the player is
    print_board(board); // call function to print the board on the screen
    if(player == 1){
      printf("Player %d, enter a number:  ", player); // print out player so you can pass game
      memset(&tosend, 0, sizeof(tosend));
      memset(&choice, 0, sizeof(choice));
      scanf("%d", &choice); //using scanf to get the choice
      //printf("%s\n", &tosend);

      // This is sending the choice, but I should also send the player?
      tosend = (char)choice+'0';
      rc = write(sockd, &tosend, sizeof(tosend));
      printf("%i", rc);
    }
    else{
        rc = read(sockd, &torecv, sizeof(torecv));
        if (!rc){
          printf("Connection terminated...\n");
          exit(1);
        }
        choice = (int)torecv;
        choice = choice-48;
        printf("Received input: %i", choice);
    }
    mark = (player == 1) ? 'X' : 'O'; //depending on who the player is, either us x or o
    /******************************************************************/
    /** little math here. you know the squares are numbered 1-9, but  */
    /* the program is using 3 rows and 3 columns. We have to do some  */
    /* simple math to conver a 1-9 to the right row/column            */
    /******************************************************************/
    row = (int)((choice-1) / ROWS); 
    column = (choice-1) % COLUMNS;

    /* first check to see if the row/column chosen is has a digit in it, if it */
    /* square 8 has and '8' then it is a valid choice                          */

    if (board[row][column] == (choice+'0'))
      board[row][column] = mark;
    else
      {
	printf("Invalid move ");
	player--;
	getchar();
      }
    /* after a move, check to see if someone won! (or if there is a draw */
    i = checkwin(board);

    
    player++;
  }while (i ==  - 1); // -1 means no one won
    
  /* print out the board again */
  print_board(board);
    
  if (i == 1) // means a player won!! congratulate them
    printf("==>\aPlayer %d wins\n ", --player);
  else
    printf("==>\aGame draw"); // ran out of squares, it is a draw
  
  return 0;
}


int checkwin(char board[ROWS][COLUMNS])
{
  /************************************************************************/
  /* brute force check to see if someone won, or if there is a draw       */
  /* return a 0 if the game is 'over' and return -1 if game should go on  */
  /************************************************************************/
  if (board[0][0] == board[0][1] && board[0][1] == board[0][2] ) // row matches
    return 1;
        
  else if (board[1][0] == board[1][1] && board[1][1] == board[1][2] ) // row matches
    return 1;
        
  else if (board[2][0] == board[2][1] && board[2][1] == board[2][2] ) // row matches
    return 1;
        
  else if (board[0][0] == board[1][0] && board[1][0] == board[2][0] ) // column
    return 1;
        
  else if (board[0][1] == board[1][1] && board[1][1] == board[2][1] ) // column
    return 1;
        
  else if (board[0][2] == board[1][2] && board[1][2] == board[2][2] ) // column
    return 1;
        
  else if (board[0][0] == board[1][1] && board[1][1] == board[2][2] ) // diagonal
    return 1;
        
  else if (board[2][0] == board[1][1] && board[1][1] == board[0][2] ) // diagonal
    return 1;
        
  else if (board[0][0] != '1' && board[0][1] != '2' && board[0][2] != '3' &&
	   board[1][0] != '4' && board[1][1] != '5' && board[1][2] != '6' && 
	   board[2][0] != '7' && board[2][1] != '8' && board[2][2] != '9')

    return 0; // Return of 0 means game over
  else
    return  - 1; // return of -1 means keep playing
}


void print_board(char board[ROWS][COLUMNS])
{
  /*****************************************************************/
  /* brute force print out the board and all the squares/values    */
  /*****************************************************************/

  printf("\n\n\n\tCurrent TicTacToe Game\n\n");

  printf("Player 1 (X)  -  Player 2 (O)\n\n\n");


  printf("     |     |     \n");
  printf("  %c  |  %c  |  %c \n", board[0][0], board[0][1], board[0][2]);

  printf("_____|_____|_____\n");
  printf("     |     |     \n");

  printf("  %c  |  %c  |  %c \n", board[1][0], board[1][1], board[1][2]);

  printf("_____|_____|_____\n");
  printf("     |     |     \n");

  printf("  %c  |  %c  |  %c \n", board[2][0], board[2][1], board[2][2]);

  printf("     |     |     \n\n");
}



int initSharedState(char board[ROWS][COLUMNS]){    
  /* this just initializing the shared state aka the board */


  int i, j, count = 1;
  printf ("in sharedstate area\n");
  for (i=0;i<3;i++)
    for (j=0;j<3;j++){
      board[i][j] = count + '0';
      count++;
    }


  return 0;

}
