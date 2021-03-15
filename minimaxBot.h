#include "player.h"

class MinimaxBot : public Player
{
protected:
    bool boardBlank(std::vector<std::vector<char>> &playingBoard);
    int aroundPiece(std::vector<std::vector<char>> &playingBoard, int x, int y, char piece);
    int minimax(std::vector<std::vector<char>> &playingBoard, int depth, bool maximizing, char piece, int lastx, int lasty);
    int evaluate(std::vector<std::vector<char>> &playingBoard, char piece, int x, int y);

public:
    MinimaxBot();
    int turn(std::vector<std::vector<char>> &playingBoard, char piece) override;
};

std::shared_ptr<Player> makeMinimaxBot();