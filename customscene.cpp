#include <QGraphicsSceneMouseEvent>
#include <QRectF>
#include <QClipboard>
#include <QApplication>
#include <QPainterPath>
#include <QPainter>

#include "customscene.h"

CustomScene::CustomScene(QObject *parent) :QGraphicsScene(parent)
{ // Constructor of Scene
    //this->over = false;
}

void CustomScene::SetPixmap(const QString &strDir, const QString &strScale)
{
    clear();                                            //sceneをクリア

    pixmap.load(strDir);                                //pixmapに画像ファイルを設定

    double scale = strScale.toDouble();
    if (scale != 1.0) {                                 //scaleが1.0以外の場合
        QSize scaleSize;
        scaleSize.setWidth((int)(pixmap.width() * scale));
        scaleSize.setHeight((int)(pixmap.height() * scale));

        setSceneRect(0, 0, scaleSize.width(), scaleSize.height());
        pixmap = pixmap.scaled(scaleSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    }else setSceneRect(0, 0, pixmap.width(), pixmap.height());

    addPixmap(pixmap);                                  //pixmapを設定

    QClipboard *clipboard = QApplication::clipboard();  //アプリケーションのグローバルクリップボードのポインタを返す
    clipboard->setPixmap(pixmap);                       //クリップボードにpixmapを設定

    QBrush brush(Qt::TexturePattern);                   //DashLineをpngファイルから作成
    brush.setTextureImage(QImage(":/images/DashLine.png"));
    QPen pen(brush, 3);
    rect_item = addRect(0, 0, 0, 0, pen);               //rect_itemを初期化
}

void CustomScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    dragpos = event->scenePos();
    qDebug("\n[dragpos] x:%4.1f y:%4.1f", dragpos.x(), dragpos.y());
}

void CustomScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QPointF scenepos = event->scenePos();
    /*
    if(scenepos.rx() < dragpos.rx()) {                  //rect_itemのX軸を設定
        topleft.setX(scenepos.rx());
        bottomright.setX(dragpos.rx());
    }else {
        topleft.setX(dragpos.rx());
        bottomright.setX(scenepos.rx());
    }

    if(scenepos.ry() < dragpos.ry()) {                  //rect_itemのY軸を設定
        topleft.setY(scenepos.ry());
        bottomright.setY(dragpos.ry());
    }else {
        topleft.setY(dragpos.ry());
        bottomright.setY(scenepos.ry());
    }
    */
    if(dragpos.rx() < scenepos.rx()) {                  //MousePress点がMouseMove点・MouseRelease点より小さい場合(X座標)
        if(dragpos.ry() < scenepos.ry()) {              //MousePress点がMouseMove点・MouseRelease点より小さい場合(Y座標)
            topleft = dragpos;
            bottomright = scenepos;
        }
        else {                                          //MousePress点がMouseMove点・MouseRelease点より大きい場合(Y座標)
            topleft.setX(dragpos.rx());
            topleft.setY(scenepos.ry());
            bottomright.setX(scenepos.rx());
            bottomright.setY(dragpos.ry());
        }
    } else {                                            //MousePress点がMouseMove点・MouseRelease点より大きい場合(X座標)
        if(dragpos.ry() < scenepos.ry()) {              //MousePress点がMouseMove点・MouseRelease点より小さい場合(Y座標)
            topleft.setX(scenepos.rx());
            topleft.setY(dragpos.ry());
            bottomright.setX(dragpos.rx());
            bottomright.setY(scenepos.ry());
        }
        else {                                          //MousePress点がMouseMove点・MouseRelease点より大きい場合(Y座標)
            topleft = scenepos;
            bottomright = dragpos;
        }
    }

    rect_item->setRect(QRectF(topleft, bottomright));   //rect_itemを移動
}

void CustomScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    mouseMoveEvent(event);

    QClipboard *clipboard = QApplication::clipboard(); //アプリケーションのグローバルクリップボードのポインタを返す
    clipboard->setPixmap(pixmap.copy(QRect(topleft.toPoint(), bottomright.toPoint())));//クリップボードにpixmapを設定
    qDebug("[topleft] x:%4.1f y:%4.1f", topleft.x(), topleft.y());
    qDebug("[bottomright] x:%4.1f y:%4.1f", bottomright.x(), bottomright.y());
}
