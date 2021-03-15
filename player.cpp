#include "player.h"
using std::cin;
using std::cout;
using std::endl;
using std::getline;
using std::string;
using std::vector;

// Player definitions
Player::Player()
{
    _name = "Player";
}

char Player::changePiece(char piece)
{
    if (piece == 'x')
    {
        return 'o';
    }
    return 'x';
}

bool Player::validMove(vector<vector<char>> &playingBoard, int x, int y)
{
    return playingBoard[y][x] == ' ';
}

int Player::getSingleInt(char min, char max)
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

int Player::turn(vector<vector<char>> &playingBoard, char piece)
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
        return checkBoard(playingBoard, x, y, piece, 4);
    }
    else
    {
        cout << "Enter Valid Position" << endl;
        turn(playingBoard, piece);
    }
}

int Player::checkBoard(vector<vector<char>> &playingBoard, int x, int y, char piece, int max)
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

/******************* MakePlayer Functions**************************/
std::shared_ptr<Player> makePlayer()
{
    return std::make_shared<Player>(Player());
}
