#include "gamescene.h"
#include "utils.h"
#include "pixmapmanager.h"
#include "fontmanager.h"
#include <QKeyEvent>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsPixmapItem>
#include <QGraphicsLineItem>
#include <QDebug>
#include <QDir>
#include <QPainter>
#include <QCursor>
#include <QRandomGenerator>

GameScene::GameScene(QObject *parent)
    : QGraphicsScene(parent)
{
    this->installEventFilter(this);
    onUserCreate();
}

GameScene::~GameScene()
{

}

float GameScene::distanceBetween(Player *player, Zombie *zombie)
{
    return std::sqrt(std::pow(zombie->x - player->x, 2) + std::pow(zombie->y - player->y, 2) );
}

float GameScene::distanceBetween(Bullet *bullet, Zombie *zombie)
{
    return std::sqrt(std::pow(zombie->x - bullet->x, 2) + std::pow(zombie->y - bullet->y, 2) );
}

bool GameScene::isPlayerCollidedWithZombie(float distance)
{
    for (int i = 0; i < zombies.size(); ++i) {
        if (distanceBetween(&player, &zombies[i]) < distance) {
            score = 0;
            player.x = SCREEN::PHYSICAL_SIZE.width() / 2;
            player.y = SCREEN::PHYSICAL_SIZE.height() / 2;
            zombieTimer = 0.0f;
            zombies.clear();
            return true;
        }
    }
    return false;
}

bool GameScene::isBulletCollidedWithBullet(float distance)
{
    for (int i = bullets.size() - 1; i >= 0; --i) {
        for (int j = zombies.size() - 1; j >= 0; --j)
        {
            if (distanceBetween(&bullets[i], &zombies[j]) < distance && !bullets[i].dead && !zombies[j].dead) {
                score++;
                bullets[i].dead = true;
                zombies[j].dead = true;
                return true;
            }
        }
    }

    return false;
}

void GameScene::onUserUpdate(const float dt)
{
    handlePlayerInput();
    player.move();

    isPlayerCollidedWithZombie(30.0);
    isBulletCollidedWithBullet(30.0);

    for (int i = bullets.size() - 1; i >= 0; --i) {
        if(bullets[i].dead) {
            bullets.remove(i);
        }
        else {
            bullets[i].update(1.0);
        }
    }

    for (int i = zombies.size() - 1; i >= 0; --i) {
        if(zombies[i].dead) {
            zombies.remove(i);
        }
        else {
            zombies[i].update(dt, &player);
        }
    }

    spawnZombie(dt);
    clear();
    draw();
}

void GameScene::loop()
{
    m_deltaTime = m_elapsedTimer.elapsed();
    m_elapsedTimer.restart();

    m_loopTime += m_deltaTime;
    while(m_loopTime > m_loopSpeed)
    {
        m_loopTime -= m_loopSpeed;
        onUserUpdate(m_loopSpeed);
        resetStatus();
    }
}

void GameScene::onUserCreate()
{
    for(int i = 0; i < 256; ++i)
    {
        m_keys[i] = new KeyStatus();
    }
    m_mouse = new MouseStatus();
    setSceneRect(0,0, SCREEN::PHYSICAL_SIZE.width(), SCREEN::PHYSICAL_SIZE.height());
    connect(&m_timer, &QTimer::timeout, this, &GameScene::loop);
    m_timer.start(int(1000.0f/FPS));
    m_elapsedTimer.start();
    zombieTimer = 0.0f;
    score = 0;
}

void GameScene::renderScene()
{
    static int index = 0;
    QString fileName = QDir::currentPath() + QDir::separator() + "screen" + QString::number(index++) + ".png";
    QRect rect = sceneRect().toAlignedRect();
    QImage image(rect.size(), QImage::Format_ARGB32);
    image.fill(Qt::transparent);
    QPainter painter(&image);
    render(&painter);
    image.save(fileName);
    qDebug() << "saved " << fileName;
}

void GameScene::handlePlayerInput()
{
    if(m_keys[KEYBOARD::KeysMapper[Qt::Key_Z]]->m_released)
    {
        renderScene();//uncoment if want to make screenshots
    }

    if (m_mouse->m_released) {
        bullets.push_back(Bullet(player.x - player.width / 2,
                                 player.y,
                                 player.toMouseAngle(QPoint(m_mouse->m_x, m_mouse->m_y))));
    }
}

