#include "SFML/Graphics.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <time.h>
#include <math.h>
#include <ctime>

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

void updateRecord(std::fstream &records, int blockCount, int remains) {
    records.open("records.txt", std::ios::in);

    std::vector<std::string> recordHistory;
    if (records.is_open()) {
        std::string line;
        int cnt = 0;
        while (std::getline(records, line) && cnt++ < 10) {
            recordHistory.push_back(line);
        }
        records.close();
    }

    records.open("records.txt", std::ios::out);

    time_t rawtime;
    struct tm timeinfo;
    time(&rawtime);
    localtime_s(&timeinfo, &rawtime);

    std::string day = "", mon = "", year = "", hour = "", min = "", sec = "";
    if (timeinfo.tm_mday < 10)
        day += '0';
    if (timeinfo.tm_mon + 1 < 10)
        mon += '0';
    if (timeinfo.tm_hour < 10)
        hour += '0';
    if (timeinfo.tm_min < 10)
        min += '0';
    if (timeinfo.tm_sec < 10)
        sec += '0';
    day += std::to_string(timeinfo.tm_mday);
    mon += std::to_string(timeinfo.tm_mon + 1);
    year += std::to_string(timeinfo.tm_year + 1900);
    hour += std::to_string(timeinfo.tm_hour);
    min += std::to_string(timeinfo.tm_min);
    sec += std::to_string(timeinfo.tm_sec);
    records << day << "/" << mon << "/" << year <<
        "    " << hour << ":" << min << ":" << sec <<
        "    " << blockCount - remains << std::endl;

    for (int i = 0; i < recordHistory.size(); i++)
        records << recordHistory[i] << std::endl;
    records.close();
}

