#include "mywidget.h"
#include "ui_mywidget.h"

#include <qdebug.h>
#include <QDir>
#include <QSettings>
#include <QMessageBox>
#include <QRandomGenerator>
#include <QThread>

MyWidget::MyWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MyWidget)
{
    ui->setupUi(this);

    qRegisterMetaType<QHash<QPair<int,int>,CellItem*> >();

    QThread * thread = new QThread();
    thread->start();

    Grid * grid = new Grid();
    grid->moveToThread(thread);

    connect( this, SIGNAL(signalUpdateGrid(QHash<QPair<int, int>, CellItem *> )),
             grid, SLOT(updateGrid(QHash<QPair<int, int>, CellItem *> )));

    connect( this, SIGNAL(signalSetBegin(QPair<int, int>)),
             grid, SLOT(setBegin(QPair<int, int>)));

    connect( this, SIGNAL(signalDrawPatch(QHash<QPair<int, int>, CellItem *>  , QPair<int, int>, QPair<int, int>)),
             grid, SLOT(drawPatch(QHash<QPair<int, int>, CellItem *>  , QPair<int, int>, QPair<int, int>)));

    connect( this, SIGNAL(signalClearPatch(QHash<QPair<int, int>, CellItem *> )),
             grid, SLOT(clearPatch(QHash<QPair<int, int>, CellItem *> )));

    graphicsScene = new QGraphicsScene();
    ui->graphicsView->setScene(graphicsScene);

    connect(ui->graphicsView, SIGNAL(mouseMoveBuildPuth(int, int)),
            this, SLOT(slotMouseMoveBuildPuth(int, int)));

    connect(ui->generateCellsTB, SIGNAL(clicked()), this, SLOT(slotGenerateCells()));
    connect(ui->generateBadCellsTB, SIGNAL(clicked()), this, SLOT(slotGenerateBadCells()));
    connect(ui->clearCellsTB, SIGNAL(clicked()), this, SLOT(clearCells()));

    loadSettings();

    nullCell = qMakePair(-1,-1);
    beginCell = nullCell;
    endCell = nullCell;
}

MyWidget::~MyWidget()
{
    saveSettings();
    delete ui;
}

void MyWidget::slotGenerateCells()
{
    graphicsScene->clear();
    hashCellItem.clear();

    for(int i = 0; i < ui->wSpinBox->value(); ++i)
        for(int j = 0; j < ui->hSpinBox->value(); ++j)
        {
            CellItem * cell = new CellItem();
            cell->setCoord(i,j);
            cell->setPos(i*cell->getSide()+2, j*cell->getSide()+2);
            cell->setFlags(QGraphicsItem::ItemIsSelectable|QGraphicsItem::ItemIsFocusable);

            connect(cell, SIGNAL(signalChangeState(int, int)), this, SLOT(slotChangeStateItem(int, int)));
            graphicsScene->addItem(cell);

            hashCellItem[qMakePair(i,j)] = cell;
        }
    emit signalUpdateGrid(hashCellItem);
}

void MyWidget::slotGenerateBadCells()
{
    int countBadCell = ui->countBadCellSpinBox->value();
    int countCell = ui->hSpinBox->value()*ui->wSpinBox->value();

    if(countBadCell > countCell)
    {
        QMessageBox::critical(this, tr("Error generate bad cell"),
                              tr("The number of destinations cannot exceed the number of cells"));
        return;
    }

    if(hashCellItem.count() > 0)
    {
        for(int i = 0; i < countBadCell; ++i)
        {
            int y = QRandomGenerator::global()->bounded( ui->hSpinBox->value() );
            int x = QRandomGenerator::global()->bounded( ui->wSpinBox->value() );
            QPair<int, int> coord = qMakePair(x,y);
            if(hashCellItem.contains(coord))
            {
                if(CellItem * cell = hashCellItem[coord])
                {
                    cell->setCondition(3);
                    cell->update();
                }
            }
        }
    }
    emit signalUpdateGrid(hashCellItem);
    graphicsScene->update(graphicsScene->sceneRect());
}

void MyWidget::slotChangeStateItem(int conditionOld, int conditionNew)
{
    if(conditionOld == 1) beginCell = nullCell;
    else if(conditionOld == 2) endCell = nullCell;

    emit signalClearPatch(hashCellItem);

    if(CellItem * cell = qobject_cast<CellItem *>(sender()))
    {
        emit signalUpdateGrid(hashCellItem);

        if(conditionNew == 1)
        {
            setConditionItem(beginCell, 0);
            beginCell = cell->getCoord();
        }
        else if(conditionNew == 2)
        {
            setConditionItem(endCell, 0);
            endCell = cell->getCoord();
        }
        cell->update();

        if(beginCell != nullCell)
            emit signalSetBegin(beginCell);
        if(beginCell != nullCell && endCell != nullCell)
            emit signalDrawPatch(hashCellItem, beginCell, endCell);
        else
            emit signalClearPatch(hashCellItem);
    }
    graphicsScene->update(graphicsScene->sceneRect());
}

void MyWidget::clearCells()
{
    QHashIterator<QPair<int, int>, CellItem*> i(hashCellItem);
    while (i.hasNext()) {
        i.next();
        CellItem * cell = i.value();
        cell->setCondition(0);
        cell->update();
    }
    beginCell = nullCell;
    endCell = nullCell;
    graphicsScene->update(graphicsScene->sceneRect());
}

void MyWidget::setConditionItem(QPair<int, int> coord, int condition)
{
    if(hashCellItem.contains(coord))
    {
        CellItem * cell = hashCellItem[coord];
        cell->setCondition(condition);
        cell->update();
    }

}

int MyWidget::getConditionItem(QPair<int, int> coord)
{
    if(hashCellItem.contains(coord))
    {
        CellItem * cell = hashCellItem[coord];
        return cell->getCondition();
    }
    return -1;
}

void MyWidget::slotMouseMoveBuildPuth(int x, int y)
{
    QPair<int, int> cell = qMakePair(x,y);
    if(beginCell == nullCell) return;
    if(beginCell != cell)
    {

        int condition = getConditionItem(cell);
        if( condition != -1 && condition != 3)
        {
            emit signalUpdateGrid(hashCellItem);
            setConditionItem(endCell, 0);
            endCell = cell;
            setConditionItem(endCell, 2);
            if(beginCell != nullCell)
                emit signalDrawPatch(hashCellItem, beginCell, endCell);
        }
    }
    graphicsScene->update(graphicsScene->sceneRect());
}

void MyWidget::loadSettings()
{
    QSettings settings(QDir::home().absoluteFilePath(".config/shortCut/shortCut.ini"), QSettings::IniFormat);
    setGeometry( settings.value("MainWin/geometry").toRect());
    ui->splitter->restoreState(settings.value("MainWin/splitter").toByteArray());
    ui->wSpinBox->setValue(settings.value("cells/w").toInt());
    ui->hSpinBox->setValue(settings.value("cells/h").toInt());
    ui->countBadCellSpinBox->setValue(settings.value("cells/bad").toInt());
}

void MyWidget::saveSettings()
{
    QSettings settings(QDir::home().absoluteFilePath(".config/shortCut/shortCut.ini"), QSettings::IniFormat);
    settings.setValue("MainWin/geometry", geometry());
    settings.setValue("MainWin/splitter", ui->splitter->saveState());
    settings.setValue("cells/w", ui->wSpinBox->value());
    settings.setValue("cells/h", ui->hSpinBox->value());
    settings.setValue("cells/bad", ui->countBadCellSpinBox->value());
}

