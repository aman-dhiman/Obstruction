#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <time.h> //Needed for random numbers.

#define UP 'i'
#define DOWN 'k'
#define LEFT 'j'
#define RIGHT 'l'
#define HIT ' '
#define QUIT 'q'
#define NEW 'n'
#define BLACK 0 //Colour constants
#define RED 1
#define GREEN 2
#define YELLOW 3
#define BLUE 4
#define MAGENTA 5
#define CYAN 6
#define WHITE 7

void makeGrid();
void drawInterface(char *nameOne, char *nameTwo, int plOneWins, int plTwoWins,
                   int turn);
void makeMove(char direction, int *turn);
void moveAI(int move);
int checkValid();
void makeMark(int move);
int winCheck();
void makeRect(int move);
void clearRect(int move);
void printMes(char *string);
void printWinner(char *name);
void clearMes();
void clearWinner();

int main()
{
    // Name variables for users names.
    char input, nameOne[16], nameTwo[16];
    char nameAI[] = "AI";
    int turn, won, plOneWins, plTwoWins, len1, len2;
    //turn determines whose turn, won determines is someone won,
    // plOneWins and plTwoWins keep track of their respective wins
    // so far.
    plOneWins = 0;
    plTwoWins = 0;
    turn = 0;
    
    srand(time(NULL));
    //Generated for AI moves.
    
    init_pair(1, WHITE, BLACK);
    init_pair(2, YELLOW, BLACK);
    init_pair(3, CYAN, BLACK);
    init_pair(4, MAGENTA, BLACK);
    //Color pairs initialization
    
    // gets player 1 name
    printf("Please enter the name for player 1: ");
    fgets(nameOne, 16, stdin);
    len1 = strlen(nameOne);
    if(nameOne[len1 - 1] == '\n')
    {
        nameOne[len1 - 1] = '\0';
    }
    
    // player 2 name or AI
    printf("Please enter the name for player 2 or type AI to play "
    "against Computer: ");
    fgets(nameTwo, 16, stdin);
    len2 = strlen(nameTwo);
    if(nameTwo[len2 - 1] == '\n')
    {
        nameTwo[len2 - 1] = '\0';
    }
    
    //Ncurses initialization
    initscr();
    start_color();
    cbreak();
    noecho();
    
    //Board initialization
    makeGrid();
    drawInterface(nameOne, nameTwo, 0, 0, 0);
    
    move(1, 2);
    
    //Font customization, used throughout the code
    attroff(A_BOLD);
    attron(COLOR_PAIR(3));
    
    printMes("Welcome to Obstruction Game. Press i, j, k, l to move"
    " and Space to select.");
    refresh();
    
    input = getchar();
    clearMes();
    
    //runs while not quit
    while(input != QUIT)
    {
        //Checks if AI's turn and then makes move
        if((strcmp(nameTwo, nameAI) == 0) && (turn % 2 == 1))
        {
            moveAI(turn);
            turn++;
        }
        else
        {
            makeMove(input, &turn);
        }
        
        //updates the board
        drawInterface(nameOne, nameTwo, plOneWins, plTwoWins, turn);
        clearRect(turn);
        makeRect(turn);
        
        won = winCheck();
        
        //If the game's over and we have a winner
        if(won)
        {
            switch(turn % 2)
            {
                case 0:
                printWinner(nameTwo);
                plTwoWins++;
                break;
                
                case 1:
                printWinner(nameOne);
                plOneWins++;
                break;
            }
            
            drawInterface(nameOne, nameTwo, plOneWins, plTwoWins, turn);
            
            do
            {
                printMes("Please press Space to continue");
                input = getchar();
            }
            while(input != ' ');
            
            clearMes();
            clearWinner();
            
            do
            {
                printMes("Please press n for new game or q to quit");
                input = getchar();
            } 
            while(input != NEW && input != QUIT);
            
            //Initializes new game if user inputs n
            if(input == NEW)
            {
                clear();
                turn = 0;
                makeGrid();
                drawInterface(nameOne, nameTwo, plOneWins, plTwoWins, turn);
                printMes("Welcome to Obstruction Game. Press i, j, k, l "
                "to move and Space to select.");
                move(1, 2);
                refresh();
            }
        }
        
        //Checks if AI's turn, otherwise inputs the character
        if((turn % 2 == 0) ||((strcmp(nameTwo, nameAI) != 0) && 
                             (turn % 2 == 1))) 
        {
            input = getchar();
        }
        
        clearMes();
    }
    
    refresh();
    
    endwin();
    return(0);
}

