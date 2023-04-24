#include "cellitem.h"
#include "cgraphicsview.h"
#include <QWheelEvent>

CGraphicsView::CGraphicsView(QWidget *parent, QGraphicsScene *scene)
    : QGraphicsView(scene, parent)
{
    setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
}

void CGraphicsView::wheelEvent(QWheelEvent *e)
{
    if (e->modifiers() & Qt::ControlModifier) {
        if (e->delta() > 0)
            this->zoomIn();
        else
            this->zoomOut();
        e->accept();
    } else {
        QGraphicsView::wheelEvent(e);
    }
}

void CGraphicsView::mouseMoveEvent(QMouseEvent *e)
{
    if(QGraphicsScene * scena = scene())
    {
        QList<QGraphicsItem *> items = scena->items(mapToScene(e->pos()));
        for(QGraphicsItem *item : items)
        {
            if(CellItem * cell = dynamic_cast<CellItem *>(item))
            {
                emit mouseMoveBuildPuth(cell->getCoord().first, cell->getCoord().second);
            }
        }
    }
}

void CGraphicsView::zoomIn()
{
    scale(1.1, 1.1);
}

void CGraphicsView::zoomOut()
{
    scale(1/1.1, 1/1.1);
}
