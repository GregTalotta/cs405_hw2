#include "tree.h"

BoardNode::BoardNode(std::vector<std::vector<char>> nboard, char p)
{
    board = nboard;
    data.piece = p;
}

BoardNode::BoardNode(std::vector<std::vector<char>> nboard, char p, std::shared_ptr<BoardNode> pnode, int posx, int posy)
{
    board = nboard;
    parrent = pnode->data;
    data.piece = p;
    data.x = posx;
    data.y = posy;
}

double BoardNode::value()
{
    if (data.visits == 0)
    {
        return 0;
    }
    return ((data.wins + data.draws) / data.visits) + (sqrt(2) * sqrt(log(parrent.visits) / data.visits));
}

char BoardNode::changePiece(char piece)
{
    if (piece == 'x')
    {
        return 'o';
    }
    return 'x';
}

void BoardNode::addLayer(std::shared_ptr<BoardNode> base)
{
    for (int j = 0; j < board.size(); ++j)
    {
        for (int i = 0; i < board[0].size(); ++i)
        {
            if (board[j][i] == ' ')
            {
                board[j][i] = changePiece(data.piece);
                children.push_back(std::make_shared<BoardNode>(BoardNode(board, changePiece(data.piece), base, i, j)));
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
    for (auto i : children)
    {
        i->addLayer(i);
        i->growTree(depth - 1);
    }
}