int main() {
    srand(time(0));

    RenderWindow menu(VideoMode(300, 300), "Arkanoid");

    Font font;
    if (!font.loadFromFile("serif.ttf"))
        std::cout << "Failed to load font!" << std::endl;

    Text startGame, toRecord;
    startGame.setFont(font);
    startGame.setFillColor(Color::Black);
    startGame.setCharacterSize(30);
    startGame.setString("Start Game");
    startGame.setPosition(75, 75);
    //std::cout << startGame.getLocalBounds().width << " " << startGame.getLocalBounds().height << std::endl;

    toRecord.setFont(font);
    toRecord.setFillColor(Color::Black);
    toRecord.setCharacterSize(30);
    toRecord.setString("Record");
    toRecord.setPosition(104, 150);
    //std::cout << toRecord.getLocalBounds().width << " " << toRecord.getLocalBounds().height << std::endl;

    //151 23
    //92 21

    Texture border;
    border.loadFromFile("images/border.png");
    Sprite sBorder1(border), sBorder2(border);
    sBorder1.setPosition(65, 68);
    sBorder2.setPosition(65, 143);

    bool isStarted = false;
    bool recordClicked = false;
    while (menu.isOpen()) {
        Event e;
        while (menu.pollEvent(e)) {
            if (e.type == Event::Closed)
                menu.close();
        }

        if (Mouse::isButtonPressed(Mouse::Left)) {
            auto mousePos = Mouse::getPosition(menu);
            auto translatedPos = menu.mapPixelToCoords(mousePos);

            if (sBorder1.getGlobalBounds().contains(translatedPos)) {
                isStarted = true;
            }

            if (sBorder2.getGlobalBounds().contains(translatedPos)) {
                RenderWindow Records(VideoMode(300, 500), "Records");

                std::fstream recordFile;
                recordFile.open("records.txt", std::ios::in);
                std::vector<Text> records;
                int recordCount = 0;

                if (recordFile.is_open()) {
                    std::string line;
                    while (std::getline(recordFile, line) && recordCount < 10) {
                        Text record;
                        record.setFont(font);
                        record.setFillColor(Color::Black);
                        record.setCharacterSize(18);
                        record.setString(line);
                        record.setPosition(20, 20 + 50 * recordCount++);
                        records.push_back(record);
                    }
                    recordFile.close();
                }
                while (Records.isOpen()) {
                    Event e;
                    while (Records.pollEvent(e)) {
                        if (e.type == Event::Closed)
                            Records.close();
                    }

                    Records.clear(Color(255, 255, 255));
                    for (int i = 0; i < recordCount; i++)
                        Records.draw(records[i]);
                    Records.display();
                }
            }
        }
        menu.clear(Color(255, 255, 255));
        menu.draw(sBorder1);
        menu.draw(startGame);
        menu.draw(sBorder2);
        menu.draw(toRecord);
        menu.display();

        if (isStarted) {

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
            for (int i = 0; i < height; i++) {
                for (int j = Map[i].length(); j < width; j++)
                    Map[i] += ' ';
            }

            RenderWindow app(VideoMode(tileWidth * width, 700), "Arkanoid");
            app.setFramerateLimit(60);

            Vector2u appSize = app.getSize();
            float appWidth = appSize.x;
            float appHeight = appSize.y;

            int remains = 0;
            Sprite* blocks = new Sprite[height * width];
            for (int i = 0; i < height; i++) {
                for (int j = 0; j < width; j++) {
                    if (Map[i][j] == '*')
                        remains++;
                    blocks[i * width + j].setTexture(tile[i % 4]);
                }
            }

            float vx = 7, vy = 7;
            int speed = 10;
            bool play = false;
            int result = 0;
            int hearts = 3;
            int levelChanger = 0;
            int blockCount = remains;

            Text message;
            message.setFont(font);
            message.setCharacterSize(50);

            Text again;
            again.setFont(font);
            again.setCharacterSize(30);
            again.setFillColor(Color(0, 0, 0));
            again.setString("Try again");
            again.setPosition((appWidth - again.getLocalBounds().width) / 2, 400);

            Text toMenu;
            toMenu.setFont(font);
            toMenu.setCharacterSize(30);
            toMenu.setFillColor(Color(0, 0, 0));
            toMenu.setString("Main Menu");
            toMenu.setPosition((appWidth - toMenu.getLocalBounds().width) / 2, 470);

            Sprite sBorder3(border), sBorder4(border);
            sBorder3.setPosition((appWidth - sBorder3.getLocalBounds().width) / 2, 395);
            sBorder4.setPosition((appWidth - sBorder4.getLocalBounds().width) / 2, 465);

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
                                levelChanger++;
                                if (levelChanger == 5) {
                                    speed = ((speed + 2) % 21) > 10 ? ((speed + 2) % 20) : 10;
                                    levelChanger = 0;
                                }
                                remains--;
                            }
                        }
                    }

                    if (remains == 0) {
                        play = false;
                        result = 1;
                        message.setFillColor(Color(255, 0, 0));
                        message.setString("Congratulations!");
                        message.setPosition((appWidth - message.getLocalBounds().width) / 2, 300);

                        std::fstream records;
                        updateRecord(records, blockCount, remains);
                    }

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
                        hearts--;
                        if (hearts == 0) {
                            play = false;
                            result = -1;
                            message.setFillColor(Color(0, 0, 255));
                            message.setString("You Lose!");
                            message.setPosition((appWidth - message.getLocalBounds().width) / 2, 300);

                            std::fstream records;
                            updateRecord(records, blockCount, remains);
                        }
                        else {
                            sBoard.setPosition((appWidth - boardWidth) / 2, 700 - boardHeight);
                            sBall.setPosition((appWidth - ballWidth) / 2, 700 - boardHeight - ballHeight);
                            vx = 0;
                            vy = 0;
                            speed = 10;
                        }
                    }

                    if (Keyboard::isKeyPressed(Keyboard::Right)) {
                        if (sBoard.getPosition().x + 9 <= appWidth - boardWidth)
                            sBoard.move(9, 0);
                        if (vx == 0 && vy == 0) {
                            vy = -(rand() % 4 + speed / 2);
                            vx = pow(speed * speed - vy * vy, 0.5);
                        }
                    }
                    if (Keyboard::isKeyPressed(Keyboard::Left)) {
                        if (sBoard.getPosition().x - 9 >= 0)
                            sBoard.move(-9, 0);
                        if (vx == 0 && vy == 0) {
                            vy = -(rand() % 4 + speed / 2);
                            vx = -pow(speed * speed - vy * vy, 0.5);
                        }
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
                            if (Keyboard::isKeyPressed(Keyboard::Right) || Keyboard::isKeyPressed(Keyboard::Left))
                                vy = speed - 0.3;
                            vy *= -1;
                        }
                        vx = dx * (vx / abs(vx)) * pow(speed * speed - vy * vy, 0.5);
                    }
                }
                else {
                    if (Mouse::isButtonPressed(Mouse::Left)) {
                        auto mousePos = Mouse::getPosition(app);
                        auto translatedPos = app.mapPixelToCoords(mousePos);

                        if (sBorder3.getGlobalBounds().contains(translatedPos)) {
                            speed = 10;
                            hearts = 3;
                            result = 0;
                        }

                        if (sBorder4.getGlobalBounds().contains(translatedPos)) {
                            isStarted = false;
                            app.close();
                        }
                    }
                    if (result == 0) {
                        sBoard.setPosition((appWidth - boardWidth) / 2, 700 - boardHeight);
                        sBall.setPosition((appWidth - ballWidth) / 2, 700 - boardHeight - ballHeight);
                        for (int i = 0; i < height; i++) {
                            for (int j = 0; j < width; j++) {
                                if (Map[i][j] == '*') {
                                    blocks[i * width + j].setPosition(j * tileWidth, i * tileHeight);
                                }
                                else blocks[i * width + j].setPosition(-100, 0);
                            }
                        }
                        vx = 0;
                        vy = 0;
                        if (Keyboard::isKeyPressed(Keyboard::Right)) {
                            vy = -(rand() % 4 + speed / 2);
                            vx = pow(speed * speed - vy * vy, 0.5);
                            play = true;
                        }
                        if (Keyboard::isKeyPressed(Keyboard::Left)) {
                            vy = -(rand() % 4 + speed / 2);
                            vx = -pow(speed * speed - vy * vy, 0.5);
                            play = true;
                        }
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
                if (result != 0) {
                    app.draw(message);
                    app.draw(sBorder3);
                    app.draw(again);
                    app.draw(sBorder4);
                    app.draw(toMenu);
                }
                app.display();
            }
        }
    }
    return 0;
}
