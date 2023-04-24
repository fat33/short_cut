#include "grid.h"
#include <QQueue>
#include <qdebug.h>

Grid::Grid()
{

}

void Grid::updateGrid(const QHash<QPair<int, int>, CellItem *> & mapCell)
{
    QHashIterator<QPair<int, int>, CellItem*> i(mapCell);
    while (i.hasNext()) {
        i.next();
        QPair<int, int> coord = i.key();
        CellItem * cell = i.value();
        QList< QPair<int, int> > arcs;
        if(cell->getCondition() != 3)
        {
            QPair<int, int> neighbor = qMakePair(coord.first+1, coord.second);
            if(mapCell.contains(neighbor) && mapCell[neighbor]->getCondition() != 3)
                arcs.append(neighbor);

            neighbor = qMakePair(coord.first-1, coord.second);
            if(mapCell.contains(neighbor) && mapCell[neighbor]->getCondition() != 3)
                arcs.append(neighbor);

            neighbor = qMakePair(coord.first, coord.second+1);
            if(mapCell.contains(neighbor) && mapCell[neighbor]->getCondition() != 3)
                arcs.append(neighbor);

            neighbor = qMakePair(coord.first, coord.second-1);
            if(mapCell.contains(neighbor) && mapCell[neighbor]->getCondition() != 3)
                arcs.append(neighbor);

            if(!arcs.isEmpty())
                mapGrid[coord] = arcs;
        }
    }
}

void Grid::setBegin(QPair<int, int> begin)
{
    QQueue<QPair<int, int>> frontier;
    frontier.enqueue(begin);

    cameFrom.clear();
    cameFrom[begin] = begin;

    while (!frontier.isEmpty()) {
        QPair<int, int> current = frontier.first();
        frontier.removeFirst();

        for (auto& next : mapGrid.value(current)) {
            if (!cameFrom.count(next)) {
                frontier.enqueue(next);
                cameFrom[next] = current;
            }
        }
    }
}

void Grid::drawPatch(const QHash<QPair<int, int>, CellItem *> & mapCell,
                     QPair<int, int>begin, QPair<int, int>end)
{
    if(cameFrom.isEmpty()) return;
    clearPatch(mapCell);
    QPair<int, int> next, current = end;
    while(current != begin)
    {
        if(cameFrom.contains(current) && current != begin)
        {
            next = cameFrom.value(current);
            if(mapCell.contains(next) && next != begin)
            {
                CellItem * cell = mapCell.value(next);
                cell->setCondition(4);
                cell->update();
            }
            current = next;
        }
        else
        {
            return;
        }
    }
}

void Grid::clearPatch(const QHash<QPair<int, int>, CellItem *> & mapCell)
{
    QHashIterator<QPair<int, int>, CellItem*> i(mapCell);
    while (i.hasNext()) {
        i.next();
        CellItem * cell = i.value();
        if(cell->getCondition() == 4)
        {
            cell->setCondition(0);
            cell->update();
        }
    }
}
