#include "player.h"
#include "../utils.h"
#include <QDebug>
#include <QCursor>

Player::Player() {
    width = 35;
    height = 45;
    x = (SCREEN::PHYSICAL_SIZE.width() / 2) - (width / 2);
    y = (SCREEN::PHYSICAL_SIZE.height() / 2) - (height / 2);
    speed = 5;
    direction = QStringLiteral("stop");
}

void Player::moveLeft()
{
    x -= speed;
    if (x < 0) {
        x = 0;
    }
}

void Player::moveRight()
{
    x += speed;
    if ( x + width > SCREEN::PHYSICAL_SIZE.width()) {
        x = SCREEN::PHYSICAL_SIZE.width() - width;
    }
}

void Player::moveUp()
{
    y -= speed;
    if (y < 0) {
        y = 0;
    }
}

void Player::moveDown()
{
    y += speed;
    if ( y + height > SCREEN::PHYSICAL_SIZE.height()) {
        y = SCREEN::PHYSICAL_SIZE.height() - height;
    }
}

void Player::move()
{
    if (direction == QStringLiteral("up-left")) {
        moveUp();
        moveLeft();
    }
    else if (direction == QStringLiteral("up-right")) {
        moveUp();
        moveRight();
    }
    else if (direction == QStringLiteral("down-left")) {
        moveDown();
        moveLeft();
    }
    else if (direction == QStringLiteral("down-right")) {
        moveDown();
        moveRight();
    }
    else if (direction == QStringLiteral("up")) {
        moveUp();
    }
    else if (direction == QStringLiteral("down")) {
        moveDown();
    }
    else if (direction == QStringLiteral("left")) {
        moveLeft();
    }
    else if (direction == QStringLiteral("right")) {
        moveRight();
    }
    else if (direction == QStringLiteral("stop")) {

    }
    else {
        qDebug() << "Invalid movement";
    }
}

double Player::toMouseAngle(QPoint mousePos)
{
    double angleRadians = std::atan2(mousePos.y() - y, mousePos.x() - x);
    double angleDegrees = angleRadians * (180.0 / M_PI);
    return angleDegrees;
}

void Player::setDirection(const QString& newDirection)
{
    direction = newDirection;
}
