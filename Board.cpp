#include "Board.h"

#include <iostream>
#include <iomanip>
#include <string>
#include <queue>
#include <random>
#include <cmath>

using namespace std;

Board::Board(int rows, int cols, int mineCount)
    : rows(rows),
      cols(cols),
      mineCount(mineCount),
      initialized(false),
      mines(rows, vector<bool>(cols, false)),
      revealed(rows, vector<bool>(cols, false)),
      flagged(rows, vector<bool>(cols, false)) {
}

bool Board::inside(int row, int col) const {
    return row >= 0 &&
           row < rows &&
           col >= 0 &&
           col < cols;
}

bool Board::isValid(int row, int col) const {
    return inside(row, col);
}

int Board::countMines(int row, int col) const {
    int count = 0;

    for (int dr = -1; dr <= 1; dr++) {
        for (int dc = -1; dc <= 1; dc++) {
            if (dr == 0 && dc == 0) {
                continue;
            }

            int nr = row + dr;
            int nc = col + dc;

            if (inside(nr, nc) && mines[nr][nc]) {
                count++;
            }
        }
    }

    return count;
}

void Board::generateMines(int safeRow, int safeCol) {
    random_device randomDevice;
    mt19937 generator(randomDevice());

    int placed = 0;

    while (placed < mineCount) {
        int row = generator() % rows;
        int col = generator() % cols;

        if (abs(row - safeRow) <= 1 &&
            abs(col - safeCol) <= 1) {
            continue;
        }

        if (!mines[row][col]) {
            mines[row][col] = true;
            placed++;
        }
    }

    initialized = true;
}

void Board::print() const {
    cout << "\n";

    // 列标题
    cout << "    |";

    for (int col = 0; col < cols; col++) {
        cout << left << setw(3) << col;
    }

    cout << "\n";

    // 棋盘
    for (int row = 0; row < rows; row++) {
        cout << right << setw(3) << row << " |";

        for (int col = 0; col < cols; col++) {
            string cell;

            if (flagged[row][col] && !revealed[row][col]) {
                cell = "F";
            } else if (revealed[row][col]) {
                if (mines[row][col]) {
                    cell = "*";
                } else {
                    cell = to_string(countMines(row, col));
                }
            } else {
                cell = "#";
            }

            cout << left << setw(3) << cell;
        }

        cout << "\n";
    }
}

void Board::reveal(int row, int col) {
    if (!inside(row, col) ||
        revealed[row][col] ||
        flagged[row][col]) {
        return;
    }

    if (!initialized) {
        generateMines(row, col);
    }

    queue<pair<int, int>> pending;
    pending.push({row, col});

    while (!pending.empty()) {
        auto current = pending.front();
        pending.pop();

        int currentRow = current.first;
        int currentCol = current.second;

        if (!inside(currentRow, currentCol) ||
            revealed[currentRow][currentCol] ||
            flagged[currentRow][currentCol]) {
            continue;
        }

        revealed[currentRow][currentCol] = true;

        if (mines[currentRow][currentCol]) {
            continue;
        }

        if (countMines(currentRow, currentCol) == 0) {
            for (int dr = -1; dr <= 1; dr++) {
                for (int dc = -1; dc <= 1; dc++) {
                    pending.push({
                        currentRow + dr,
                        currentCol + dc
                    });
                }
            }
        }
    }
}
bool Board::chordReveal(int row, int col) {
    if (!inside(row, col) ||
        !revealed[row][col]) {
        return false;
    }

    int flagsAround = 0;

    for (int dr = -1; dr <= 1; dr++) {
        for (int dc = -1; dc <= 1; dc++) {
            int nr = row + dr;
            int nc = col + dc;

            if (inside(nr, nc) && flagged[nr][nc]) {
                flagsAround++;
            }
        }
    }

    // 只有旗子数量等于数字时才清理
    if (flagsAround != countMines(row, col)) {
        return false;
    }

    bool hitMine = false;

    for (int dr = -1; dr <= 1; dr++) {
        for (int dc = -1; dc <= 1; dc++) {
            int nr = row + dr;
            int nc = col + dc;

            if (inside(nr, nc) &&
                !flagged[nr][nc] &&
                !revealed[nr][nc]) {

                reveal(nr, nc);

                if (mines[nr][nc]) {
                    hitMine = true;
                }
            }
        }
    }

    return hitMine;
}

void Board::toggleFlag(int row, int col) {
    if (!inside(row, col) || revealed[row][col]) {
        return;
    }

    flagged[row][col] = !flagged[row][col];
}

bool Board::isMine(int row, int col) const {
    return inside(row, col) && mines[row][col];
}

bool Board::isRevealed(int row, int col) const {
    return inside(row, col) && revealed[row][col];
}
bool Board::isFlagged(int row, int col) const {
    return inside(row, col) && flagged[row][col];
}
int Board::adjacentMines(int row, int col) const {
    if (!inside(row, col)) {
        return 0;
    }

    return countMines(row, col);
}

bool Board::isWon() const {
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            if (!mines[row][col] &&
                !revealed[row][col]) {
                return false;
            }
        }
    }

    return true;
}
void Board::revealAllMines() {
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            if (mines[row][col]) {
                revealed[row][col] = true;
            }
        }
    }
}