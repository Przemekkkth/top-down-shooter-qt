#include "zombie.h"
#include "player.h"
#include <cmath>
#include <qdebug>

Zombie::Zombie(int _x, int _y)
{
    x = _x;
    y = _y;
    width = 35;
    height = 45;
    angle = 0.0f;
    speed = .10f;
    dead = false;
}

void Zombie::update(const float dt, Player *player)
{
    angle = std::atan2(player->y - y, player->x - x);

    x += cos(angle) * speed * dt;
    y += sin(angle) * speed * dt;
}
