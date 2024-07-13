#ifndef BULLET_H
#define BULLET_H

class Bullet
{
public:
    Bullet(int _x, int _y, float _angle);
    void update(const float dt);

    float x, y, speed;
    int width, height;
    bool dead;
    float angle;
};

#endif // BULLET_H
