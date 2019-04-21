#ifndef CUSTOMSCENE_H
#define CUSTOMSCENE_H

#include <QObject>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QPointF>

class CustomScene : public QGraphicsScene
{
    Q_OBJECT

public:
    CustomScene(QObject *parent = nullptr);
    // void SetPixmap(const QPixmap &pixmap);
    void SetPixmap(const QString &strDir, const QString &strScale);

private:
    QGraphicsRectItem* rect_item;
    QPixmap pixmap;
    QPointF dragpos, topleft, bottomright;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
};

#endif // CUSTOMSCENE_H
