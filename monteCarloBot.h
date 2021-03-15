#ifndef MONTECARLOBOT_H
#define MONTECARLOBOT_H

#include <memory>
#include <chrono>
#include "player.h"
#include "boardNode.h"

class MonteCarloBot : public Player
{
protected:
    double time = 5.0;
    void findNextMove(std::vector<std::vector<char>> &playingBoard);
    std::shared_ptr<BoardNode> selectionPhase(std::shared_ptr<BoardNode> root);
    double utcValue(std::shared_ptr<BoardNode> node);
    void exspansionPhase(std::shared_ptr<BoardNode> node);
    int simulationPhase(std::shared_ptr<BoardNode> node);
    void backPropogation(std::shared_ptr<BoardNode> node, int state);

public:
    MonteCarloBot();
    void setTime();
    int turn(std::vector<std::vector<char>> &playingBoard, char piece) override;
};


std::shared_ptr<Player> makeMonteCarloBot();

#endif