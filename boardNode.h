#ifndef TREE_H
#define TREE_H

#include <vector>
#include <fstream>
#include <string>
#include <memory>
#include <iostream>
#include <cmath>
#include <cstdlib>


class BoardNode
{
protected:
    char changePiece(char piece);
    bool validMove(std::vector<std::vector<char>> &playingBoard, int x, int y);

public:
    char piece;
    int y;
    int x;
    double visits = 0.0;
    double wins = 0.0;
    double draws = 0.0;
    std::shared_ptr<BoardNode> parrent;
    //std::shared_ptr<nodeData> data = std::make_shared<nodeData>();
    std::vector<std::vector<char>> board;
    // ~BoardNode();
    BoardNode(std::vector<std::vector<char>> nboard, char);
    BoardNode(std::vector<std::vector<char>> nboard, char, std::shared_ptr<BoardNode> pnode, int, int);
    std::vector<std::shared_ptr<BoardNode>> children;
    void addLayer(std::shared_ptr<BoardNode> base);
    void growTree(int depth);
    double value();
    std::shared_ptr<BoardNode> getRandomChild();
    std::shared_ptr<BoardNode> getBestChild();
};

#endif