void GameScene::draw()
{
    addItem(new QGraphicsPixmapItem(PixmapManager::Instance()->getBG()));

    QGraphicsPixmapItem* pItem = new QGraphicsPixmapItem(PixmapManager::Instance()->getPlayer());
    pItem->setPos(player.x, player.y);
    pItem->setTransformOriginPoint(pItem->boundingRect().center());
    pItem->setRotation(player.toMouseAngle(QPoint(m_mouse->m_x, m_mouse->m_y)));
    addItem(pItem);

    foreach(Bullet bullet, bullets) {
        QGraphicsPixmapItem *bItem = new QGraphicsPixmapItem(PixmapManager::Instance()->getBullet());
        bItem->setPos(bullet.x + bullet.width / 2, bullet.y + bullet.height / 2);
        addItem(bItem);
    }

    for (int i = 0; i < zombies.size(); ++i) {
        QGraphicsPixmapItem* zItem = new QGraphicsPixmapItem(PixmapManager::Instance()->getZombie());
        zItem->setPos(zombies[i].x, zombies[i].y);
        zItem->setTransformOriginPoint(zItem->boundingRect().center());
        zItem->setRotation(zombies[i].angle * (180.0 / M_PI));
        addItem(zItem);
    }

    drawStat();
    drawCrosshair();
}

void GameScene::drawStat()
{
    QGraphicsSimpleTextItem *tItem = new QGraphicsSimpleTextItem();
    QFont mainFont = FontManager::Instance()->getMainFont();
    mainFont.setPixelSize(30);
    tItem->setFont(mainFont);
    tItem->setPen(QPen(QColor(Qt::red)));
    tItem->setBrush(QBrush(QColor(Qt::red)));
    tItem->setText(QString("Score: ") + QString::number(score));
    tItem->setTransformOriginPoint(tItem->boundingRect().topRight());
    tItem->setPos(SCREEN::PHYSICAL_SIZE.width() - tItem->boundingRect().width() - 10, 10);
    addItem(tItem);
}

void GameScene::drawCrosshair()
{
    QGraphicsPixmapItem *cItem = new QGraphicsPixmapItem();
    cItem->setPixmap(PixmapManager::Instance()->getCrosshair());
    cItem->setPos(m_mouse->m_x - cItem->boundingRect().width() / 2, m_mouse->m_y - cItem->boundingRect().height() / 2);
    addItem(cItem);
}

void GameScene::resetStatus()
{
    for(int i = 0; i < 256; ++i)
    {
        m_keys[i]->m_released = false;
    }
    for(int i = 0; i < 256; ++i)
    {
        m_keys[i]->m_pressed = false;
    }
    m_mouse->m_released = false;
}

void GameScene::spawnZombie(const float dt)
{
    zombieTimer += dt;
    if (zombieTimer > 750) {
        zombieTimer = 0.0f;
        int side = QRandomGenerator::global()->bounded(4);
        int hRandomVal = QRandomGenerator::global()->bounded(SCREEN::PHYSICAL_SIZE.height());
        int wRandomVal = QRandomGenerator::global()->bounded(SCREEN::PHYSICAL_SIZE.width());

        switch (side) {
            case 0: // left
                zombies.push_back(Zombie(-50, hRandomVal));
            break;
            case 1: // right
                zombies.push_back(Zombie(SCREEN::PHYSICAL_SIZE.width() + 50, hRandomVal));
            break;
            case 2: // up
                zombies.push_back(Zombie(wRandomVal, -50));
            break;
            case 3: // down
                zombies.push_back(Zombie(wRandomVal, SCREEN::PHYSICAL_SIZE.height() + 50));
            break;
        }
    }
}

void GameScene::keyPressEvent(QKeyEvent *event)
{
    if(KEYBOARD::KeysMapper.contains(event->key()))
    {
        if(event->isAutoRepeat())
        {
            m_keys[KEYBOARD::KeysMapper[event->key()]]->m_held = true;
            m_keys[KEYBOARD::KeysMapper[event->key()]]->m_pressed = false;
        }
        else
        {
            m_keys[KEYBOARD::KeysMapper[event->key()]]->m_pressed = true;
            m_keys[KEYBOARD::KeysMapper[event->key()]]->m_held    = false;
        }
    }
    QGraphicsScene::keyPressEvent(event);
}

