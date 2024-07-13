#ifndef GAMESCENE_H
#define GAMESCENE_H

#include <QGraphicsScene>
#include <QElapsedTimer>
#include <QTimer>
#include <QImage>
#include <QGraphicsSimpleTextItem>
#include <QVector>
#include "objects/player.h"
#include "objects/bullet.h"
#include "objects/zombie.h"

struct KeyStatus
{
    bool m_pressed = false;
    bool m_held = false;
    bool m_released = false;
};

struct MouseStatus
{
    float m_x = 0.0f;
    float m_y = 0.0f;
    bool m_released = false;
    bool m_pressed = false;
};

class GameScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit GameScene(QObject *parent = nullptr);
    ~GameScene();

    static float distanceBetween(Player* player, Zombie* zombie);
    static float distanceBetween(Bullet *bullet, Zombie *zombie);
    bool isPlayerCollidedWithZombie(float distance);
    bool isBulletCollidedWithBullet(float distance);
private slots:
    void loop();

private:
    void onUserCreate();
    void onUserUpdate(const float dt);
    void renderScene();
    void handlePlayerInput();
    void draw();
    void drawStat();
    void drawCrosshair();
    void resetStatus();
    void spawnZombie(const float dt);

    Player player;
    QSet<int> pressedKeys;
    QVector<Bullet> bullets;
    QVector<Zombie> zombies;
    float zombieTimer;
    int score;

    KeyStatus* m_keys[256];
    MouseStatus* m_mouse;
    const int FPS = 60;
    QTimer m_timer;
    QElapsedTimer m_elapsedTimer;
    float m_deltaTime = 0.0f, m_loopTime = 0.0f;
    const float m_loopSpeed = int(1000.0f/FPS);
protected:
    virtual void keyPressEvent(QKeyEvent *event) override;
    virtual void keyReleaseEvent(QKeyEvent *event) override;
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    virtual bool eventFilter(QObject *watched, QEvent *event) override;
};


#endif // GAMESCENE_H
