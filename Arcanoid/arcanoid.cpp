#include "SFML/Graphics.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <time.h>
#include <math.h>

using namespace sf;

bool isCollide(Sprite &s1, Sprite &s2) {
    return s1.getGlobalBounds().intersects(s2.getGlobalBounds());
}

void changeDirection(Sprite &s1, Sprite &s2, Vector2u size1, float& vx, float& vy) {
    Vector2f pos1 = s1.getPosition();
    Vector2f pos2 = s2.getPosition();

    if (vy > 0) {
        if (pos1.y > pos2.y - size1.y - vy) vx *= -1;
        else vy *= -1;
    }
    else {
        if (pos1.y < pos2.y + size1.y + vy) vx *= -1;
        else vy *= -1;
    }
}

int main() {
    srand(time(0));

    Texture tile[4], board, ball;
    tile[0].loadFromFile("images/tile1.png");
    tile[1].loadFromFile("images/tile2.png");
    tile[2].loadFromFile("images/tile3.png");
    tile[3].loadFromFile("images/tile4.png");
    board.loadFromFile("images/board.png");
    ball.loadFromFile("images/ball.png");

    Vector2u boardSize = board.getSize();
    float boardWidth = boardSize.x;
    float boardHeight = boardSize.y;

    Vector2u ballSize = ball.getSize();
    float ballWidth = ballSize.x;
    float ballHeight = ballSize.y;

    Vector2u tileSize = tile[0].getSize();
    float tileWidth = tileSize.x;
    float tileHeight = tileSize.y;


    Sprite sBoard, sBall;
    sBoard.setTexture(board);
    sBall.setTexture(ball);

    std::fstream newfile;
    newfile.open("map.txt", std::ios::in);

    std::vector<std::string> Map;
    int width = 0;
    if (newfile.is_open()) {
        std::string line;
        while (std::getline(newfile, line)) {
            std::cout << line << std::endl;
            Map.push_back(line);
            if (line.length() > width)
                width = line.length();
        }
        newfile.close();
    }

    int height = Map.size();
    for (int i = 0; i < height; i++){
        for (int j = Map[i].length(); j < width; j++)
            Map[i] += ' ';
    }

    RenderWindow app(sf::VideoMode(tileWidth * width, 700), "Arkanoid");
    app.setFramerateLimit(60);

    Vector2u appSize = app.getSize();
    float appWidth = appSize.x;
    float appHeight = appSize.y;

    Sprite *blocks = new Sprite[height * width];
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            blocks[i * width + j].setTexture(tile[i % 4]);
        }
    }

    float vx = 7, vy = 7;
    bool play = false;
    int remains = 0;

    while (app.isOpen()) {
        sf::Event e;
        while (app.pollEvent(e)) {
            if (e.type == sf::Event::Closed)
                app.close();
        }

        if (play) {
            sBall.move(vx, vy);
            for (int i = 0; i < height; i++) {
                for (int j = 0; j < width; j++) {
                    if (isCollide(sBall, blocks[i * width + j])) {
                        changeDirection(sBall, blocks[i * width + j], ballSize, vx, vy);
                        blocks[i * width + j].setPosition(-100, 0);
                    }
                    if (blocks[i * width + j].getPosition().x >= 0)
                        remains++;
                }
            }

            if (remains == 0)
                play = false;
            remains = 0;

            Vector2f curPos = sBall.getPosition();
            if (curPos.x < 0) {
                vx *= -1;
                sBall.setPosition(ballWidth, curPos.y);
            }
            if (curPos.x > appWidth - ballWidth) {
                vx *= -1;
                sBall.setPosition(appWidth - ballWidth, curPos.y);
            }
            if (curPos.y < 0) {
                vy *= -1;
                sBall.setPosition(curPos.x, 0);
            }
            if (curPos.y > appHeight - ballHeight) {
                play = false;
            }

            if (Keyboard::isKeyPressed(Keyboard::Right)) {
                if (sBoard.getPosition().x + 9 <= appWidth - boardWidth)
                    sBoard.move(9, 0);
            }
            if (Keyboard::isKeyPressed(Keyboard::Left)) {
                if (sBoard.getPosition().x - 9 >= 0)
                    sBoard.move(-9, 0);
            }

            if (isCollide(sBall, sBoard)) {
                Vector2f boardPos = sBoard.getPosition();
                int dx = 1;
                if (curPos.x >= boardPos.x + boardWidth - ballWidth / 2 ||
                    curPos.x <= boardPos.x + ballWidth / 2) {
                    vy = -((rand() % 2) + 1);
                    if (curPos.x >= boardPos.x + boardWidth - ballWidth / 2) {
                        if (vx < 0) dx = -1;
                        else dx = 1;
                    }
                    else {
                        if (vx > 0) dx = -1;
                        else dx = 1;
                    }
                }
                else {
                    float gap = (boardWidth / 2) - abs(curPos.x - (boardPos.x + boardWidth / 2));
                    vy = -(8.0 / boardWidth * 2 * gap);
                }
                vx = dx * (vx / abs(vx)) * pow(98 - vy * vy, 0.5);
            }
        }
        else {
            sBoard.setPosition((appWidth - boardWidth) / 2, 700 - boardHeight);
            sBall.setPosition((appWidth - ballWidth) / 2, 700 - boardHeight - ballHeight);
            for (int i = 0; i < height; i++) {
                for (int j = 0; j < width; j++) {
                    if (Map[i][j] == '*') blocks[i * width + j].setPosition(j * tileWidth, i * tileHeight);
                    else blocks[i * width + j].setPosition(-100, 0);
                }
            }
            if (Keyboard::isKeyPressed(Keyboard::Right)) {
                vy = -(rand() % 4 + 4);
                vx = pow(98 - vy * vy, 0.5);
                play = true;
            }
            if (Keyboard::isKeyPressed(Keyboard::Left)) {
                vy = -(rand() % 4 + 4);
                vx = -pow(98 - vy * vy, 0.5);
                play = true;
            }
        }
        app.clear(Color(255, 255, 255));
        app.draw(sBall);
        app.draw(sBoard);
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                app.draw(blocks[i * width + j]);
            }
        }
        app.display();
    }
    return 0;
}