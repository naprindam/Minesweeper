#include <SFML/Graphics.hpp>

#include <chrono>
#include <string>

#include "Board.h"

using namespace std;

int main() {
    const int tileSize = 40;
    const int panelHeight = 80;

    const int windowWidth = 1200;
    const int windowHeight = 760;

    sf::RenderWindow window(
        sf::VideoMode(windowWidth, windowHeight),
        "Minesweeper"
    );

    sf::Font font;
    bool hasFont = font.loadFromFile(
        "C:/Windows/Fonts/arial.ttf"
    );

    int rows = 9;
    int cols = 9;
    int mineCount = 10;

    Board board(rows, cols, mineCount);

    bool gameOver = false;
    bool gameWon = false;

    auto startTime = chrono::steady_clock::now();
    sf::Clock doubleClickClock;
    sf::Vector2i lastCell(-1, -1);
    auto resetGame = [&](int newRows,
                         int newCols,
                         int newMineCount) {
        rows = newRows;
        cols = newCols;
        mineCount = newMineCount;

        board = Board(rows, cols, mineCount);

        gameOver = false;
        gameWon = false;
        startTime = chrono::steady_clock::now();

        window.setTitle("Minesweeper");
    };

    auto numberColor = [](int number) {
        if (number == 1) {
            return sf::Color::Blue;
        }

        if (number == 2) {
            return sf::Color::Green;
        }

        if (number == 3) {
            return sf::Color::Red;
        }

        if (number == 4) {
            return sf::Color(100, 0, 150);
        }

        return sf::Color::Black;
    };

    while (window.isOpen()) {
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::MouseButtonPressed) {
                int mouseX = event.mouseButton.x;
                int mouseY = event.mouseButton.y;

                // Restart 按钮
                sf::FloatRect restartButton(
                    20, 15, 130, 40
                );

                if (restartButton.contains(mouseX, mouseY)) {
                    resetGame(rows, cols, mineCount);
                    continue;
                }

                // Beginner 按钮
                sf::FloatRect beginnerButton(
                    180, 15, 130, 40
                );

                if (beginnerButton.contains(mouseX, mouseY)) {
                    resetGame(9, 9, 10);
                    continue;
                }

                // Intermediate 按钮
                sf::FloatRect intermediateButton(
                    330, 15, 150, 40
                );

                if (intermediateButton.contains(mouseX, mouseY)) {
                    resetGame(16, 16, 40);
                    continue;
                }

                // Expert 按钮
                sf::FloatRect expertButton(
                    500, 15, 130, 40
                );

                if (expertButton.contains(mouseX, mouseY)) {
                    resetGame(16, 30, 99);
                    continue;
                }

                if (gameOver || gameWon) {
                    continue;
                }

                // 棋盘左上角居中
                int boardWidth = cols * tileSize;
                int boardLeft = (windowWidth - boardWidth) / 2;
                int boardTop = panelHeight;

                if (mouseX < boardLeft ||
                    mouseX >= boardLeft + boardWidth ||
                    mouseY < boardTop ||
                    mouseY >= boardTop + rows * tileSize) {
                    continue;
                }

                int col = (mouseX - boardLeft) / tileSize;
                int row = (mouseY - boardTop) / tileSize;

                if (!board.isValid(row, col)) {
                    continue;
                }

                // 左键揭开
                if (event.mouseButton.button ==
                    sf::Mouse::Left) {

                    sf::Vector2i currentCell(col, row);

                    bool isDoubleClick =
                        currentCell == lastCell &&
                        doubleClickClock.getElapsedTime()
                            .asMilliseconds() < 400;

                    doubleClickClock.restart();
                    lastCell = currentCell;

                    if (isDoubleClick &&
                        board.isRevealed(row, col)) {

                        bool hitMine =
                            board.chordReveal(row, col);

                        if (hitMine) {
                            board.revealAllMines();
                            gameOver = true;
                            window.setTitle(
                                "Game Over - Press Restart"
                            );
                        }
                    } else {
                        board.reveal(row, col);

                        if (board.isMine(row, col)) {
                            board.revealAllMines();
                            gameOver = true;
                            window.setTitle(
                                "Game Over - Press Restart"
                            );
                        }
                    }

                    if (!gameOver && board.isWon()) {
                        gameWon = true;
                        window.setTitle("You Win!");
                    }
                }

                // 右键插旗
                if (event.mouseButton.button ==
                    sf::Mouse::Right) {
                    board.toggleFlag(row, col);
                }
            }
        }

        int elapsedSeconds =
            static_cast<int>(
                chrono::duration_cast<chrono::seconds>(
                    chrono::steady_clock::now() - startTime
                ).count()
            );

        int flagCount = 0;

        for (int row = 0; row < rows; row++) {
            for (int col = 0; col < cols; col++) {
                if (board.isFlagged(row, col)) {
                    flagCount++;
                }
            }
        }

        int remainingMines = mineCount - flagCount;

        window.clear(sf::Color(35, 35, 35));

        // 顶部按钮
        sf::RectangleShape restartButtonShape(
            sf::Vector2f(130, 40)
        );
        restartButtonShape.setPosition(20, 15);
        restartButtonShape.setFillColor(
            sf::Color(70, 130, 220)
        );
        window.draw(restartButtonShape);

        sf::RectangleShape beginnerButtonShape(
            sf::Vector2f(130, 40)
        );
        beginnerButtonShape.setPosition(180, 15);
        beginnerButtonShape.setFillColor(
            sf::Color(70, 160, 90)
        );
        window.draw(beginnerButtonShape);

        sf::RectangleShape intermediateButtonShape(
            sf::Vector2f(150, 40)
        );
        intermediateButtonShape.setPosition(330, 15);
        intermediateButtonShape.setFillColor(
            sf::Color(210, 150, 50)
        );
        window.draw(intermediateButtonShape);

        sf::RectangleShape expertButtonShape(
            sf::Vector2f(130, 40)
        );
        expertButtonShape.setPosition(500, 15);
        expertButtonShape.setFillColor(
            sf::Color(190, 70, 70)
        );
        window.draw(expertButtonShape);

        if (hasFont) {
            sf::Text restartText;
            restartText.setFont(font);
            restartText.setString("Restart");
            restartText.setCharacterSize(18);
            restartText.setFillColor(sf::Color::White);
            restartText.setPosition(45, 23);
            window.draw(restartText);

            sf::Text beginnerText;
            beginnerText.setFont(font);
            beginnerText.setString("Beginner");
            beginnerText.setCharacterSize(17);
            beginnerText.setFillColor(sf::Color::White);
            beginnerText.setPosition(195, 23);
            window.draw(beginnerText);

            sf::Text intermediateText;
            intermediateText.setFont(font);
            intermediateText.setString("Intermediate");
            intermediateText.setCharacterSize(16);
            intermediateText.setFillColor(sf::Color::White);
            intermediateText.setPosition(340, 23);
            window.draw(intermediateText);

            sf::Text expertText;
            expertText.setFont(font);
            expertText.setString("Expert");
            expertText.setCharacterSize(17);
            expertText.setFillColor(sf::Color::White);
            expertText.setPosition(530, 23);
            window.draw(expertText);

            sf::Text infoText;
            infoText.setFont(font);
            infoText.setCharacterSize(20);
            infoText.setFillColor(sf::Color::White);
            infoText.setString(
                "Mines: " +
                to_string(remainingMines) +
                "    Time: " +
                to_string(elapsedSeconds)
            );
            infoText.setPosition(700, 22);
            window.draw(infoText);
        }

        // 计算棋盘位置
        int boardWidth = cols * tileSize;
        int boardLeft = (windowWidth - boardWidth) / 2;
        int boardTop = panelHeight;

        for (int row = 0; row < rows; row++) {
            for (int col = 0; col < cols; col++) {
                sf::RectangleShape tile;

                tile.setSize(sf::Vector2f(
                    tileSize - 2,
                    tileSize - 2
                ));

                tile.setPosition(
                    boardLeft + col * tileSize + 1,
                    boardTop + row * tileSize + 1
                );

                if (board.isFlagged(row, col)) {
                    tile.setFillColor(
                        sf::Color(220, 80, 80)
                    );
                } else if (board.isMine(row, col) &&
                           gameOver) {
                    tile.setFillColor(
                        sf::Color(40, 40, 40)
                    );
                } else if (board.isRevealed(row, col)) {
                    tile.setFillColor(
                        sf::Color(150, 190, 150)
                    );
                } else {
                    tile.setFillColor(
                        sf::Color(180, 180, 180)
                    );
                }

                window.draw(tile);

                if (hasFont &&
                    board.isRevealed(row, col) &&
                    !board.isMine(row, col)) {

                    int number =
                        board.adjacentMines(row, col);

                    if (number > 0) {
                        sf::Text numberText;

                        numberText.setFont(font);
                        numberText.setString(
                            to_string(number)
                        );
                        numberText.setCharacterSize(26);
                        numberText.setFillColor(
                            numberColor(number)
                        );
                        numberText.setPosition(
                            boardLeft + col * tileSize + 13,
                            boardTop + row * tileSize + 5
                        );

                        window.draw(numberText);
                    }
                }
            }
        }

        if (hasFont && (gameOver || gameWon)) {
            sf::Text resultText;

            resultText.setFont(font);
            resultText.setCharacterSize(30);
            resultText.setFillColor(
                gameWon
                    ? sf::Color::Green
                    : sf::Color::Red
            );

            resultText.setString(
                gameWon
                    ? "YOU WIN!"
                    : "GAME OVER"
            );

            resultText.setPosition(850, 680);
            window.draw(resultText);
        }

        window.display();
    }

    return 0;
}