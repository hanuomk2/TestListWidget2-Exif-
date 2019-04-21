#include <QStringList>
#include <QListWidgetItem>
#include <QDir>
#include <QFileDialog>
#include <QImage>
#include <QPixmap>
#include <QCommonStyle>
#include <QStringList>

#include "widget.h"
#include "ui_widget.h"
extern "C" {
    #include "exif-master/exif.h"
}

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{    
    ui->setupUi(this);

    //strImagePATHの初期化
    strImagePATH = nullptr;

    //lineEditの初期化
    ui->lineEdit->setText(PATH);

    //grahicsViewの初期化
    scene = new CustomScene(this);
    ui->graphicsView->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    ui->graphicsView->setScene(scene);

    // comboBoxの初期化:デフォルトは"1.0"(Index(3))
    ui->comboBox->addItems(QStringList() << tr("0.25") << tr("0.5") << tr("0.75") << tr("1.0") << tr("1.5") << tr("2.0") << tr("4.0"));
    ui->comboBox->setCurrentIndex(3);

    //各ボタンのアイコン画像の設定
    /*ui->btnFolderToParent->setIcon(QIcon(QApplication::style()->standardIcon(QStyle::SP_FileDialogToParent)));
    ui->btnFolderDialog->setIcon(QIcon(QApplication::style()->standardIcon(QStyle::SP_DirIcon)));
    ui->btnEnterKey->setIcon(QIcon(QApplication::style()->standardIcon(QStyle::SP_DialogOkButton)));*/
    ui->btnFolderToParent->setIcon(QCommonStyle().standardIcon(QStyle::SP_FileDialogToParent));
    ui->btnFolderDialog->setIcon(QCommonStyle().standardIcon(QStyle::SP_DirIcon));
    ui->btnEnterKey->setIcon(QCommonStyle().standardIcon(QStyle::SP_DialogOkButton));

    //シグナル・スロットの設定
    //lineEditで[Enter]キー押下時のシグナル・スロットを設定
    connect(ui->lineEdit, SIGNAL(returnPressed()), this, SLOT(collectImage()));
    //listWidgetでアイテム選択(クリック)時のシグナル・スロットを設定
    connect(ui->listWidget, SIGNAL(clicked(QModelIndex)), this, SLOT(selectImage()));
    //comboBoxでアイテム選択時のシグナル・スロットを設定
    connect(ui->comboBox, SIGNAL(currentTextChanged(QString)), this, SLOT(selectComboBox(QString)));

    //各ボタンのシグナル・スロットを設定
    connect(ui->btnEnterKey, SIGNAL(clicked(bool)), this, SLOT(collectImage()));
    connect(ui->btnFolderDialog, SIGNAL(clicked(bool)), this, SLOT(openFolderDialog()));
    connect(ui->btnFolderToParent, SIGNAL(clicked(bool)), this, SLOT(upFolderToParent()));
 }

Widget::~Widget()
{
    delete ui;
}

//listWidgetへの画像(フォルダ含む)の追加
void Widget::collectImage()
{
    QString strDir = ui->lineEdit->text();
    QDir dir(strDir);
    QStringList listDir;
    QStringList listImage;
    QStringList listFilter1, listFilter2;
    listFilter1 << "*.jpg" << "*.jpeg";
    listFilter2 << "*.bmp" << "*.gif" << "*.png";

    if(dir.exists())
    {
        strDirPATH = strDir;

        ui->listWidget->clear();
        ui->listWidget->setIconSize(QSize(ICONWIDTH, ICONHEIGHT));

        // フォルダのパス、アイコン(固定)をlistWidgetに登録する
        listDir = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
        for(int i=0; i < listDir.size(); i++)
        {
            strDir = listDir.at(i);
            ui->listWidget->addItem(strDir);
            QIcon mIcon(":/images/dir.png");
            ui->listWidget->item(i)->setIcon(mIcon);
        }

        // jpg, jpegファイルのパス、アイコンをlistWidgetに登録する
        int startNum = listDir.size();
        listImage = dir.entryList(listFilter1);
        for(int i=0; i < listImage.size(); i++)
        {
            strDir = listImage.at(i);
            ui->listWidget->insertItem(startNum+i, strDir);
            getExifThumbnailImage(startNum+i, strDirPATH + QDir::separator() + strDir);
        }

        // bmp, gif, gifファイルのパス、アイコンをlistWidgetに登録する
        startNum = listDir.size() + listImage.size();
        listImage = dir.entryList(listFilter2);
        for(int i=0; i < listImage.size(); i++)
        {
            strDir = listImage.at(i);
            ui->listWidget->insertItem(startNum+i, strDir);
            QIcon mIcon(strDirPATH + QDir::separator() + strDir);
            ui->listWidget->item(startNum+i)->setIcon(mIcon);
        }
    }
    else ui->lineEdit->setText(strDirPATH);
}

