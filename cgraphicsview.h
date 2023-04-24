#ifndef CGRAPHICSVIEW_H
#define CGRAPHICSVIEW_H

#include <QGraphicsView>

class CGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    CGraphicsView(QWidget *parent = Q_NULLPTR, QGraphicsScene *scene = Q_NULLPTR);
protected:
    void wheelEvent(QWheelEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
public slots:
    void zoomIn();
    void zoomOut();
signals:
    void mouseMoveBuildPuth(int, int);
};

#endif // CGRAPHICSVIEW_H
