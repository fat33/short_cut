#ifndef MYWIDGET_H
#define MYWIDGET_H

#include "cellitem.h"
#include "grid.h"

#include <QWidget>
#include <qgraphicsscene.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MyWidget; }
QT_END_NAMESPACE

class MyWidget : public QWidget
{
    Q_OBJECT

public:
    MyWidget(QWidget *parent = Q_NULLPTR);
    ~MyWidget();
protected slots:
    void slotGenerateCells();
    void slotGenerateBadCells();
    void slotChangeStateItem(int, int);
    void clearCells();
    void setConditionItem(QPair<int, int>, int);
    int  getConditionItem(QPair<int, int>);
    void slotMouseMoveBuildPuth(int, int);
protected:
    void loadSettings();
    void saveSettings();
private:
    Ui::MyWidget *ui;
    QGraphicsScene * graphicsScene;
    QHash<QPair<int, int>, CellItem*> hashCellItem;
    QPair<int, int> beginCell;
    QPair<int, int> endCell;
    QPair<int, int> nullCell;


signals:
    void signalUpdateGrid(const QHash<QPair<int, int>, CellItem *> &);
    void signalSetBegin(QPair<int, int>);
    void signalDrawPatch(const QHash<QPair<int, int>, CellItem *> & , QPair<int, int>, QPair<int, int>);
    void signalClearPatch(QHash<QPair<int, int>, CellItem *> );

};
#endif // MYWIDGET_H