void GameScene::keyReleaseEvent(QKeyEvent *event)
{
    if(KEYBOARD::KeysMapper.contains(event->key()))
    {
        if(!event->isAutoRepeat())
        {
            m_keys[KEYBOARD::KeysMapper[event->key()]]->m_held = false;
            m_keys[KEYBOARD::KeysMapper[event->key()]]->m_pressed = false;
            m_keys[KEYBOARD::KeysMapper[event->key()]]->m_released = true;
        }

    }
    QGraphicsScene::keyReleaseEvent(event);
}

void GameScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    m_mouse->m_x = event->scenePos().x();
    m_mouse->m_y = event->scenePos().y();
    m_mouse->m_pressed = true;
    QGraphicsScene::mousePressEvent(event);
}

void GameScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    m_mouse->m_x = event->scenePos().x();
    m_mouse->m_y = event->scenePos().y();
    QGraphicsScene::mouseMoveEvent(event);
}

void GameScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    m_mouse->m_x = event->scenePos().x();
    m_mouse->m_y = event->scenePos().y();
    m_mouse->m_pressed = false;
    m_mouse->m_released = true;
    QGraphicsScene::mouseReleaseEvent(event);
}

bool GameScene::eventFilter(QObject *watched, QEvent *event)
{
    if(event->type()==QEvent::KeyPress) {

        pressedKeys += ((QKeyEvent*)event)->key();

        // if( pressedKeys.contains(Qt::Key_Up) && pressedKeys.contains(Qt::Key_Left) )
        // {
        //     player.moveLeft();
        //     player.moveUp();
        // }
        // else if( pressedKeys.contains(Qt::Key_Up))
        // {
        //     player.moveUp();
        // }

    }
    else if(event->type()==QEvent::KeyRelease)
    {

        pressedKeys -= ((QKeyEvent*)event)->key();
    }

    if( (pressedKeys.contains(Qt::Key_Up) && pressedKeys.contains(Qt::Key_Left)) || (pressedKeys.contains(Qt::Key_W) && pressedKeys.contains(Qt::Key_A))  )
    {
        player.setDirection(QStringLiteral("up-left"));
    }
    else if( (pressedKeys.contains(Qt::Key_Up) && pressedKeys.contains(Qt::Key_Right)) || (pressedKeys.contains(Qt::Key_W) && pressedKeys.contains(Qt::Key_D)) )
    {
        player.setDirection(QStringLiteral("up-right"));
    }
    else if( (pressedKeys.contains(Qt::Key_Down) && pressedKeys.contains(Qt::Key_Left)) || (pressedKeys.contains(Qt::Key_S) && pressedKeys.contains(Qt::Key_A)) )
    {
        player.setDirection(QStringLiteral("down-left"));
    }
    else if( (pressedKeys.contains(Qt::Key_Down) && pressedKeys.contains(Qt::Key_Right)) || (pressedKeys.contains(Qt::Key_S) && pressedKeys.contains(Qt::Key_D)) )
    {
        player.setDirection(QStringLiteral("down-right"));
    }
    else if( pressedKeys.contains(Qt::Key_Up) || pressedKeys.contains(Qt::Key_W))
    {
        player.setDirection(QStringLiteral("up"));
    }
    else if( pressedKeys.contains(Qt::Key_Down) || pressedKeys.contains(Qt::Key_S))
    {
        player.setDirection(QStringLiteral("down"));
    }
    else if( pressedKeys.contains(Qt::Key_Left) || pressedKeys.contains(Qt::Key_A))
    {
        player.setDirection(QStringLiteral("left"));
    }
    else if( pressedKeys.contains(Qt::Key_Right) || pressedKeys.contains(Qt::Key_D))
    {
        player.setDirection(QStringLiteral("right"));
    }
    else {
        player.setDirection(QStringLiteral("stop"));
    }


    return false;
    //return GameScene::eventFilter(watched, event);
}
