#include "widget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    if(a.arguments().size() > 1) w.collectImage(a.arguments());//ここ重要
    w.show();

    // Q_INIT_RESOURCE(resource); // リソースファイルを初期化
    return a.exec();
}
