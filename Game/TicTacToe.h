//
// Created by phoenixflower on 6/18/20.
//

#ifndef LANDESSDEVCORE_TICTACTOE_H
#define LANDESSDEVCORE_TICTACTOE_H
//
//  TicTacToe.h
//  Foundation
//
//  Created by James Landess on 12/27/13.
//  Copyright (c) 2013 LandessDev. All rights reserved.
//

#ifndef Foundation_TicTacToe_h
#define Foundation_TicTacToe_h
#include <iostream>
#include <string>
#include <stdlib.h>
#include <ctype.h>
#include <limits.h>

/*
namespace AI {
    namespace Games
    {
        namespace TicTacToe
        {
           // using namespace std;

            //FUNCTION PROTOTYPES
            //int menu();						//Handles the game menu.
            void drawBoard(bool update);	//Initializes the game board graphics and if update is true, re-draws the board accordingly.
            void playGame();				//Hanldes versus mode.
            void validateInput();			//Validates userInput.
            void makeMove(bool stalemate);	//Commits players move to gameBoard array.
            void AI();						//This function substitutes for playGame if user selects single player from the menu.
            int AIsmarts(int Player);		//Returns position to take if AI needs to block potential win. Also returns position to take if AI can generate a win.
            bool gameStatus();				//Returns true if a player has won.

            //GLOBAL VARIABLES
            int gameBoard[9]={1,2,3,4,5,6,7,8,9};	//This is the game board array. The drawBoard function will place an X for each 11 and an O for each 22
                                                     //that it sees when its update parameter is set to true.

            int var=0;						//Used for as a count variable in for loops referencing the gameBoard array (ie. gameBoard[var])
            int turn=0;						//Counts each turn up to a total of nine. Used to help determine if a stalemate has taken place.
            int xturn=0;					//Counts what turn X is on. Used as reference for AI in single player mode.
            int option=0;					//Stores the menuInput returnd by the function menu.
            int userInput;					//Stores the requested position from each player both versus and single player modes.
            bool badInput;					//User validation variable.
            bool GameOver=false;			//This is set to true if gameStatus function determines that a player has won.
            string restart = "";			//This variable is used to determine if the loop in main should execute again.(ie. if the user wants to play another game.)
            char player='X';				//Player variable, sets to either X or O and changes after every turn.


            int main(){
                //Show menu and return choice to option
                option = menu();

                //Game loop
                while(option != 3 && restart !="n" && restart !="no"){
                    turn=0;
                    xturn=0;
                    drawBoard(false);
                    if(option == 1){AI();}else{playGame();}
                    if(restart != "n" && restart !="no"){option = menu();}
                }//end while

                return 0;
            }//end main



            int menu(){

                int option = 0;
                bool invalid = false;

                do {
                    //This loop will repeat everytime there is bad input from the user.

                    cout << "\n\n         Menu\n  ___________________\n\n";
                    cout << "   1. Single Player\n\n   2. Versus \n\n   3. Quit\n  ___________________\n\n";
                    cout << "   > ";

                    cin >> menuInput;

                    menuInput = menuInput.c_str();
                    int length = menuInput.length();
                    int x;

                    for (x = 0; x < length; x++){
                        if(!isdigit(menuInput[x])){
                            invalid = true;
                        }
                    }

                    if(!invalid){
                        option = atoi(menuInput.c_str());

                        if(!(option > 0 && option <= 9)) {
                            invalid = true;
                        }
                    }

                }
                while(invalid);


                return option;
            }//end function menu


            void drawBoard(bool update){
                //Clear the Screen


                int column=0;

                for(var=0;var<9;var++){
                    for(column=0;column<3;column++){

                        //If the board needs to be updated.
                        if(update == true){
                            //if the position is X
                            if(gameBoard[var]== 11){cout << "    X  ";}//end if
                                //if the position is O
                            else if(gameBoard[var]== 22){cout << "    O  ";}//end if
                                //if the position is not X or O
                            else{cout << "    " << gameBoard[var]<< "  ";}
                        }

                            //If the board needs to be initialized or re-initialized
                        else{
                            int count;
                            for(count=0;count<9;count++){
                                gameBoard[count]=count+1;//re-initializing gameBoard
                            }//end for
                            cout << "    " << gameBoard[var]<< "  ";
                        }//end else
                        var+=1;
                    }//end for loop

                    //Following two lines are required for proper display of the game board in the terminal.
                    if(var < 8){cout << "\n\n";}
                    var+=-1;
                }//end for loop

                cout << "\n  ___________________\n\n";

            }//end function drawBoard


            void playGame(){
                do{
                    validateInput();
                    makeMove(false);
                }while(turn < 9 && GameOver == false);
                makeMove(true);
            }//end function playGame


            bool gameStatus(){
                int Player;
                if(player=='X'){Player=11;}else{Player=22;}//end if
                //check rows for win
                if(gameBoard[0]==Player && gameBoard[1]==Player && gameBoard[2]==Player){return true;}//end if
                else if(gameBoard[3]==Player && gameBoard[4]==Player && gameBoard[5]==Player){return true;}//end if
                else if(gameBoard[6]==Player && gameBoard[7]==Player && gameBoard[8]==Player){return true;}//end if
                    //check columns for win
                else if(gameBoard[0]==Player && gameBoard[3]==Player && gameBoard[6]==Player){return true;}//end if
                else if(gameBoard[1]==Player && gameBoard[4]==Player && gameBoard[7]==Player){return true;}//end if
                else if(gameBoard[2]==Player && gameBoard[5]==Player && gameBoard[8]==Player){return true;}//end if
                    //check diagonals for win
                else if(gameBoard[0]==Player && gameBoard[4]==Player && gameBoard[8]==Player){return true;}//end if
                else if(gameBoard[2]==Player && gameBoard[4]==Player && gameBoard[6]==Player){return true;}//end if

                return false;

            }//end function gameStatus


            void validateInput(const int input)
            {
                do{

                    if(gameBoard[userInput-1]!=input)
                    {
                        badInput=true;
                    }
                    else{badInput=false;}
                }while(badInput==true);
            }//end function validateInput


            void makeMove(bool stalemate){
                if(stalemate==false){
                    for(var=0;var<9;var++){
                        if(player == 'X'){
                            if(gameBoard[var]== userInput){gameBoard[var]=11;GameOver = gameStatus();player='O';break;}//end if
                        }else{
                            if(gameBoard[var]== userInput){gameBoard[var]=22;GameOver = gameStatus();player='X';break;}//end if
                        }//end else
                    }//end for loop
                    drawBoard(true);
                    turn+=1;
                }//end if
                else{
                    if(GameOver==true){
                        if(player=='O'){cout << "  X wins!\n\n";}else{cout << "  O wins!\n\n";}

                        cout << "  Would you like to play again? y or n: ";
                    }//end if
                    else{cout << "  Stalemate!\n"; cout << "  Would you like to play again? y or n: ";}
                    cin >> restart;
                    system("clear");
                }//end else
            }//end function makeMove


            void AI(){

                do{
                    if(player=='X'){
                        validateInput();
                        xturn+=1;
                    }//end if

                        // AI SECTION //
                    else{
                        userInput=AIsmarts(22);
                        if(userInput==0){
                            userInput=AIsmarts(11);
                        }//end if

                        if(userInput==0){
                            if(xturn == 0){userInput=5;}//if x has not made any moves, take center.
                            else if(xturn == 1){
                                if(gameBoard[0]==11){if(gameBoard[4]==5){userInput=5;}else{userInput=6;}}//end if (if x took top left corner)
                                else if (gameBoard[1]==11){if(gameBoard[4]==5){userInput=5;}else{userInput=9;}}//end else if (if x took top center)
                                else if (gameBoard[2]==11){if(gameBoard[4]==5){userInput=5;}else{userInput=9;}}//end else if (if x took top right corner)
                                else if (gameBoard[3]==11){if(gameBoard[4]==5){userInput=5;}else{userInput=9;}}//end else if (if x took left center)
                                else if (gameBoard[4]==11){userInput=1;}//end else if (if x took center)
                                else if (gameBoard[5]==11){if(gameBoard[4]==5){userInput=5;}else{userInput=1;}}//end else if (if x took right center)
                                else if (gameBoard[6]==11){if(gameBoard[4]==5){userInput=5;}else{userInput=1;}}//end else if (if x took bottom left corner)
                                else if (gameBoard[7]==11){if(gameBoard[4]==5){userInput=5;}else{userInput=1;}}//end else if (if x took bottom center)
                                else if (gameBoard[8]==11){if(gameBoard[4]==5){userInput=5;}else{userInput=3;}}//end else if (if x took bottom right corner)
                            }//end else

                            else if (xturn == 2){
                                userInput = AIsmarts(22);
                                if(userInput==0){
                                    if(gameBoard[0]==11 && gameBoard[8]==11){userInput=6;}//end if (if x took top left corner) and (if x took bottom right corner)
                                    if(gameBoard[0]==11 && gameBoard[5]==11){userInput=3;}//end if (if x took top left corner) and (if x took right center)
                                    if(gameBoard[0]==11 && gameBoard[7]==11){userInput=6;}//end if (if x took top left corner) and (if x took bottom cenetr)

                                    else if (gameBoard[1]==11 && gameBoard[3]==11){userInput=3;}//end else if (if x took top center) and (if x took left center)
                                    else if (gameBoard[1]==11 && gameBoard[6]==11){userInput=9;}//end else if (if x took top center) and (if x took bottom left corner)
                                    else if (gameBoard[1]==11 && gameBoard[7]==11){userInput=3;}//end else if (if x took top center) and (if x took bottom cenetr)
                                    else if (gameBoard[1]==11 && gameBoard[8]==11){userInput=3;}//end else if (if x took top center) and (if x took bottom right corner)
                                    else if (gameBoard[1]==11 && gameBoard[5]==11){userInput=9;}//end else if (if x took top center) and (if x took right center)

                                    else if (gameBoard[2]==11 && gameBoard[3]==11){userInput=1;}//end else if (if x took top right corner) and (if x took left center)
                                    else if (gameBoard[2]==11 && gameBoard[6]==11){userInput=3;}//end else if (if x took top right corner) and (if x took bottom left corner)
                                    else if (gameBoard[2]==11 && gameBoard[7]==11){userInput=1;}//end else if (if x took top right corner) and (if x took bottom center)

                                    else if (gameBoard[3]==11 && gameBoard[1]==11){userInput=3;}//end else if (if x took left center) and (if x took top center)
                                    else if (gameBoard[3]==11 && gameBoard[2]==11){userInput=1;}//end else if (if x took left center) and (if x took top right corner)
                                    else if (gameBoard[3]==11 && gameBoard[5]==11){userInput=7;}//end else if (if x took left center) and (if x took right center)
                                    else if (gameBoard[3]==11 && gameBoard[8]==11){userInput=7;}//end else if (if x took left center) and (if x took bottom right corner)
                                    else if (gameBoard[3]==11 && gameBoard[7]==11){userInput=1;}//end else if (if x took left center) and (if x took bottom center)

                                    else if (gameBoard[4]==11 && gameBoard[8]==11){userInput=3;}//end else if (if x took center and bottom right corner)

                                    else if (gameBoard[5]==11 && gameBoard[0]==11){userInput=8;}//end else if (if x took right center) and (if x took top left corner)
                                    else if (gameBoard[5]==11 && gameBoard[1]==11){userInput=9;}//end else if (if x took right center) and (if x took top center)
                                    else if (gameBoard[5]==11 && gameBoard[3]==11){userInput=2;}//end else if (if x took right center) and (if x took left center)
                                    else if (gameBoard[5]==11 && gameBoard[6]==11){userInput=9;}//end else if (if x took right center) and (if x took bottom left corner)
                                    else if (gameBoard[5]==11 && gameBoard[7]==11){userInput=2;}//end else if (if x took right center) and (if x took bottom center)

                                    else if (gameBoard[6]==11 && gameBoard[1]==11){userInput=6;}//end else if (if x took bottom left corner) and (if x took top center)
                                    else if (gameBoard[6]==11 && gameBoard[2]==11){userInput=5;}//end else if (if x took bottom left corner) and (if x took top right corner)
                                    else if (gameBoard[6]==11 && gameBoard[5]==11){userInput=9;}//end else if (if x took bottom left corner) and (if x took right center)

                                    else if (gameBoard[7]==11 && gameBoard[0]==11){userInput=6;}//end else if (if x took bottom center) and (if x took top left corner)
                                    else if (gameBoard[7]==11 && gameBoard[1]==11){userInput=9;}//end else if (if x took bottom center) and (if x took top center)
                                    else if (gameBoard[7]==11 && gameBoard[2]==11){userInput=1;}//end else if (if x took bottom center) and (if x took top right corner)
                                    else if (gameBoard[7]==11 && gameBoard[3]==11){userInput=1;}//end else if (if x took bottom center) and (if x took left center)
                                    else if (gameBoard[7]==11 && gameBoard[5]==11){userInput=3;}//end else if (if x took bottom center) and (if x took right center)

                                    else if (gameBoard[8]==11 && gameBoard[0]==11){userInput=4;}//end else if (if x took bottom right corner) and (if x took top left corner)
                                    else if (gameBoard[8]==11 && gameBoard[1]==11){userInput=6;}//end else if (if x took bottom right corner) and (if x took top center)
                                    else if (gameBoard[8]==11 && gameBoard[3]==11){userInput=8;}//end else if (if x took bottom right corner) and (if x took left center)
                                }//end if
                            }//end AIsmarts if
                            else if(xturn > 2){//Executed if no obvious/relavant move is available
                                for(var=0;var<9;var++){
                                    if(gameBoard[var]==var+1){userInput=var+1;break;}
                                }
                            }//end else
                        }//end if


                    }//end else
                    makeMove(false);
                }while(turn < 9 && GameOver == false);
                makeMove(true);
            }//end function AI


            int AIsmarts(int Player){
                //check rows
                if(gameBoard[0]==Player && gameBoard[1]==Player && gameBoard[2]==3){return 3;}//end if
                else if(gameBoard[3]==Player && gameBoard[4]==Player && gameBoard[5]==6){return 6;}//end if
                else if(gameBoard[6]==Player && gameBoard[7]==Player && gameBoard[8]==9){return 9;}//end if

                else if(gameBoard[0]==Player && gameBoard[2]==Player && gameBoard[1]==2){return 2;}//end if
                else if(gameBoard[3]==Player && gameBoard[5]==Player && gameBoard[4]==5){return 5;}//end if
                else if(gameBoard[6]==Player && gameBoard[8]==Player && gameBoard[7]==8){return 8;}//end if

                else if(gameBoard[1]==Player && gameBoard[2]==Player && gameBoard[0]==1){return 1;}//end if
                else if(gameBoard[4]==Player && gameBoard[5]==Player && gameBoard[3]==4){return 4;}//end if
                else if(gameBoard[7]==Player && gameBoard[8]==Player && gameBoard[6]==7){return 7;}//end if

                    //check columns
                else if(gameBoard[0]==Player && gameBoard[3]==Player && gameBoard[6]==7){return 7;}//end if
                else if(gameBoard[1]==Player && gameBoard[4]==Player && gameBoard[7]==8){return 8;}//end if
                else if(gameBoard[2]==Player && gameBoard[5]==Player && gameBoard[8]==9){return 9;}//end if

                else if(gameBoard[6]==Player && gameBoard[3]==Player && gameBoard[0]==1){return 1;}//end if
                else if(gameBoard[7]==Player && gameBoard[4]==Player && gameBoard[1]==2){return 2;}//end if
                else if(gameBoard[8]==Player && gameBoard[5]==Player && gameBoard[2]==3){return 3;}//end if

                else if(gameBoard[0]==Player && gameBoard[6]==Player && gameBoard[3]==4){return 4;}//end if
                else if(gameBoard[1]==Player && gameBoard[7]==Player && gameBoard[4]==5){return 5;}//end if
                else if(gameBoard[2]==Player && gameBoard[8]==Player && gameBoard[5]==6){return 6;}//end if

                    //check diagonals
                else if(gameBoard[0]==Player && gameBoard[4]==Player && gameBoard[8]==9){return 9;}//end if
                else if(gameBoard[2]==Player && gameBoard[4]==Player && gameBoard[6]==7){return 7;}//end if

                else if(gameBoard[6]==Player && gameBoard[4]==Player && gameBoard[2]==3){return 3;}//end if
                else if(gameBoard[8]==Player && gameBoard[4]==Player && gameBoard[0]==1){return 1;}//end if

                else if(gameBoard[0]==Player && gameBoard[8]==Player && gameBoard[4]==5){return 5;}//end if
                else if(gameBoard[2]==Player && gameBoard[6]==Player && gameBoard[4]==5){return 5;}//end if

                else{return 0;}

            }//end function AIsmarts
        }
    }
}
/**
 int main(){
 //Show menu and return choice to option
 option = menu();

 //Game loop
 while(option != 3 && restart !="n" && restart !="no"){
 turn=0;
 xturn=0;
 drawBoard(false);
 if(option == 1){AI();}else{playGame();}
 if(restart != "n" && restart !="no"){option = menu();}
 }//end while

 return 0;
 }//end main
 */
#endif

#endif //LANDESSDEVCORE_TICTACTOE_H
