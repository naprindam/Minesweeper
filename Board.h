#ifndef BOARD_H
#define BOARD_H

#include <vector>

class Board {
private:
    int rows;
    int cols;
    int mineCount;
    bool initialized;
    std::vector<std::vector<bool>> mines;
    std::vector<std::vector<bool>> revealed;
    std::vector<std::vector<bool>> flagged;

    bool inside(int row, int col) const;
    int countMines(int row, int col) const;
    void generateMines(int safeRow, int safeCol);

public:
    Board(int rows, int cols, int mineCount);
    void revealAllMines();
    void print() const;
    void reveal(int row, int col);
    bool chordReveal(int row, int col);
    void toggleFlag(int row, int col);
    bool isFlagged(int row, int col) const;
    int adjacentMines(int row, int col) const;
    bool isMine(int row, int col) const;
    bool isRevealed(int row, int col) const;
    bool isWon() const;
    bool isValid(int row, int col) const;
};

#endif