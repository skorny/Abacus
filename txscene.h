#ifndef TXSCENE_H
#define TXSCENE_H

#include <QGraphicsScene>


class txScene : public QGraphicsScene
{
public:
    txScene();

protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
    qreal m_last_scene_y;
    qreal m_last_y = 0;
    qreal m_last_screen_y = 0;
    QGraphicsSimpleTextItem *st;

};

#endif // TXSCENE_H
