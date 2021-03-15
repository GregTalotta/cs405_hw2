#include <vector>
using std::vector;
#include <cstdlib>
using std::rand;
#include <ctime>
#include <iostream>
using std::cin;
using std::cout;
using std::endl;
using std::getline;
using std::ostream;
#include <cctype>
using std::toupper;
#include <string>
using std::string;
#include <fstream>
using std::fstream;
#include <chrono>
#include <thread>
#include <memory>
using std::make_shared;
using std::shared_ptr;
using std::make_unique;
using std::unique_ptr;
#include "player.h"
#include "minimaxBot.h"
#include "tree.h"


char getSingleChar();
int getSingleInt(char min, char max);
fstream writeFile();
void printBoard(vector<vector<char>> &playingBoard, ostream &os);
bool win(fstream &save, int player, int bot);
void gameLoop(fstream &save, bool first, bool &bot2);
void startGame();
void readFile();
void playGame();

int _calcBoards = 0;
vector<shared_ptr<Player>> _players;

char getSingleChar()
{
    string input;
    getline(cin, input);
    return toupper(input[0]);
}

int getSingleInt(char min, char max)
{
    string input;
    getline(cin, input);
    char in = input[0];
    if ((char)toupper(in) == 'F')
    {
        return (int)-1;
    }
    if ((in < min) || (in > max))
    {
        cout << "Enter valid input. (" << min << "-" << max << ")" << endl;
        return getSingleInt(min, max);
    }
    return (int)in - 48;
}

void printBoard(vector<vector<char>> &playingBoard, ostream &os)
{
    for (int y = 0; y < 5; ++y)
    {
        if ((y > 0) && (y < 5))
        {
            os << "-----------------" << endl;
        }
        for (int x = 0; x < 5; ++x)
        {
            os << playingBoard[y][x];
            if (x < 4)
            {
                os << " | ";
            }
            else
            {
                os << endl;
            }
        }
    }
    os << "\n\n"
       << endl;
    return;
}

fstream writeFile()
{
    fstream save;
    save.open("save_game.txt", std::ios::out | std::ios::trunc);
    if (!save)
    {
        cout << "File not created!" << endl;
    }
    else
    {
        cout << "File created successfully!" << endl;
        return save;
    }
}

bool win(fstream &save, int player1State, int player2State, bool first)
{
    if (player1State == 1)
    {
        cout << "\n"
             << _players[first]->_name << " Forfits, " << _players[!first]->_name << " wins\n"
             << endl;
        if (save)
        {
            save << _players[first]->_name << " Forfits, " << _players[!first]->_name << " wins" << endl;
        }
        return true;
    }
    if (player2State == 1)
    {
        cout << "\n"
             << _players[!first]->_name << " Forfits, " << _players[first]->_name << " wins\n"
             << endl;
        if (save)
        {
            save << _players[!first]->_name << " Forfits, " << _players[first]->_name << " wins" << endl;
        }
        return true;
    }
    if (player1State == 2)
    {
        cout << _players[first]->_name << " wins" << endl;
        if (save)
        {
            save << _players[first]->_name << " wins" << endl;
        }
        return true;
    }
    if (player2State == 2)
    {
        cout << _players[!first]->_name << " wins" << endl;
        if (save)
        {
            save << _players[!first]->_name << " wins" << endl;
        }
        return true;
    }
    if ((player1State == 3) || (player2State == 3))
    {
        cout << "Draw, no winner" << endl;
        if (save)
        {
            save << "Draw, no winner" << endl;
        }
        return true;
    }
    return false;
}

void gameLoop(fstream &save, bool first)
{
    bool done = false;
    int turn = 0;
    vector<vector<char>> playingBoard = {
        {' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' '},
    };
    printBoard(playingBoard, cout);
    if (save)
        printBoard(playingBoard, save);
    int player1State = 0;
    int player2State = 0;
    while (!done)
    {
        ++turn;
        if ((turn == 14) && (player1State != 2) && (player2State != 2))
        {
            player1State = 3;
            player2State = 3;
        }
        bool won = win(save, player1State, player2State, first);
        if (won)
        {
            return;
        }

        player1State = _players[first]->turn(playingBoard, _players[first]->_piece);
        printBoard(playingBoard, cout);
        if (save)
            printBoard(playingBoard, save);
        if ((player1State == 1) || (turn == 13))
        {
            continue;
        }

        player2State = _players[!first]->turn(playingBoard, _players[!first]->_piece);
        printBoard(playingBoard, cout);
        if (save)
            printBoard(playingBoard, save);
    }
}

void startGame()
{
    fstream savelog;
    char answer;
    bool isBot2 = false;
    cout << "Would you like to save to a file? Note: You can only save one game. (Y/N)" << endl;
    answer = getSingleChar();
    if (answer == 'Y')
    {
        savelog = writeFile();
        savelog << "\n\nSaved game!" << endl;
    }
    vector<vector<shared_ptr<Player>>> options = {
        {makePlayer(), makePlayer()},
        {makePlayer(), makeMinimaxBot()},
        {makeMinimaxBot(), makeMinimaxBot()}};
    for (int i = 0; i < options.size(); ++i)
    {
        cout << i + 1 << ": " << options[i][0]->_name << " vs " << options[i][1]->_name << endl;
    }
    cout << "Enter the number of what you would like to play." << endl;
    int gameType = getSingleInt('1', '3') - 1;
    _players = options[gameType];
    srand(time(NULL));
    bool first = rand() % 2;
    if (!first)
    {
        _players[0]->_name += " 1";
        _players[0]->_piece = 'x';
        _players[1]->_name += " 2";
        _players[1]->_piece = 'o';
        cout << _players[0]->_name << " goes first" << endl;
        if (savelog)
            savelog << _players[0]->_name << " goes first" << endl;
    }
    else
    {
        _players[0]->_name += " 2";
        _players[0]->_piece = 'o';
        _players[1]->_name += " 1";
        _players[1]->_piece = 'x';
        cout << _players[1]->_name << " goes first" << endl;
        if (savelog)
            savelog << _players[1]->_name << " goes first" << endl;
    }
    gameLoop(savelog, first);
    savelog.close();
    return;
}

void readFile()
{
    fstream save;
    save.open("save_game.txt", std::ios::in);
    if (!save)
    {
        cout << "Unable to open file!" << endl;
    }
    else
    {
        cout << "File opened!" << endl;
        string line;
        int i = 0;
        while ((getline(save, line)) && (i < 3))
        {
            cout << line << endl;
            ++i;
        }
        while (getline(save, line))
        {
            cout << line << endl;
            i = 0;
            while ((getline(save, line)) && (i < 11))
            {
                cout << line << endl;
                ++i;
            }
            //needs at least c++ 11
            std::this_thread::sleep_for(std::chrono::milliseconds(1500));
        }
        return;
    }
}

void playGame()
{
    char answer;
    while (true)
    {
        cout << "Would you like to play 5x5, 4 in a row tic tac toe? (Y/N) Or replay the saved match? (R)" << endl;
        answer = getSingleChar();
        if (answer == 'R')
        {
            readFile();
        }
        else if (answer == 'N')
        {
            cout << "have a nice day!" << endl;
            return;
        }
        else if (answer == 'Y')
        {
            startGame();
        }
        else
        {
            cout << "Enter a valid answer" << endl;
        }
    }
}
