// c++ final project.cpp : This file contains the 'main' function. Program execution begins and ends there.
//@auhtors : Noah , Garrett and Andreh.

#include <iostream>
//adds format manipulators
#include <iomanip>
//includes to_string()
#include <string>
//allows for transforming strings to uppercase easily
#include <algorithm>
//allows input file
#include <fstream>
//allows us to break up
#include <sstream>
//rng mechanics
#include <cstdlib>
//for true rng
#include <time.h> 

using namespace std;

// Struct to hold player into
struct player_type
{
    string player_name;
    int player_age;
    int player_weight;
};

//functions
int playerTurn(); // Gets players inputs sends to splitPlayerMove, then checks if it is an available space and will place if possible. Aswell as checking for commands "help, rules, or print" - Noah, Andreh
void splitPlayerMove(string playerMove); // Splits players input into variables that we can deal with and use with our backend much easier. - Noah
void cpuTurn(); // Randomly choose place on board, if available it will place, if not it will try again - Noah
void PrintBoard(); // Prints the board and player info - Garrett
void PrintBoardtoFile(); //Prints the board and player info to file - Garrett and Andreh
int checkForWin(); // Checks to see if someone has one - Garrett
int checkForTie(); // Checks to see if its been a tie - Garrett
player_type prompt_player_get_player_info(); // Prompts the player to get their info to populate the struct - Garrett

//global variables
char turnRow, turnColumn;
const int rows = 3;
const int columns = 3;
int cputurnstatus, playerturnstatus;

//main board array
string MainBoard[rows][columns] =
{ {"*", "*", "*"},
 {"*", "*", "*"},
 {"*", "*", "*"} };

player_type user_player; // struct for user player info

// Contributions to main file - Noah Garrett Andreh
int main()
{
    //randomly 
    srand((unsigned)time(NULL));

    //stream
    char char1 = '*';
    string title = "ITCS 2530 FINAL PROJECT - TIC TAC TOE";
    double linelength = 55;

    //line one (stream manipulator dependant)
    cout << setfill(char1) << left << setw(linelength);
    cout << char1 << '\n';
    //line two
    cout << setfill(char1) << right << setw(linelength - 8) << title;
    cout << char1 << char1 << char1 << char1 << char1 << char1 << char1 << char1;
    //line three (stream manipulator dependant)
    cout << setfill(char1) << left << setw(linelength) << endl;
    cout << char1 << '\n' << '\n';

    user_player = prompt_player_get_player_info(); // prompting user and retrieving info

    //srand(time(NULL));
    int rng1 = rand() % 2;

    //outputs whether player or CPU goes first
    if (rng1 == 0)
        cout << "A random number generator has decided the player will go first!" << endl;
    else if (rng1 == 1)
        cout << "A random number generator has decided the CPU will go first!" << endl;

    while (true)
    {
        if (true) //FULL GAME TURN 
        {
            //if statement checks for rng deciding wether cpu or player
            if (rng1 == 0)
            {
                playerTurn();
                rng1 = 1;
            }
            else if (rng1 == 1)
            {
                cout << "CPU turn:\n";
                cpuTurn();
                rng1 = 0;
            }
            PrintBoard();
        }
        if ((checkForWin() == 1) || (checkForTie() == 1)) // to print the game to a status file when its over
        {
            PrintBoardtoFile(); // to print the game to a status file when its over
            exit(1);
        }
    }
    return 0;
}

// Checks to see if it is a tie game - Garrett
int checkForTie()
{
    int tie_flag = 1; // inits a tie game flag

    // Only change the flag if one of the spaces arent filled
    for (int x = 0; x < rows; x++)
    {
        for (int y = 0; y < columns; y++)
        {
            if (MainBoard[x][y] == "*") // if any of the spaces arent filled, it cannot be a tie yet - the game isnt over yet
            {
                tie_flag = 0;
            }
        }
    }

    if (tie_flag == 1)
    {
        cout << "TIE GAME!\n";
    }
    return tie_flag;
}

