#include "player.h"
#include "tree.h"
#include <memory>
#include <chrono>

class MonteCarloBot : public Player
{
protected:
    std::shared_ptr<BoardNode> root;
    double time = 0.0;
    void findNextMove(std::vector<std::vector<char>> &playingBoard);
    std::shared_ptr<BoardNode> selectionPhase(std::shared_ptr<BoardNode> root);
    double utcValue(std::shared_ptr<BoardNode> node);
    void exspansionPhase(std::shared_ptr<BoardNode> node);
    int simulationPhase(std::shared_ptr<BoardNode> node);
    void backPropogation(std::shared_ptr<BoardNode> node, int state);
public:
    MonteCarloBot();
    void setTime();
    int turn(std::vector<std::vector<char>> &playingBoard, char piece);
};

// utc calc: ((data->wins + data->draws) / data->visits) + (sqrt(2) * sqrt(log(parrent->data->visits) / data->visits))

std::shared_ptr<Player> makeMonteCarloBot();