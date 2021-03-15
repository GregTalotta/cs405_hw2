#include "tree.h"
using std::rand;

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
    data->piece = p;
}

BoardNode::BoardNode(std::vector<std::vector<char>> nboard, char p, std::shared_ptr<BoardNode> &pnode, int posx, int posy)
{
    board = nboard;
    parrent = pnode;
    data->piece = p;
    data->x = posx;
    data->y = posy;
}

double BoardNode::value()
{
    if (data->visits == 0)
    {
        return 0;
    }
    return ((data->wins + data->draws) / data->visits);
}

char BoardNode::changePiece(char piece)
{
    if (piece == 'x')
    {
        return 'o';
    }
    return 'x';
}

void BoardNode::addLayer(std::shared_ptr<BoardNode> &base)
{
    for (int j = 0; j < board.size(); ++j)
    {
        for (int i = 0; i < board[0].size(); ++i)
        {
            if (board[j][i] == ' ')
            {
                board[j][i] = changePiece(data->piece);
                children.push_back(std::make_unique<BoardNode>(BoardNode(board, changePiece(data->piece), base, i, j)));
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
