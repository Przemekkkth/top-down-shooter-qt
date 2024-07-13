#include "pixmapmanager.h"

PixmapManager* PixmapManager::ptr = nullptr;

PixmapManager *PixmapManager::Instance()
{
    if(!ptr)
    {
        ptr = new PixmapManager();
    }
    return ptr;
}

QPixmap& PixmapManager::getPixmap(TextureID id)
{
    return m_textures.get(id);
}

QPixmap &PixmapManager::getBG()
{
    return getPixmap(TextureID::BG);
}

QPixmap &PixmapManager::getBullet()
{
    return getPixmap(TextureID::Bullet);
}

QPixmap &PixmapManager::getPlayer()
{
    return getPixmap(TextureID::Player);
}

QPixmap &PixmapManager::getZombie()
{
    return getPixmap(TextureID::Zombie);
}

QPixmap &PixmapManager::getCrosshair()
{
    return getPixmap(TextureID::Crosshair);
}

PixmapManager::PixmapManager()
{
    m_textures.load(TextureID::BG, QStringLiteral(":/assets/background.png"));
    m_textures.load(TextureID::Player, QStringLiteral(":/assets/player.png"));
    m_textures.load(TextureID::Bullet, QStringLiteral(":/assets/bullet.png"));
    m_textures.load(TextureID::Zombie, QStringLiteral(":/assets/zombie.png"));
    m_textures.load(TextureID::Crosshair, QStringLiteral(":/assets/crosshair.png"));
}
