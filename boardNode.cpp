#include "boardNode.h"
#include <iostream>
using std::rand;
using std::vector;

std::shared_ptr<BoardNode> BoardNode::getRandomChild()
{
    int pos = rand() % (int)children.size();
    return children[pos];
}

std::shared_ptr<BoardNode> BoardNode::getBestChild()
{
    std::shared_ptr<BoardNode> best = children[0];
    for (int i = 1; i < children.size(); ++i)
    {
        if (children[i]->value() > best->value())
        {
            best = children[i];
        }
    }
    return best;
}

BoardNode::BoardNode(std::vector<std::vector<char>> nboard, char p)
{
    board = nboard;
    piece = p;
}

BoardNode::BoardNode(std::vector<std::vector<char>> nboard, char p, std::shared_ptr<BoardNode> pnode, int posx, int posy)
{
    board = nboard;
    parrent = pnode;
    piece = p;
    x = posx;
    y = posy;
}

double BoardNode::value()
{
    if (visits == 0)
    {
        return 0;
    }
    return ((wins + (0.5 * draws)) / visits);
}

char BoardNode::changePiece(char piece)
{
    if (piece == 'x')
    {
        return 'o';
    }
    return 'x';
}

bool BoardNode::validMove(vector<vector<char>> &playingBoard, int posx, int posy)
{
    return playingBoard[posy][posx] == ' ';
}

void BoardNode::addLayer(std::shared_ptr<BoardNode> base)
{
    for (int j = 0; j < base->board.size(); ++j)
    {
        for (int i = 0; i < base->board.size(); ++i)
        {
            if (validMove(board, i, j))
            {
                board[j][i] = changePiece(base->piece);
                children.push_back(std::make_unique<BoardNode>(BoardNode(board, changePiece(base->piece), base, i, j)));
                board[j][i] = ' ';
            }
        }
    }
}

void BoardNode::growTree(int depth)
{
    if (depth == 1)
    {
        return;
    }
    for (int i = 0; i < children.size(); ++i)
    {
        children[i]->addLayer(children[i]);
        children[i]->growTree(depth - 1);
    }
}
