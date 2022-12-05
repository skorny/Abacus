#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSimpleTextItem>
#include <QDebug>
#include "txscene.h"

txScene::txScene()
{
//    QGraphicsScene::mouseMoveEvent()
    setSceneRect(0, 0, 315, 350);
    st = addSimpleText("*");
    st->setPos(sceneRect().right() - 70, sceneRect().bottom() - 10);
    st->setPos(QPointF(0,0));
    st->setVisible(false);
}

void txScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    m_last_scene_y = event->scenePos().y();
    m_last_y = event->pos().y();
    m_last_screen_y = event->screenPos().y();
    QGraphicsScene::mousePressEvent(event);
}
void txScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QString str = QString::number(event->scenePos().x()) + " " + QString::number(event->scenePos().y());
    st->setText(str);
    event->setScenePos(QPointF(event->scenePos().x(), m_last_scene_y));
    QGraphicsScene::mouseMoveEvent(event);
}
void txScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mouseReleaseEvent(event);
}
