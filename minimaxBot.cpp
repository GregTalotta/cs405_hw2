#include "minimaxBot.h"
using std::cin;
using std::cout;
using std::endl;
using std::fstream;
using std::getline;
using std::istream;
using std::ostream;
using std::string;
using std::vector;

std::shared_ptr<Player> makeMinimaxBot()
{
    return std::make_shared<MinimaxBot>(MinimaxBot());
}

// MinimaxBot Definitions
MinimaxBot::MinimaxBot()
{
    _name = "Minimax Bot";
}

bool MinimaxBot::boardBlank(vector<vector<char>> &playingBoard)
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

int MinimaxBot::aroundPiece(vector<vector<char>> &playingBoard, int x, int y, char piece)
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

int MinimaxBot::minimax(vector<vector<char>> &playingBoard, int depth, bool maximizing, char piece, int lastx, int lasty)
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

    ++calcBoards;
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

int MinimaxBot::evaluate(vector<vector<char>> &playingBoard, char piece, int x, int y)
{
    int modifier = 10;
    //check if it makes you win
    if ((y > 0) && (y < 4) && (x > 0) && (x < 4))
    {
        modifier += 10;
    }
    if (checkBoard(playingBoard, x, y, piece, 4) == 2)
    {
        modifier += 900;
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

int MinimaxBot::turn(vector<vector<char>> &playingBoard, char piece)
{
    int bestx;
    int besty;
    int bestScore = -1000000000;
    calcBoards = 0;
    if (boardBlank(playingBoard))
    {
        bestx = 2;
        besty = 2;
        playingBoard[besty][bestx] = piece;
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
    return checkBoard(playingBoard, bestx, besty, piece, 4);
}