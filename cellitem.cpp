#include "cellitem.h"

#include <QMenu>
#include <QColor>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsSceneContextMenuEvent>


CellItem::CellItem(QGraphicsItem *parent)
    :QGraphicsItem(parent)
{
    condition = 0;
    number = 0;
    side = 50;
}

QRectF CellItem::boundingRect() const
{
    QPointF position(0,0);
    QSizeF size(side,side);
    return QRectF(position, size);
}

void CellItem::paint(QPainter * painter, const QStyleOptionGraphicsItem */*option*/, QWidget *)
{
    int width = 0;

    QColor color = Qt::white;
    QString text;
    switch (condition) {
    case 0:
        color = Qt::cyan;
        break;
    case 1:
        color = Qt::green;
        text = tr("A");
        break;
    case 2:
        color = Qt::green;
        text = tr("B");
        break;
    case 3:
        color = Qt::gray;
        break;
    case 4:
        color = Qt::white;
        break;
    }

    painter->setBrush(color);

    painter->setPen(QPen(Qt::black, width));
    painter->drawRect(2,2,side-4,side-4);
    painter->drawText(side/2,side/2,text);
    painter->drawText(0+4,side-4,QString("%0,%1").arg(coord.first).arg(coord.second));
}


void CellItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    int oldCondition = condition;
    if(event->button() == Qt::LeftButton)
    {
        condition = 1;
    }
    else if(event->button() == Qt::RightButton)
    {
        condition = 2;
    }
    else
    {
        if(condition == 0)
            condition = 3;
        else
            condition = 0;
    }
    emit signalChangeState(oldCondition, condition);
}
