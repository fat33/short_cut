#ifndef GRID_H
#define GRID_H

#include "cellitem.h"

#include <qmap.h>
#include <qobject.h>

class Grid: public QObject
{
    Q_OBJECT
public:
    Grid();
public slots:
    void updateGrid(const QHash<QPair<int, int>, CellItem*> &);
    void setBegin(QPair<int, int>);
    void drawPatch(const QHash<QPair<int, int>, CellItem *> & , QPair<int, int>, QPair<int, int>);
    void clearPatch(const QHash<QPair<int, int>, CellItem*> &);
private:
    QHash<QPair<int, int>, QList< QPair<int, int> > > mapGrid;
    QHash<QPair<int, int>, QPair<int, int>> cameFrom;
};

#endif // GRID_H
