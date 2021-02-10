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
using std::istream;
using std::ostream;
#include <cctype>
using std::toupper;
#include <string>
using std::stoi;
using std::string;
#include <fstream>
using std::fstream;
#include <limits>
#include <chrono>
#include <thread>

char getSingleChar();
int getSingleInt(char min, char max);
int evaluate(vector<vector<char>> &playingBoard, char piece, int x, int y);
fstream writeFile();
bool validMove(vector<vector<char>> &playingBoard, int x, int y);
int checkBoard(vector<vector<char>> &playingBoard, int x, int y, char piece, int max);
void printBoard(vector<vector<char>> &playingBoard, ostream &os);

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

bool validMove(vector<vector<char>> &playingBoard, int x, int y)
{
    return playingBoard[y][x] == ' ';
}

int checkBoard(vector<vector<char>> &playingBoard, int x, int y, char piece, int max)
{
    //tie
    bool end = true;
    for (int i = 0; i < 5; ++i)
    {
        for (int j = 0; j < 5; ++j)
        {
            if (playingBoard[i][j] == ' ')
            {
                end = false;
            }
        }
    }
    if (end)
    {
        return 3;
    }

    //check row
    int count = 0;
    for (int i = 0; i < 5; ++i)
    {
        if (playingBoard[y][i] == piece)
        {
            ++count;
        }
        else
        {
            count = 0;
        }
        if (count == max)
        {
            return 2;
        }
    }
    //check column
    count = 0;
    for (int i = 0; i < 5; ++i)
    {
        if (playingBoard[i][x] == piece)
        {

            ++count;
        }
        else
        {
            count = 0;
        }
        if (count == max)
        {
            return 2;
        }
    }
    //diagnol down
    count = 0;
    int ddy = y;
    int ddx = x;
    while ((ddy > 0) && (ddx > 0))
    {

        --ddy;
        --ddx;
    }
    while ((ddy < 5) && (ddx < 5))
    {

        if (playingBoard[ddy][ddx] == piece)
        {
            ++count;
        }
        else
        {
            count = 0;
        }
        ++ddx;
        ++ddy;
        if (count == max)
        {
            return 2;
        }
    }
    //diagnol up
    count = 0;
    int duy = y;
    int dux = x;
    while ((duy < 4) && (dux > 0))
    {
        ++duy;
        --dux;
    }
    while ((duy >= 0) && (dux < 5))
    {
        if (playingBoard[duy][dux] == piece)
        {
            ++count;
        }
        else
        {
            count = 0;
        }
        ++dux;
        --duy;
        if (count == max)
        {
            return 2;
        }
    }
    return 0;
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

int playerTurn(vector<vector<char>> &playingBoard, char piece, fstream &save)
{
    cout << "Enter the column (1-5): FF to quit" << endl;
    int x = getSingleInt('1', '5') - 1;
    if (x == -2)
    {
        return 1;
    }
    cout << "Enter the row (1-5): FF to quit" << endl;
    int y = getSingleInt('1', '5') - 1;
    if (y == -2)
    {
        return 1;
    }
    if (validMove(playingBoard, x, y))
    {
        playingBoard[y][x] = piece;
        printBoard(playingBoard, cout);
        if (save)
            printBoard(playingBoard, save);
        return checkBoard(playingBoard, x, y, piece, 4);
    }
    else
    {
        cout << "Enter Valid Position" << endl;
        playerTurn(playingBoard, piece, save);
    }
}

int aroundPiece(vector<vector<char>> &playingBoard, int x, int y, char piece)
{
    int countTotal = 0;
    int found = 0;
    for (int ny = 0; ny < 5; ++ny)
    {
        for (int nx = 0; nx < 5; ++nx)
        {
            if (((ny >= y - 1) && (ny <= y + 1)) && ((nx >= x - 1) && (nx <= x + 1)))
            {
                if ((ny != y) && (nx != x))
                {
                    ++countTotal;
                    if (playingBoard[ny][nx] != piece)
                    {
                        continue;
                    }
                    ++found;
                }
            }
        }
    }
    return found;
}

bool boardBlank(vector<vector<char>> &playingBoard)
{
    for (int ny = 0; ny < 5; ++ny)
    {
        for (int nx = 0; nx < 5; ++nx)
        {
            if (playingBoard[ny][nx] != ' ')
            {
                return false;
            }
        }
    }
    return true;
}

char changePiece(char piece)
{
    if (piece == 'x')
    {
        return 'o';
    }
    return 'x';
}

int minimax(vector<vector<char>> &playingBoard, int depth, bool maximizing, char piece, int lastx, int lasty)
{
    int check = checkBoard(playingBoard, lastx, lasty, changePiece(piece), 4);
    if (check == 3)
    {
        return 0;
    }
    else if ((check == 2) && (!maximizing))
    {
        return 10000;
    }
    else if ((check == 2) && (maximizing))
    {
        return -10000;
    }
    if (depth >= 3)
    {
        if (!maximizing)
        {
            return evaluate(playingBoard, changePiece(piece), lastx, lasty);
        }
        else
        {
            return -evaluate(playingBoard, changePiece(piece), lastx, lasty);
        }
    }

    if (maximizing)
    {
        int bestx;
        int besty;
        int bestScore = -10000000;
        for (int y = 0; y < 5; ++y)
        {
            for (int x = 0; x < 5; ++x)
            {
                if (validMove(playingBoard, x, y))
                {
                    playingBoard[y][x] = piece;
                    int score = minimax(playingBoard, depth + 1, false, changePiece(piece), x, y);
                    if (score > bestScore)
                    {
                        bestScore = score;
                        bestx = x;
                        besty = y;
                    }
                    playingBoard[y][x] = ' ';
                }
            }
        }
        return bestScore;
    }
    else
    {
        int bestx;
        int besty;
        int bestScore = 10000000;
        for (int y = 0; y < 5; ++y)
        {
            for (int x = 0; x < 5; ++x)
            {
                if (validMove(playingBoard, x, y))
                {
                    playingBoard[y][x] = piece;
                    int score = minimax(playingBoard, depth + 1, true, changePiece(piece), x, y);

                    if (score < bestScore)
                    {
                        bestScore = score;
                        bestx = x;
                        besty = y;
                    }
                    playingBoard[y][x] = ' ';
                }
            }
        }
        return bestScore;
    }
}

int evaluate(vector<vector<char>> &playingBoard, char piece, int x, int y)
{
    int modifier = 10;
    //check if it makes you win
    if ((y > 0) && (y < 4) && (x > 0) && (x < 4))
    {
        modifier += 10;
    }
    if (checkBoard(playingBoard, x, y, piece, 4) == 2)
    {
        return 900;
    }
    else if (checkBoard(playingBoard, x, y, piece, 3) == 2)
    {
        modifier += 700;
    }
    //check if it stops the other player from winning
    playingBoard[y][x] = changePiece(piece);
    if (checkBoard(playingBoard, x, y, changePiece(piece), 4) == 2)
    {
        modifier += 800;
    }
    else if (checkBoard(playingBoard, x, y, changePiece(piece), 3) == 2)
    {
        modifier += 700;
    }
    playingBoard[y][x] = piece;
    //check to see if in middle
    for (int i = 1; i < 4; ++i)
    {
        for (int j = 1; j < 4; ++j)
        {
            bool midx = ((j == 1) || (j == 3));
            bool midy = ((i == 1) || (i == 3));
            if (playingBoard[j][i] == piece)
            {
                ++modifier;
                if (midx && midy)
                {
                    ++modifier;
                }
            }
            if (playingBoard[j][i] == changePiece(piece))
            {
                --modifier;
                if (midx && midy)
                {
                    --modifier;
                }
            }
        }
    }
    //middle is good
    if ((x == 2) && (y == 2))
    {
        modifier += 197;
    }
    //*******There are no pieces of the same type next to eachother options******
    //check how many enimies are around this spot, the more the better
    int numEnimies = aroundPiece(playingBoard, x, y, changePiece(piece));
    if (numEnimies > 0)
    {
        modifier += 2 * numEnimies;
    }
    //add around your other pieces
    int numAlies = aroundPiece(playingBoard, x, y, piece);
    if (numAlies > 0)
    {
        modifier += numAlies;
    }
    //If it is none of the others make it have a random value
    return modifier;
}

int botTurn(vector<vector<char>> &playingBoard, char piece, fstream &save)
{
    //needs at least c++ 11
    std::this_thread::sleep_for(std::chrono::milliseconds(1500));
    int bestx;
    int besty;
    int bestScore = -1000000000;
    if (boardBlank(playingBoard))
    {
        bestx = 2;
        besty = 2;
        playingBoard[besty][bestx] = piece;
        printBoard(playingBoard, cout);
        if (save)
            printBoard(playingBoard, save);
        return checkBoard(playingBoard, bestx, besty, piece, 4);
    }
    else
    {
        for (int y = 0; y < 5; ++y)
        {
            for (int x = 0; x < 5; ++x)
            {
                if (validMove(playingBoard, x, y))
                {
                    playingBoard[y][x] = piece;
                    int score = minimax(playingBoard, 0, false, changePiece(piece), x, y);
                    if (score > bestScore)
                    {
                        bestScore = score;
                        bestx = x;
                        besty = y;
                    }
                    playingBoard[y][x] = ' ';
                }
            }
        }
    }
    playingBoard[besty][bestx] = piece;
    printBoard(playingBoard, cout);
    if (save)
        printBoard(playingBoard, save);
    return checkBoard(playingBoard, bestx, besty, piece, 4);
}

bool win(fstream &save, int player, int bot)
{
    if (player == 1)
    {
        cout << "\nPlayer2 Forfits, Bot1 wins\n"
             << endl;
        if (save)
        {
            save << "Player2 Forfits, Bot1 wins" << endl;
        }
        return true;
    }
    if (player == 2)
    {
        cout << "Player2 wins" << endl;
        if (save)
        {
            save << "Player2 winds" << endl;
        }
        return true;
    }
    if (bot == 2)
    {
        cout << "Bot1 wins" << endl;
        if (save)
        {
            save << "Bot1 wins" << endl;
        }
        return true;
    }
    if ((player == 3) || (bot == 3))
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

void gameLoop(fstream &save, bool first, bool &bot2)
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
    int playerState = 0;
    int botState = 0;
    while (!done)
    {
        ++turn;
        if ((turn == 14) && (playerState != 2) && (botState != 2))
        {
            playerState = 3;
            botState = 3;
        }
        bool won = win(save, playerState, botState);
        if (won)
        {
            return;
        }
        if (first)
        {
            if (!bot2)
            {
                cout << "Player's turn" << endl;
                if (save)
                    save << "Player's turn" << endl;
                playerState = playerTurn(playingBoard, 'x', save);
                if (playerState == 1)
                {
                    continue;
                }
            }
            else if (bot2)
            {
                cout << "Bot 2's turn" << endl;
                if (save)
                    save << "Bot 2's turn" << endl;
                playerState = botTurn(playingBoard, 'x', save);
            }
            if ((playerState == 2) || (turn == 13))
            {
                continue;
            }
            cout << "Bot 1's turn" << endl;
            if (save)
                save << "Bot 1's turn" << endl;
            botState = botTurn(playingBoard, 'o', save);
        }
        if (!first)
        {
            cout << "Bot 1's turn" << endl;
            if (save)
                save << "Bot 1's turn" << endl;
            botState = botTurn(playingBoard, 'x', save);
            if ((botState == 2) || (turn == 13))
            {
                continue;
            }
            if (!bot2)
            {
                cout << "Player's turn" << endl;
                if (save)
                    save << "Player's turn" << endl;
                playerState = playerTurn(playingBoard, 'o', save);
                if (playerState == 1)
                {
                    continue;
                }
            }
            else
            {
                cout << "Bot 2's turn" << endl;
                if (save)
                    save << "Bot 2's turn" << endl;
                playerState = botTurn(playingBoard, 'o', save);
            }
        }
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
    cout << "How many bots should there be? (1/2)" << endl;
    int botCount = getSingleInt('1', '2');
    if (botCount == 2)
    {
        isBot2 = true;
    }
    srand(time(NULL));
    bool first = rand() % 2;
    if (first)
    {
        cout << "Player goes first" << endl;
        if (savelog)
            savelog << "Player goes first" << endl;
    }
    else
    {
        cout << "Bot goes first" << endl;
        if (savelog)
            savelog << "Player goes first" << endl;
    }
    gameLoop(savelog, first, isBot2);
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
