#include <SFML/Graphics.hpp>

using namespace sf;

bool isCollide(Sprite s1, Sprite s2) {
    return s1.getGlobalBounds().intersects(s2.getGlobalBounds());
}

int main(){
    RenderWindow app(sf::VideoMode(1000, 700), "Arkanoid");
    app.setFramerateLimit(60);

    Texture tile[4], board, ball;
    tile[1].loadFromFile("images/tile1.png");
    tile[2].loadFromFile("images/tile2.png");
    tile[3].loadFromFile("images/tile3.png");
    tile[4].loadFromFile("images/tile4.png");
    board.loadFromFile("images/board.png");
    ball.loadFromFile("images/ball.png");

    Sprite sBoard(board), sBall(ball);
    sBoard.setPosition(500, 690);
    sBall.setPosition(500, 675);

    Sprite blocks[4][20];
    for(int i=0; i<4; i++) {
        for (int j = 0; j < 20; j++) {
            blocks[i][j].setTexture(tile[i]);
            blocks[i][j].setPosition(j * 60, i * 20);
        }
    }

    int vx = 5, vy = 5;

    while(app.isOpen()){
        sf::Event e;
        while(app.pollEvent(e)){
            if (e.type == sf::Event::Closed)
                app.close();
        }

        sBall.move(vx, 0);
        for (int i = 0; i < 4; i++){
            for (int j = 0; j < 80; j++){
                if (isCollide(sBall, blocks[i][j])){
                    blocks[i][j].setPosition(-100, 0);
                    vx *= -1;
                }
            }
        }

        sBall.move(0, vy);
        for (int i = 0; i < 4; i++){
            for (int j = 0; j < 80; j++){
                if (isCollide(sBall, blocks[i][j])){
                    blocks[i][j].setPosition(-100, 0);
                    vy *= -1;
                }
            }
        }

        Vector2f curPos = sBall.getPosition();
        if (curPos.x < 0 || curPos.x > 1000) vx *= -1;
        if (curPos.y < 0) vy *= -1;
        if (curPos.y > 700) {
            vx = 0;
            vy = 0;
        }

        if (Keyboard::isKeyPressed(Keyboard::Right)) sBoard.move(7, 0);
        if (Keyboard::isKeyPressed(Keyboard::Left)) sBoard.move(-7, 0);

        if (isCollide(sBall, sBoard)) vy *= -1;
        app.clear();
        app.draw(sBall);
        app.draw(sBoard);
        for(int i=0; i<4; i++) {
            for (int j = 0; j < 20; j++) {
                app.draw(blocks[i][j]);
            }
        }
        app.display();
    }
    return 0;
}