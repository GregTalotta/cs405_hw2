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

std::shared_ptr<Player> makeMonteCarloBot()
{
    return std::make_shared<MonteCarloBot>(MonteCarloBot());
}

// MonteCarloBot Definitions
MonteCarloBot::MonteCarloBot()
{
    _name = "Monte Carlo Bot";
}

int MonteCarloBot::turn(std::vector<std::vector<char>> &playingBoard, char piece)
{
    std::shared_ptr<BoardNode> root = make_unique<BoardNode>(BoardNode(playingBoard, changePiece(piece)));
    root->addLayer(root);
    if (time <= 0.0)
    {
        //cout some stuff
        //setTime()
    }
    auto startTime = std::chrono::system_clock::now();
    auto endTime = std::chrono::system_clock::now();
    std::chrono::duration<double> diff = endTime - startTime;
    while (diff.count() < time)
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
        exploringNode.reset();
        for(int i = 0; i < root->children.size(); ++i){
            root->children[i]->children.clear();
        }
        cout << root->children[1].use_count() << endl;
        //end loop
        endTime = std::chrono::system_clock::now();
        diff = endTime - startTime;
    }
    shared_ptr<BoardNode> bestNode = root->getBestChild();
    
    int nx = bestNode->x;
    int ny = bestNode->y;
    root.reset();
    bestNode.reset();
    playingBoard[ny][nx] = piece;
    return checkBoard(playingBoard, nx, ny, _piece, 4);
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
    if (node->visits == 0)
    {
        return 100000;
    }
    return ((node->wins + (0.5 * node->draws)) / node->visits) + (sqrt(2) * sqrt(log(node->parrent->visits) / node->visits));
}

void MonteCarloBot::exspansionPhase(std::shared_ptr<BoardNode> node)
{
    if (node->children.size() == 0)
    {
        node->addLayer(node);
    }
}

int MonteCarloBot::simulationPhase(std::shared_ptr<BoardNode> node)
{
    //This is the problem area
    std::shared_ptr<BoardNode> tempNode = node;
    int gameStatus = checkBoard(tempNode->board, tempNode->x, tempNode->y, node->piece, 4);
    while (gameStatus == 0)
    {
        if (tempNode->children.size() == 0)
        {
            tempNode->addLayer(tempNode);
        }
        tempNode = tempNode->getRandomChild();
        gameStatus = checkBoard(tempNode->board, tempNode->x, tempNode->y, node->piece, 4);
    }
    node->children.clear();
    if (gameStatus == 3)
    {
        return gameStatus;
    }
    if (_piece != tempNode->piece)
    {
        return 1;
    }
    node->children.clear();
    return gameStatus;
}

void MonteCarloBot::backPropogation(std::shared_ptr<BoardNode> node, int state)
{
    std::shared_ptr<BoardNode> tempNode = node;
    while (tempNode)
    {
        ++tempNode->visits;
        if (tempNode->piece == _piece)
        {
            if (state == 2)
            {
                ++tempNode->wins;
            }
            else if (state == 3)
            {
                ++tempNode->draws;
            }
        }
        tempNode = tempNode->parrent;
    }
    return;
}