// Prompts the player and gets their info - error checks player age - Garrett
player_type prompt_player_get_player_info()
{
    player_type temp_holder;

    cout << "Hello and welcome to a game of tic tac toe!\n";

    cout << "Player, what is your first name? ";
    cin >> temp_holder.player_name;

    do // Continue to prompt player if their info is invalid
    {
        cout << "What is your age (1-110)?\n";
        cin >> temp_holder.player_age;
    } while ((temp_holder.player_age <= 0) || (temp_holder.player_age >= 110));

    cout << "Great, thank you!\n\n";
    return temp_holder; // Returns data as a struct
}

// Gets players inputs sends to splitPlayerMove, then checks if it is an available space and will place if possible. Aswell as checking for commands "help, rules, or print" - Noah, Andreh
int playerTurn()
{
    playerturnstatus = 0;
    while (playerturnstatus == 0)
    {
        string playerMove;
        cout << "\ntype \"help\" for help on how to win. "; //asks the user if they want help
        cout << "\ntype \"rules\" for the rules of tic tac toe."; //asks the user if they want know the rules
        cout << "\ntype \"print\" for printout of the most recent status of the game.\n"; //asks the user if they want to print the status of the game
        cout << "\nWhere do you want to place your X? please choose a number 1-3 and letter A-C (type in the format of '1A'): ";
        cin >> playerMove;


        if (playerMove == "help") // if statement to display how to win a tic tac toe game / help
        {

            ifstream in;
            in.open("help.txt");

            cout << in.rdbuf();

            in.close();
            turnRow = 'e';
        }

        if (playerMove == "rules") // if statement to display the rules of the game
        {
            ifstream in;
            in.open("rules.txt");
            cout << in.rdbuf();
            in.close();
            turnRow = 'e';


        }

        if (playerMove == "print")  //if statement to print the status of the game to file
        {
            PrintBoardtoFile();
            PrintBoard(); // also prints board to screen
            turnRow = 'e';

        }

        playerturnstatus = 0;
        splitPlayerMove(playerMove);

        //checks for status from splitPlayerMove, then will apply players move onto the board, making sure it is a valid move
        if (turnRow != 'e' && turnColumn != 'e') 
        {
            //converts from string to int to make it easier to apply to board
            int tC;
            if (turnColumn == 'a' || turnColumn == 'A')
                tC = 0;
            else if (turnColumn == 'b' || turnColumn == 'B')
                tC = 1;
            else if (turnColumn == 'c' || turnColumn == 'C')
                tC = 2;

            //converts from string to int to make it easier to apply to board
            int tR = turnRow - '0';
            tR = tR - 1;

            //applies turn to board here
            if (MainBoard[tR][tC] == "*")
            {
                MainBoard[tR][tC] = "X";
                playerturnstatus = 1;
            }
            else if (MainBoard[tR][tC] == "X" || MainBoard[tR][tC] == "O")
            {
                cout << "Invalid move!" << endl;
                playerturnstatus = 0;
            }
        }
    }
    return 0;
}

// Splits players input into variables that we can deal with and use with our backend much easier. - Noah
void splitPlayerMove(string playerMove) 
{
    
    if (playerMove[0] == '1' || playerMove[0] == '2' || playerMove[0] == '3') //does lots of checks on playermoves input, allows for syntax of either for example 1a or 1,a
    {

        if (playerMove[1] == ',')
        {
            if (playerMove[2] == 'a' || playerMove[2] == 'A' || playerMove[2] == 'b' || playerMove[2] == 'B' || playerMove[2] == 'c' || playerMove[2] == 'C')
            {
                turnColumn = playerMove[2];
            }
            else
            {
                cout << "Please re-read instructions on input!" << endl;

                turnRow = 'e';
                turnColumn = 'e';
            }
        }
        else if (playerMove[1] == 'a' || playerMove[1] == 'A' || playerMove[1] == 'b' || playerMove[1] == 'B' || playerMove[1] == 'c' || playerMove[1] == 'C')
        {
            turnColumn = playerMove[1];
        }
        else
        {
            cout << "Please re-read instructions on input!" << endl;
            turnRow = 'e';
            turnColumn = 'e';
        }
        turnRow = playerMove[0];
    }
    else if (playerMove != "help" && playerMove != "rules" && playerMove != "print") // if the user enters any of the commands the warning below will cancel
    {
        cout << "Please re-read instructions on input!" << endl;
        turnRow = 'e';
        turnColumn = 'e';
    }
    else if (playerMove == "print") // prompts the user when asked to print status
        cout << "\nprint successful. please check gameStatus.txt file as well" << endl;
}

