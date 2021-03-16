/*  AI using the Monte Carlo method.
    I based my implimentation on: https://www.baeldung.com/java-monte-carlo-tree-search
*/

#include "monteCarloBot.h"
using std::shared_ptr;
using std::make_shared;
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

void MonteCarloBot::setTime(double in){
    _time = in;
}

int MonteCarloBot::turn(std::vector<std::vector<char>> &playingBoard, char piece)
{
    ++_rounds;
    _calcBoards = 0;
    std::shared_ptr<BoardNode> root = make_shared<BoardNode>(BoardNode(playingBoard, changePiece(piece)));
    root->addLayer(root);
    _calcBoards += root->children.size() + 1;
    if(root->children.size()==1){
        _calcBoardsLifeTime += _calcBoards;
        playingBoard[root->children[0]->y][root->children[0]->x]=_piece;
        return checkBoard(playingBoard, root->children[0]->x, root->children[0]->y, _piece, 4);
    }
    if (_time <= 0.0)
    {
        double in=1.0;
        std::cout << "How many seconds should the bot have?" << std::endl;
        std::cin>>in;
        setTime(in);
        std::cout << "The bot will play for about "<< _time << " seconds" << std::endl;
    }
    auto startTime = std::chrono::system_clock::now();
    auto endTime = std::chrono::system_clock::now();
    std::chrono::duration<double> diff = endTime - startTime;
    while (diff.count() < _time)
    {
        shared_ptr<BoardNode> goodNode = selectionPhase(root);
        if (goodNode->children.size() == 0) //can exspand condition
        {
            exspansionPhase(goodNode);
            _calcBoards += goodNode->children.size();
        }
        shared_ptr<BoardNode> exploringNode = goodNode;
        if (exploringNode->children.size() > 0)
        {
            exploringNode = goodNode->getRandomChild();
        }
        int playoutResult = simulationPhase(exploringNode);
        backPropogation(exploringNode, playoutResult);
        exploringNode.reset();
        //end loop
        endTime = std::chrono::system_clock::now();
        diff = endTime - startTime;
    }
    shared_ptr<BoardNode> bestNode = root->getBestChild();
    
    int nx = bestNode->x;
    int ny = bestNode->y;
    root.reset();
    bestNode.reset();
    _calcBoardsLifeTime += _calcBoards;
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
    return ((node->wins + node->draws) / node->visits) + (sqrt(2) * sqrt(log(node->parent->visits) / node->visits));
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
    std::vector<vector<char>> tempBoard = node->board;
    int pastStatus = checkBoard(node->parent->board, node->parent->x, node->parent->y, node->parent->piece, 4);
    if(pastStatus == 2){
        return pastStatus;
    }
    int gameStatus = checkBoard(tempBoard, node->x, node->y, node->piece, 4);
    char curPiece =node->piece;
    while (gameStatus == 0)
    {
        int rx = rand() % (int)node->board.size();
        int ry = rand() % (int)node->board.size();
        if(tempBoard[ry][rx]==' '){
            ++_calcBoards;
            tempBoard[ry][rx] = curPiece;
            gameStatus = checkBoard(tempBoard, rx, ry, curPiece, 4);
            curPiece=changePiece(curPiece);
        }
    }
    if (gameStatus == 3)
    {
        return gameStatus;
    }
    if (_piece == curPiece)
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
        tempNode = tempNode->parent;
    }
    return;
}
