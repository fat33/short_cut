#ifndef CELLITEM_H
#define CELLITEM_H

#include <QGraphicsItem>

class CellItem : public QObject, public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)

public:
    CellItem(QGraphicsItem *parent = Q_NULLPTR);

    virtual QRectF boundingRect() const;                                        //!< функция определяет внешние границы
    virtual void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);   //!< функция рисует содержание
    virtual void mousePressEvent(QGraphicsSceneMouseEvent*);


public slots:
    void setName(QString n){name = n;}
    QString getName(){return name;}

    void setNumber(int n){number = n;}
    int getNumber(){return number;}

    void setCondition(int c){condition = c;}
    int getCondition(){return condition;}

    void setSide(int s){side = s;}
    int getSide(){return side;}

    void setCoord(int x, int y){coord = qMakePair(x, y);}
    QPair<int, int> getCoord(){return coord;}

private:
    QPair<int, int> coord;
    QString name;
    int number;
    int condition;
    int side;

signals:
    void signalChangeState(int, int);
};

#endif // CELLITEM_H
