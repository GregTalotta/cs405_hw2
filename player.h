#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include <fstream>
#include <string>
#include <memory>
#include <iostream>

class Player
{
protected:
    int checkBoard(std::vector<std::vector<char>> &playingBoard, int x, int y, char piece, int max);
    int getSingleInt(char min, char max);
    bool validMove(std::vector<std::vector<char>> &playingBoard, int x, int y);
    int calcBoards = 0;

public:
    Player();
    std::string _name;
    char _piece;
    virtual int turn(std::vector<std::vector<char>> &playingBoard, char piece);
};

class MinimaxBot : public Player
{
protected:
    bool boardBlank(std::vector<std::vector<char>> &playingBoard);
    char changePiece(char piece);
    int aroundPiece(std::vector<std::vector<char>> &playingBoard, int x, int y, char piece);
    int minimax(std::vector<std::vector<char>> &playingBoard, int depth, bool maximizing, char piece, int lastx, int lasty);
    int evaluate(std::vector<std::vector<char>> &playingBoard, char piece, int x, int y);

public:
    MinimaxBot();
    int turn(std::vector<std::vector<char>> &playingBoard, char piece) override;
};

// class MonteCarloBot : public Player
// {
//     int turn(std::vector<std::vector<char>> &playingBoard, char piece);
// };

std::shared_ptr<Player> makePlayer();
std::shared_ptr<Player> makeMinimaxBot();
// std::shared_ptr<Player> makeMonteCarloBot();

#endif
