#include "mywidget.h"
#include <QApplication>

#include <QTranslator>
#include <QLocale>
#include <QLibraryInfo>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator qtTranslator;
    qtTranslator.load("qt_" + QLocale::system().name(),
                      QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    a.installTranslator(&qtTranslator);

    QTranslator translator;
    translator.load(":/transfer/short_cut_ru_RU");
    a.installTranslator(&translator);

    MyWidget w;
    w.show();
    return a.exec();
}
