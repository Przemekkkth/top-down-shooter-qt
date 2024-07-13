#include "bullet.h"
#include "../utils.h"
#include <cmath>
#include <qdebug>

Bullet::Bullet(int _x, int _y, float _angle) {
    x = _x;
    y = _y;
    speed = 5;
    angle = _angle * (M_PI / 180);
    dead = false;
    width = 32;
    height = 32;
}

void Bullet::update(const float dt)
{
    x += cos(angle) * speed * dt;
    y += sin(angle) * speed * dt;

    if(x < -width) {
        dead = true;
    }
    else if(x + width > SCREEN::PHYSICAL_SIZE.width()) {
        dead = true;
    }
    else if(y < -height) {
        dead = true;
    }
    else if(y + height > SCREEN::PHYSICAL_SIZE.height()) {
        dead = true;
    }
}