/*makeGrid maked the 6 x 6 board
pre: nothing
post: prints the grid */
void makeGrid()
{
    init_pair(1, WHITE, BLACK);
    attron(COLOR_PAIR(1) | A_BOLD);
    
    clearMes();
    
    for(int j = 1; j < 31; j++)
    {
        for(int i = 0; i < 50; i = i + 8)
        {
            mvaddch(j, i, 124);
        }
        
        for(int i = 1; i < 50; i = i + 8)
        {
            mvaddch(j, i, 124);
        }
    }
    
    for(int i = 0; i < 50; i++)
    {
        for(int j = 0; j < 31; j = j + 5)
        {
            mvaddch(j, i, 61);
        }
    }
    
    attroff(COLOR_PAIR(1) | A_BOLD);
    
    refresh();
    return;
}

/* drawInterface prints obstruct, player names and their wins.
pre: takes player's names, their wins and the turn as input
post: prints all information nicely. */
void drawInterface(char *nameOne, char *nameTwo, int plOneWins, int plTwoWins, int turn)
{
    int ypos, xpos;
    
    init_pair(2, YELLOW, BLACK);
    init_pair(3, CYAN, BLACK);
    init_pair(4, MAGENTA, BLACK);
    
    //gets current position
    getyx(stdscr, ypos, xpos);
    
    attron(COLOR_PAIR(2));
    attroff(A_BOLD);
    
    // draws OBSTRUCT
    mvwprintw(stdscr, 2, 52, "### ### ### ### ### # # ### ### # ### #  #");
    mvwprintw(stdscr, 3, 52, "# # # # #    #  # # # # #    #  # # # ## #");
    mvwprintw(stdscr, 4, 52, "# # ### ###  #  ### # # #    #  # # # # ##");
    mvwprintw(stdscr, 5, 52, "# # # #   #  #  ##  # # #    #  # # # #  #");
    mvwprintw(stdscr, 6, 52, "### ### ###  #  # # ### ###  #  # ### #  #");
    
    attron(COLOR_PAIR(4));
    
    //Prints player names
    mvwprintw(stdscr, 12, 55, "Player 1:");
    mvwprintw(stdscr, 12, 78, "Player 2:");
    mvwprintw(stdscr, 14, 57, "%s", nameOne);
    mvwprintw(stdscr, 14, 80, "%s", nameTwo);
    
    attron(COLOR_PAIR(3));
    
    //Prints players wins
    mvwprintw(stdscr, 20, 60, "%d", plOneWins);
    mvwprintw(stdscr, 20, 71, ":");
    mvwprintw(stdscr, 20, 82, "%d", plTwoWins);
    
    move(ypos, xpos);
    
    refresh();
}

/* makeMove takes user input and checks if it is a valid move 
and makes the move.
Pre: takes users input and the number of turn
Post: Makes appropriate move */
void makeMove(char direction, int *turn)
{
    int xpos, ypos, move, moveValid;
    move = *turn;
    
    getyx(stdscr, ypos, xpos);
    
    init_pair(1, WHITE, BLACK);
    attron(COLOR_PAIR(1));
    
    //Makes appropriate move for the input
    switch(direction)
    {
        case UP:
        ypos = ypos - 5;
        break;
        
        case DOWN:
        ypos = ypos + 5;
        break;
        
        case LEFT:
        xpos = xpos - 8;
        break;
        
        case RIGHT:
        xpos = xpos + 8;
        break;
        
        //makes Mark when hit space
        case HIT:
        moveValid = checkValid();
        if(moveValid)
        {
            makeMark(move);
            *turn = *turn + 1;
        }
        break;
        
        //Message for inappropriate input
        default:
        printMes("Invalid input. Please Use i, j, k or l to move,"
        " spacebar to mark and q to quit");
    }
    
    //Checks for valid movement and moves
    if(ypos < 31 && xpos < 50)
    {
        move(ypos, xpos);
    }
    else
    {
        printMes("Invalid Move. Out of bounds");
    }
    
    refresh();
}

