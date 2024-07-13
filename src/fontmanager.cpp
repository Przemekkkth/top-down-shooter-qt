#include "fontmanager.h"
#include <QFontDatabase>

FontManager::FontManager()
{
    int id = QFontDatabase::addApplicationFont(QStringLiteral(":/assets/juniory.ttf"));
    QString family = QFontDatabase::applicationFontFamilies(id).at(0);
    QFont stringFont(family);
    m_fontMap[FontID::MAIN] = stringFont;
}

FontManager* FontManager::ptr = nullptr;

FontManager *FontManager::Instance()
{
    if(!ptr)
    {
        ptr = new FontManager();
    }
    return ptr;
}

QFont FontManager::getFont(FontID id) const
{
    return m_fontMap[id];
}

QFont FontManager::getMainFont() const
{
    return getFont(FontID::MAIN);
}
