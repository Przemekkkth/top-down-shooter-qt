#include <QApplication>
#include "view.h"

int main(int argc, char **argv)
{
    QApplication a(argc, argv);
    QCursor cursor(Qt::BlankCursor);
    QApplication::setOverrideCursor(cursor);
    QApplication::changeOverrideCursor(cursor);
    View v;
    v.setWindowTitle(QStringLiteral("Top Down Shooter Qt/C++"));
    v.show();
    a.exec();
}
