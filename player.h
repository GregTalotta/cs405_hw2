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
    int _calcBoards = 0;
    char changePiece(char piece);

public:
    Player();
    std::string _name;
    char _piece;
    virtual int turn(std::vector<std::vector<char>> &playingBoard, char piece);
};

std::shared_ptr<Player> makePlayer();

#endif