//listWidgetで画像(フォルダ含む)を選択
void Widget::selectImage()
{
    //現在選択されているアイテムを取得
    QListWidgetItem* item = ui->listWidget->currentItem();
    QString strDir = strDirPATH + "/" + item->text();
    QDir dir(strDir);

    //アイテムがフォルダの場合、listWidgetを更新
    if(dir.exists())
    {
        ui->lineEdit->setText(strDir);
        collectImage();
    }
    //アイテムが画像の場合、graphicsViewに画像を表示
    else
    {
        strImagePATH = strDir;
        selectComboBox(ui->comboBox->currentText());
    }
}

//comboBoxでアイテム選択
void Widget::selectComboBox(QString str)
{
    scene->SetPixmap(strImagePATH, str);
}

//[フォルダダイアログを開く]ボタンが押下された場合
void Widget::openFolderDialog()
{
    // ファイルダイアログを表示
    QString strDir = QFileDialog::getExistingDirectory(this, "Choose Folder", ui->lineEdit->text());
    // ファイルダイアログで正常な戻り値が出力された場合
    if(!strDir.isEmpty())
    {
        ui->lineEdit->setText(strDir);
        collectImage();
    }
}

//[上のフォルダに移動]ボタンが押下された場合
void Widget::upFolderToParent()
{
    QStringList listDirPath = ui->lineEdit->text().split("/");
    QString strDir = "";
    int roopend = listDirPath.size()-2;
    for(int i=0; i < roopend; i++)
        strDir += listDirPath.at(i) + "/";
    // if(roopend >= 0) strDir += listDirPath.at(roopend);
    strDir += listDirPath.at(roopend);
    if(roopend <= 0) strDir += "/";

    //ルートフォルダではない場合
    if(!strDir.isEmpty())
    {
        ui->lineEdit->setText(strDir);
        collectImage();
    }
}

void Widget::getExifThumbnailImage(int num, const QString &strPATH)
{
    //構造体、変数等の初期化
    void **ifdArray = nullptr;
    TagNodeInfo *tag;
    int result;

    // QStringをchar*に変換
    char* image_full_path = strPATH.toLocal8Bit().data();
    unsigned int len = (unsigned int)strPATH.toLocal8Bit().size();

    /* IFDテーブルの作成 *********************************************************************/
    // parse the JPEG header and create the pointer array of the IFD tables
    ifdArray = createIfdTableArray(image_full_path, &result);

    // check result status
    switch (result) {
    case 0: // no IFDs
        qDebug("[%s] does not seem to contain the Exif segment.\n", image_full_path);
        break;
    case ERR_READ_FILE:
        qDebug("failed to open or read [%s].\n", image_full_path);
        break;
    case ERR_INVALID_JPEG:
        qDebug("[%s] is not a valid JPEG file.\n", image_full_path);
        break;
    case ERR_INVALID_APP1HEADER:
        qDebug("[%s] does not have valid Exif segment header.\n", image_full_path);
        break;
    case ERR_INVALID_IFD:
        qDebug("[%s] contains one or more IFD errors. use -v for details.\n", image_full_path);
        break;
    default:
        qDebug("[%s] createIfdTableArray: result=%d\n", image_full_path, result);
        break;
    }

    if (!ifdArray)
    {
        // Thumbnailがなかった場合等
        QIcon mIcon(strPATH);
        ui->listWidget->item(num)->setIcon(mIcon);

        return;
    }

     /* 1st IFDにサムネイル画像があれば、それを表示する **************************************/
     unsigned char *p;

     // try to get thumbnail data from 1st IFD
     p = getThumbnailDataOnIfdTableArray(ifdArray, &len, &result);
     if (p) {
         //サムネイルデータをJPGフォーマットでQImageに流し込む
         QImage image;
         image.loadFromData(p, (int)len, "JPG");

         /* TAG_Orientationを基に画像の向きを正す**************************************/
         tag = getTagInfo(ifdArray, IFD_0TH, TAG_Orientation);
         if (tag) {
             if (!tag->error) {
                 //反転処理
                 switch (tag->numData[0]) {
                     case 2: case 5: case 7:
                         // 水平反転
                         image = image.mirrored(true, false);
                         break;
                     case 4:
                         // 垂直反転
                         image = image.mirrored(false, true);
                         break;
                     default:
                         break;
                 }

                 // 回転処理
                 switch (tag->numData[0]) {
                     case 3:
                         image = image.transformed(QMatrix().rotate(180));
                         break;
                     case 6: case 7:
                         image = image.transformed(QMatrix().rotate(90));
                         break;
                     case 5: case 8:
                         image = image.transformed(QMatrix().rotate(270));
                         break;
                     default:
                         break;
                 }
             }
             freeTagInfo(tag);
         }

         // 描画処理(QImageをQPixmapに変換)
         // QPixmap pixmap = QPixmap::fromImage(image);
         QIcon mIcon(QPixmap::fromImage(image));
         ui->listWidget->item(num)->setIcon(mIcon);
     }
     else
     {
         qDebug("getThumbnailDataOnIfdTableArray: ret=%d\n", result);
     }
    // free IFD table array
    freeIfdTableArray(ifdArray);
}
