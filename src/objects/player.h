#ifndef PLAYER_H
#define PLAYER_H
#include <QString>
#include <QPoint>

class GameScene;
class Player
{
public:
    Player();
    double toMouseAngle(QPoint mousePos);
    void setDirection(const QString& newDirection);
    void draw(GameScene& gs);
    void moveLeft();
    void moveRight();
    void moveUp();
    void moveDown();
    void move();

    int x, y, speed;
    int width, height;
    QString direction;
};

#endif // PLAYER_H
