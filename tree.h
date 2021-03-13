#ifndef TREE_H
#define TREE_H

#include <vector>
#include <fstream>
#include <string>
#include <memory>
#include <iostream>
#include <cmath>

struct nodeData
{
    char piece;
    int y;
    int x;
    double visits = 0;
    double wins = 0;
    double losses = 0;
    double draws = 0;
};

class BoardNode
{
protected:
    char changePiece(char piece);

public:
    std::vector<std::vector<char>> board;
    BoardNode(std::vector<std::vector<char>> nboard, char);
    BoardNode(std::vector<std::vector<char>> nboard, char, std::shared_ptr<BoardNode> pnode, int, int);
    std::vector<std::shared_ptr<BoardNode>> children;
    nodeData parrent;
    nodeData data;
    void addLayer(std::shared_ptr<BoardNode> base);
    void growTree(int depth);
    double value();
};

#endif