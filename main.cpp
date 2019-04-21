#include "widget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.show();

    // Q_INIT_RESOURCE(resource); // リソースファイルを初期化
    return a.exec();
}
