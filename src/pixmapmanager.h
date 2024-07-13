#ifndef PIXMAPMANAGER_H
#define PIXMAPMANAGER_H


#include "resourceholder.h"
#include <QPixmap>

class PixmapManager
{
public:
    enum TextureID
    {
        BG, Bullet, Player, Zombie, Crosshair
    };
    static PixmapManager* Instance();
    QPixmap& getPixmap(TextureID id);
    QPixmap& getBG();
    QPixmap& getBullet();
    QPixmap& getPlayer();
    QPixmap& getZombie();
    QPixmap& getCrosshair();
private:
    PixmapManager();
    PixmapManager(PixmapManager& other) = delete;
    void operator=(const PixmapManager &) = delete;

    ResourceHolder<QPixmap, TextureID> m_textures;
    static PixmapManager* ptr;
};

#endif // PIXMAPMANAGER_H