// Prints board to the console with a nested loop - Garrett
void PrintBoard()
{
    cout << endl << endl;
    cout << "                      A    B    C" << endl;

    for (int x = 1; x <= rows; x++)
    {
        cout << "                   " << x << "  ";
        for (int y = 1; y <= columns; y++)
        {
            cout << MainBoard[x - 1][y - 1] + "    ";
        }
        cout << endl;
    }

    cout << "Player name: " << user_player.player_name << endl;
    cout << "Player age: " << user_player.player_age << endl;

    cout << endl << endl;
}

// To print the status of the game to file - Garrett
void PrintBoardtoFile()  
{
    ofstream out;
    out.open("gameStatus.txt");
    out << endl << endl;
    out << "                      A    B    C" << endl;

    for (int x = 1; x <= rows; x++)
    {
        out << "                   " << x << "  ";
        for (int y = 1; y <= columns; y++)
        {
            out << MainBoard[x - 1][y - 1] + "    ";
        }
        out << endl;
    }

    cout << "Player name: " << user_player.player_name << endl;
    cout << "Player age: " << user_player.player_age << endl;

    out << endl << endl;
    out.close();
}

// Randomly choose place on board, if available it will place, if not it will try again - Noah
void cpuTurn()
{
    //local vars
    double rng2, rng3;
    cputurnstatus = 0;
    while (cputurnstatus == 0) //makes sure its cpu turn, then gets random numbers to use as its turn
    {
        rng2 = rand() % 3;
        rng3 = rand() % 3;
        int cpuRow = rng2;
        int cpuColumn = rng3;

        //applies move to board, if allowed ends or allows turn to keep going
        if (MainBoard[cpuRow][cpuColumn] == "*")
        {
            MainBoard[cpuRow][cpuColumn] = "O";
            cputurnstatus = 1;
        }
        else
        {
            cputurnstatus = 0;
        }
    }
}

// Checking each of the possible win combos - Garrett
int checkForWin()
{
    for (int row_idx = 0; row_idx < rows; row_idx++) // checking each row for a win
    {
        // if the row isnt filled w/ *'s & the all the values match
        if ((MainBoard[row_idx][0] != "*") && (MainBoard[row_idx][0] == MainBoard[row_idx][1]) && (MainBoard[row_idx][0] == MainBoard[row_idx][2])) // there is a win in one of the rows
        {
            cout << "GAME OVER!" << endl;
            return 1;
        }
    }

    // Checking each column for a win
    for (int col_idx = 0; col_idx < columns; col_idx++)
    {
        // if col isnt filled w/ all *'s & all values match
        if ((MainBoard[0][col_idx] != "*") && (MainBoard[0][col_idx] == MainBoard[1][col_idx]) && (MainBoard[0][col_idx] == MainBoard[2][col_idx])) // there is a win in 1 of the cols
        {
            cout << "GAME OVER!" << endl;
            return 1;
        }
    }

    // Checking topL to bottomR diagonal
    if ((MainBoard[0][0] != "*") && (MainBoard[0][0] == MainBoard[1][1]) && (MainBoard[0][0] == MainBoard[2][2])) // win in the topL to bottomR diagonal
    {
        cout << "GAME OVER!" << endl;
        return 1;
    }

    // Checking topR to bottomL diagonal
    if ((MainBoard[0][2] != "*") && (MainBoard[0][2] == MainBoard[1][1]) && (MainBoard[0][2] == MainBoard[2][0])) // win in the topR to bottomL diagonal
    {
        cout << "GAME OVER!" << endl;
        return 1;
    }

    return 0; // if made it this far, not a win
}