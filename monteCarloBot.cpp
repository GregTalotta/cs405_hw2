/*  AI using the Monte Carlo method.
    I based my implimentation on: https://www.baeldung.com/java-monte-carlo-tree-search
*/

#include "monteCarloBot.h"
using std::cin;
using std::cout;
using std::endl;
using std::getline;
using std::istream;
using std::make_unique;
using std::ostream;
using std::shared_ptr;
using std::string;
using std::vector;

int MonteCarloBot::turn(std::vector<std::vector<char>> &playingBoard, char piece)
{
    if (!root)
    {
        root = make_unique<BoardNode>(BoardNode(playingBoard, changePiece(piece)));
        root->addLayer(root);
    }
    if (time <= 0.0)
    {
        //cout some stuff
        //setTime()
    }
    auto startTime = std::chrono::system_clock::now();
    auto endTime = std::chrono::system_clock::now();
    std::chrono::duration<double> runTime = endTime - startTime;
    while (runTime.count() < time)
    {
        shared_ptr<BoardNode> goodNode = selectionPhase(root);
        if (true) //can exspand condition
        {
            exspansionPhase(goodNode);
        }
        shared_ptr<BoardNode> exploringNode = goodNode;
        if (exploringNode->children.size() > 0)
        {
            exploringNode = goodNode->getRandomChild();
        }
        int playoutResult = simulationPhase(exploringNode);
        backPropogation(exploringNode, playoutResult);
        //end loop
        endTime = std::chrono::system_clock::now();
        runTime = endTime - startTime;
    }
    shared_ptr<BoardNode> bestNode = root->getBestChild();
    root = bestNode;
    root->parrent.reset();
    playingBoard[root->data->y][root->data->x] = piece;
    return checkBoard(playingBoard, root->data->x, root->data->y, _piece, 4);
}

std::shared_ptr<BoardNode> MonteCarloBot::selectionPhase(std::shared_ptr<BoardNode> root)
{
    std::shared_ptr<BoardNode> node = root->children[0];
    for (int i = 1; i < root->children.size(); ++i)
    {
        if (utcValue(root->children[i]) > utcValue(node))
        {
            node = root->children[i];
        }
    }
    return node;
}

double MonteCarloBot::utcValue(std::shared_ptr<BoardNode> node)
{
    return ((node->data->wins + node->data->draws) / node->data->visits) + (sqrt(2) * sqrt(log(node->parrent->data->visits) / node->data->visits));
}

void MonteCarloBot::exspansionPhase(std::shared_ptr<BoardNode> node)
{
    node->addLayer(node);
}

int MonteCarloBot::simulationPhase(std::shared_ptr<BoardNode> node)
{
    std::shared_ptr<BoardNode> tempNode = node;
    int gameStatus = checkBoard(tempNode->board, root->data->x, root->data->y, node->data->piece, 4);
    while (gameStatus == 0)
    {
        if (tempNode->children.size() == 0)
        {
            tempNode->addLayer(tempNode);
        }
        tempNode = tempNode->getRandomChild();
        gameStatus = checkBoard(tempNode->board, root->data->x, root->data->y, node->data->piece, 4);
    }
    node->children.clear();
    if (gameStatus == 3)
    {
        return gameStatus;
    }
    if (_piece != tempNode->data->piece)
    {
        return 1;
    }
    return gameStatus;
}

void MonteCarloBot::backPropogation(std::shared_ptr<BoardNode> node, int state)
{
    std::shared_ptr<BoardNode> tempNode = node;
    while (tempNode)
    {
        ++tempNode->data->visits;
        if (tempNode->data->piece == _piece)
        {
            if (state == 2)
            {
                ++tempNode->data->wins;
            }
            else if (state == 3)
            {
                ++tempNode->data->draws;
            }
        }
        tempNode = tempNode->parrent;
    }
    return;
}