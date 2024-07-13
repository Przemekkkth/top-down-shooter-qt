#ifndef ZOMBIE_H
#define ZOMBIE_H
class Player;

class Zombie
{
public:
    Zombie(int _x, int _y);
    void update(const float dt, Player *player);

    float x, y, angle;
    int width, height;
    float speed;
    bool dead;
};

#endif // ZOMBIE_H