/* moveAI makes the moves for the AI.
Pre: takes the number of turn as input to pass it to makeMark function
Post: generates random number, checks if move valid and then moves */
void moveAI(int move)
{
    int ypos, xpos;
    
    // Keeps generating input until valid
    do
    {   //random number generated
        ypos = ((rand() % 6) * 5) + 1;
        xpos = ((rand() % 6) * 8) + 2;
    
        move(ypos, xpos);
        checkValid();
    }
    while(checkValid() == 0);
    
    makeMark(move);
    refresh();
}

/* checkValid checks for valid input by searching all nearby
grids and returns true if valid.
Pre: takes the current cursor position
Post: Validates the move and returns true */
int checkValid()
{
    int ypos, xpos;
    
    // Different character variables to check nearby grids 
    char xmymx, omymx, xymx, oymx, xpymx, opymx, xmyx, omyx, xyx, oyx, 
    xpyx, opyx, xmypx, omypx, xypx, oypx, xpypx, opypx; 
    
    getyx (stdscr, ypos, xpos);
    
    // characters initialized and set to nearby grids' characters
    xmymx = mvinch (ypos - 5, xpos - 8);
    move(ypos, xpos);
    xymx = mvinch (ypos, xpos - 8);
    move(ypos, xpos);
    xpymx = mvinch (ypos + 5, xpos - 8);
    move(ypos, xpos);
    xmyx = mvinch (ypos - 5, xpos);
    move(ypos, xpos);
    xyx = mvinch (ypos, xpos);
    move(ypos, xpos);
    xpyx = mvinch (ypos + 5, xpos);
    move(ypos, xpos);
    xmypx = mvinch (ypos - 5, xpos + 8);
    move(ypos, xpos);
    xypx = mvinch (ypos, xpos + 8);
    move(ypos, xpos);
    xpypx = mvinch (ypos + 5, xpos + 8);
    move(ypos, xpos);
    
    ypos = ypos + 1;
    
    omymx = mvinch (ypos - 5, xpos - 8);
    move(ypos, xpos);
    oymx = mvinch (ypos, xpos - 8);
    move(ypos, xpos);
    opymx = mvinch (ypos + 5, xpos - 8);
    move(ypos, xpos);
    omyx = mvinch (ypos - 5, xpos);
    move(ypos, xpos);
    oyx = mvinch (ypos, xpos);
    move(ypos, xpos);
    opyx = mvinch (ypos + 5, xpos);
    move(ypos, xpos);
    omypx = mvinch (ypos - 5, xpos + 8);
    move(ypos, xpos);
    oypx = mvinch (ypos, xpos + 8);
    move(ypos, xpos);
    opypx = mvinch (ypos + 5, xpos + 8);
    move(ypos - 1, xpos);
    
    //If nearby grids have been marked, returns false, otherwise true
    if (xmymx == 'x' || xymx == 'x' || xpymx == 'x' || xmyx == 'x' ||
        xyx == 'x' || xpyx == 'x' || xmypx == 'x' || xypx == 'x' ||
        xpypx == 'x' || omymx == 'o' || oymx == 'o' || opymx == 'o' ||
        omyx == 'o' || oyx == 'o' || opyx == 'o' || omypx == 'o' ||
        oypx == 'o' || opypx == 'o')
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

/* makeMark takes the current position and the number of turn
to determine which player and marks the grid
Pre: takes current cursor position and move
Post: Prints a cross or ring depending on whose turn it is */
void makeMark (int move)
{
    int ypos, xpos;
    
    init_pair(1, WHITE, BLACK);
    init_pair(2, YELLOW, BLACK);
    init_pair(3, CYAN, BLACK);
    
    switch (move % 2)
    {   
        //Player 1's move
        case 0:
        attron(COLOR_PAIR(2) | A_BOLD);
        getyx (stdscr, ypos, xpos);
        for(int j = ypos; j <  ypos + 4; j++)
        {
            if(j == ypos || j == ypos + 3)
            {
                mvaddch(j, xpos, 'x');
                mvaddch(j, xpos + 1, 'x');
                mvaddch(j, xpos + 4, 'x');
                mvaddch(j, xpos + 5, 'x');
            }
            else
            {
                mvaddch(j, xpos + 2, 'x');
                mvaddch(j, xpos + 3, 'x');
            }
        }  
        move(ypos, xpos);
        attron(COLOR_PAIR(1));
        break;
        
        //Player 2's move 
        case 1:
        attron(COLOR_PAIR(3) | A_BOLD);
        getyx (stdscr, ypos, xpos);
        for(int j = ypos; j < ypos + 4; j++)
        {
            if(j == ypos || j == ypos + 3)
            {
                mvaddch(j, xpos + 2, 'o');
                mvaddch(j, xpos + 3, 'o');
            }
            else
            {
                mvaddch(j, xpos, 'o');
                mvaddch(j, xpos + 1, 'o');
                mvaddch(j, xpos + 4, 'o');
                mvaddch(j, xpos + 5, 'o');
            }
        }
        move(ypos, xpos);
        
        attron(COLOR_PAIR(1));
        attroff(A_BOLD);
        
        break;
    }
     
    refresh();
}

/* winCheck check if the game's over and we have a winner
Pre: Checks every grid for availability 
Post: Returns true if no more valid moves available */
int winCheck()
{
    int x, y, win;
    win = 0;
    
    getyx(stdscr, y, x);
    
    //For loops to go to every grid and check
    for(int j = 1; j < 27; j = j + 5)
    {
        for(int i = 2; i < 43; i = i + 8)
        {
            int ypos, xpos;
            ypos = j;
            xpos = i;
            
            char xmymx, omymx, xymx, oymx, xpymx, opymx, xmyx, omyx, 
            xyx, oyx, xpyx, opyx, xmypx, omypx, xypx, oypx, xpypx, opypx; 
    
            xmymx = mvinch (ypos - 5, xpos - 8);
            move(ypos, xpos);
            xymx = mvinch (ypos, xpos - 8);
            move(ypos, xpos);
            xpymx = mvinch (ypos + 5, xpos - 8);
            move(ypos, xpos);
            xmyx = mvinch (ypos - 5, xpos);
            move(ypos, xpos);
            xyx = mvinch (ypos, xpos);
            move(ypos, xpos);
            xpyx = mvinch (ypos + 5, xpos);
            move(ypos, xpos);
            xmypx = mvinch (ypos - 5, xpos + 8);
            move(ypos, xpos);
            xypx = mvinch (ypos, xpos + 8);
            move(ypos, xpos);
            xpypx = mvinch (ypos + 5, xpos + 8);
            move(ypos, xpos);
    
            ypos = ypos + 1;
    
            omymx = mvinch (ypos - 5, xpos - 8);
            move(ypos, xpos);
            oymx = mvinch (ypos, xpos - 8);
            move(ypos, xpos);
            opymx = mvinch (ypos + 5, xpos - 8);
            move(ypos, xpos);
            omyx = mvinch (ypos - 5, xpos);
            move(ypos, xpos);
            oyx = mvinch (ypos, xpos);
            move(ypos, xpos);
            opyx = mvinch (ypos + 5, xpos);
            move(ypos, xpos);
            omypx = mvinch (ypos - 5, xpos + 8);
            move(ypos, xpos);
            oypx = mvinch (ypos, xpos + 8);
            move(ypos, xpos);
            opypx = mvinch (ypos + 5, xpos + 8);
            
            move(y, x);
            
            // Keep adding to win integer for every non-available grid
            if (xmymx == 'x' || xymx == 'x' || xpymx == 'x' || xmyx == 'x' ||
            xyx == 'x' || xpyx == 'x' || xmypx == 'x' || xypx == 'x' ||
            xpypx == 'x' || omymx == 'o' || oymx == 'o' || opymx == 'o' ||
            omyx == 'o' || oyx == 'o' || opyx == 'o' || omypx == 'o' ||
            oypx == 'o' || opypx == 'o')
            {
                win = win + 1;
            }
        }
    }
    
    // If all grids unavailable, win turns 36
    if(win == 36)
    {
        return 1;
    }
    else 
    {
        return 0;
    }
}

/* makeRect creates a rectangle around the player name whohas the next move
Pre: takes number of move into account to determine whose turn it is
Post: Draws rectangle around appropriate player's name */
void makeRect(int move)
{
    int ypos, xpos, turn;
    getyx(stdscr, ypos, xpos);
    turn = move % 2;
    
    switch(turn)
    {
        // Player 1's rectangle
        case 0:
        
        init_pair(2, YELLOW, BLACK);
        attron(COLOR_PAIR(2));
        
        for(int j = 10; j < 17; j++)
        {
            for(int i = 53; i < 66; i++)
            {
                if(j == 10 || j == 16)
                {
                    mvaddch(j, i, 'x');
                }
                else
                {
                    mvaddch(j, 53, 'x');
                    mvaddch(j, 65, 'x');
                }
            }
        }
        attroff(COLOR_PAIR(2));
        break;
        
        // Player 2's rect.
        case 1:
        
        init_pair(3, CYAN, BLACK);
        attron(COLOR_PAIR(3));
        
        for(int j = 10; j < 17; j++)
        {
            for(int i = 76; i < 89; i++)
            {
                if(j == 10 || j == 16)
                {
                    mvaddch(j, i, 'o');
                }
                else
                {
                    mvaddch(j, 76, 'o');
                    mvaddch(j, 88, 'o');
                }
            }
        }
        attroff(COLOR_PAIR(3));
        break;
    }
    
    move(ypos, xpos);
    refresh();
}

/* Does exactly opposite of drawRect. It clears the rectangle drawRect
made.
Pre: takes move number into account
Post: Clears the appropriate rectangle */
void clearRect(int move)
{
    int ypos, xpos, turn;
    getyx(stdscr, ypos, xpos);
    turn = move % 2;
    
    switch(turn)
    {
        case 0:
        for(int j = 10; j < 17; j++)
        {
            for(int i = 76; i < 89; i++)
            {
                if(j == 10 || j == 16)
                {
                    mvaddch(j, i, ' ');
                }
                else
                {
                    mvaddch(j, 76, ' ');
                    mvaddch(j, 88, ' ');
                }
            }
        }
        break;
        
        case 1:
        for(int j = 10; j < 17; j++)
        {
            for(int i = 53; i < 66; i++)
            {
                if(j == 10 || j == 16)
                {
                    mvaddch(j, i, ' ');
                }
                else
                {
                    mvaddch(j, 53, ' ');
                    mvaddch(j, 65, ' ');
                }
            }
        }
        break;
    }
    
    move(ypos, xpos);
    refresh();
}

/* printMes print appropriate message when needed.
Pre: takes a string into account
Post: prints appropriate message and instructions. */
void printMes(char *string)
{
    int xpos, ypos;
    getyx(stdscr, ypos, xpos);
    
    attroff(A_BOLD);
    
    clearMes();
    mvwprintw(stdscr, 34, 0, string);
    
    move(ypos, xpos);
    refresh();
}

/* printWinner prints the "You won the game message" with player's
name.
Pre: Takes player's name
Post: Prints winning message with the name. */
void printWinner (char *name)
{
    int xpos, ypos;
    getyx (stdscr, ypos, xpos);
    
    init_pair(3, CYAN, BLACK);
    attron(COLOR_PAIR(3));
    attroff(A_BOLD);
    
    clearWinner();
    mvwprintw(stdscr, 33, 0,"%s won the game", name);
    
    move(ypos, xpos);
    attroff(COLOR_PAIR(3));
    refresh();
}

/* clearMes clears the message printMes printed.
Pre: current cursor position
Post: clears the message printed by printMes() */ 
void clearMes()
{
    int xpos, ypos;
    getyx(stdscr, ypos, xpos);
    
    mvwprintw(stdscr, 34, 0, "                                            "
    "                                                  ");
    
    move(ypos, xpos);
    refresh();
}

/* clearWinner clears the winning message printed by 
printWinner.
Pre: Current cursor position
Post: clears the printWinner message. */
void clearWinner()
{
    int ypos, xpos;
    getyx(stdscr, ypos, xpos);
    
    mvwprintw(stdscr, 33, 0, "                                           "
    "                                                 ");
    
    move(ypos, xpos);
    refresh();
}