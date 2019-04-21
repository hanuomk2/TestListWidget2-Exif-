#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QString>

#include "customscene.h"

const int ICONWIDTH=160;
const int ICONHEIGHT=120;
const QString PATH="C:/Windows/Web/Screen";

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void collectImage();
    void selectImage();
    void selectComboBox(QString str);
    void openFolderDialog();
    void upFolderToParent();
    void getExifThumbnailImage(int num, const QString &strPATH);

private:
    Ui::Widget *ui;
    QString strDirPATH, strImagePATH;
    CustomScene* scene;
};

#endif // WIDGET_H
