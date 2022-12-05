#include <QBrush>
#include <QDebug>
#include <QGraphicsTextItem>
#include <QPropertyAnimation>
#include <QEvent>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QApplication>
#include <QDrag>
#include <QMimeData>
#include <QMouseEvent>
#include <QGraphicsScene>

#include "knuckles.h"


bone::bone(const QRectF &rect, QGraphicsItem *parent)
    :QObject()
    ,QGraphicsEllipseItem(rect,parent)
    ,m_brush(QBrush())
    ,m_brush_selected(QBrush())
{
}

bone::bone(qulonglong c1, qulonglong c2, qreal x, qreal y, qreal w, qreal h, QGraphicsItem *parent, int number)
    :QObject()
    ,QGraphicsEllipseItem(x,y,w,h,parent)
    ,m_brush(QBrush())
    ,m_brush_selected(QBrush())
{
    setAcceptHoverEvents(true);
    setCacheMode(DeviceCoordinateCache);
    setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable);

    t = new QGraphicsTextItem(QString::number(number), this);
    t->setPos(0,0);
    t->setVisible(false);

    anim = new QPropertyAnimation(this, "pos");
    anim->setEasingCurve(QEasingCurve::InOutBack);

    this->setBrush(m_brush);
    QPointF ptCenter(this->rect().center());
    QRadialGradient gradient(ptCenter, this->rect().width(), ptCenter);
    gradient.setColorAt(0, c1);
    gradient.setColorAt(1, c2);
    this->setBrush(QBrush(gradient));

    gradient.setColorAt(0, 0xffffff);
    gradient.setColorAt(1, c2 + 0x101010);
    m_brush_selected = QBrush(gradient);
}
bone::bone(QGraphicsItem *parent)
    :QObject()
    ,QGraphicsEllipseItem(parent)
{
    //setCacheMode(DeviceCoordinateCache);
}

bone::~bone()
{
    if(anim)
        delete anim;
    if(t)
        delete t;
}

//---------------------------------------------------------------------------
void bone::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    onhover(true);
    return;
    //QGraphicsObject::hoverEnterEvent(event);
}

//---------------------------------------------------------------------------
void bone::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    onhover(false);
    return;
}

//---------------------------------------------------------------------------
void bone::setDuration(int i)
{
    anim->setDuration(i);
}

//---------------------------------------------------------------------------
void bone::setEndValue(QPointF point)
{
    anim->setEndValue(point);
}

//---------------------------------------------------------------------------
void bone::setCurve(QEasingCurve c)
{
    anim->setEasingCurve(c);
}

//---------------------------------------------------------------------------
void bone::startAnimation()
{
    anim->start();
}

//---------------------------------------------------------------------------
void bone::onhover(bool in)
{
    if(in){
        m_brush = this->brush();
        this->setBrush( m_brush_selected );
    }
    else{
        setBrush(m_brush);
    }
    this->update();
}

/// /////////////////////////////////////////////////////////////////////////// ////////////////////////////////////////////////////////////////////////
///
///
/// /////////////////////////////////////////////////////////////////////////// ////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------//

Line::Line(float r, QGraphicsEllipseItem *Pparent)
    :QObject()
    ,QGraphicsEllipseItem(Pparent)
{

    setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable | QGraphicsItem::ItemSendsScenePositionChanges);
    m_rang = r;
    qreal x = 0; qreal y = 0;
    if((r >= 0.23) && (r <= 0.26)){
        for(int i=0; i < 4; i++){
            qulonglong c1 = 0xEAC7B2;
            qulonglong c2 = 0xB56E3F;
            if( (i == 1) || (i==2)){
                c1 = 0x707070;
                c2 = 0x010101;
            }
            bone* ell = new bone(c1,c2,0,y,20,35,this, i+1);
            ell->setData(0, i); // индекс костяшки
            ell->setData(2, y);
            ell->setPos(x+i*22,y);
            ell->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable);
            m_knuckles.push_back(ell);
        }
    }
    else{
        for(int i=0; i < 10; i++){
            qulonglong c1 = 0xEAC7B2;
            qulonglong c2 = 0xB56E3F;
            if( ((i == 0) && (m_rang == 1000)) || (i == 4) || (i == 5)){
                c1 = 0x707070;
                c2 = 0x010101;
            }
            bone* ell = new bone(c1, c2, 0,y,20,35,this, i+1);
            ell->setData(0, i); // индекс костяшки
            ell->setData(2, y);
            ell->setPos(x+i*22,y);
            ell->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable);
            m_knuckles.push_back(ell);
        }
    }
}

//---------------------------------------------------------------------------
Line::~Line()
{
    foreach(auto it, m_knuckles){
        m_knuckles.remove(0);
        delete it;
    }
    m_knuckles.clear();
}

//---------------------------------------------------------------------------
void Line::init()
{
    QGraphicsEllipseItem* ell = m_knuckles[4];
    QRadialGradient gradient(ell->rect().center(), ell->rect().width(), ell->rect().center());
    gradient.setColorAt(0, 0x707070);
    gradient.setColorAt(1, 0x010101);
    ell->setBrush(QBrush(gradient));

    ell = m_knuckles[5];
    QRadialGradient gradien(ell->rect().center(), ell->rect().width(), ell->rect().center());
    gradien.setColorAt(0, 0x707070);
    gradien.setColorAt(1, 0x010101);
    ell->setBrush(QBrush(gradien));

    if(m_rang == 1000){
        QGraphicsEllipseItem* ell = m_knuckles[0];
        QRadialGradient gradient(ell->rect().center(), ell->rect().width(), ell->rect().center());
        gradient.setColorAt(0, 0x707070);
        gradient.setColorAt(1, 0x010101);
        ell->setBrush(QBrush(gradient));
    }
}

//---------------------------------------------------------------------------
void Line::move(int count, int deltadur)
{
    //qDebug() << "m_knuckles.count : " << m_knuckles.count();
    if(count > 10){
        return;
    }
    m_value = count-1;
    qreal w = m_knuckles[0]->rect().width();
    for(int i = 0; i < count; i++){
        bone* ell = m_knuckles[i];
        ell->setDuration(750+i*15 + deltadur);
        ell->setCurve(QEasingCurve::InOutBack);
        if(m_rang == 0.25)
            ell->setEndValue(QPointF(-230+i*w,0));
        else
            ell->setEndValue(QPointF(-100+i*w,0));
        ell->startAnimation();
    }
    return;
}

//---------------------------------------------------------------------------
void Line::finishmove(int deltadur)
{
    //qDebug() << __func__ << "m_value: " << m_value << " m_moving:" << m_moving << " m_direction:" << m_direction;
    if(m_moving){
        qreal stop = -100;
        if(!m_direction){
            if(m_rang == 0.25)
                stop = -230;
        }
        else{
            return;
            stop = 50;
        }
        qreal w = m_knuckles[0]->rect().width();
        for(int i = 0; i <= m_value; i++){
            bone* ell = m_knuckles[i];
            ell->setDuration(150+i*15);
            ell->setCurve();
            ell->setEndValue(QPointF(stop+i*w,0));
            ell->startAnimation();
        }
    }
    return;
}

//---------------------------------------------------------------------------
void Line::flush()
{
    int x = 0;
    m_value = -1;
    m_moving = false;
    m_direction = false;
    m_reset = false;
    for(int i = 0; i < m_knuckles.count(); i++){
        bone* ell = m_knuckles[i];
        ell->setPos(x+i*22,0);
    }
}

//---------------------------------------------------------------------------
void Line::installEventFilterForAll()
{
    foreach(auto it, m_knuckles){
        it->installSceneEventFilter(this);
    }
}

//---------------------------------------------------------------------------
bool Line::sceneEventFilter(QGraphicsItem *watched, QEvent *event)
{
    bone* b = qgraphicsitem_cast<bone*>(watched);
    if(b != 0){
        if(event->type() == QEvent::GraphicsSceneHoverEnter){
            //qDebug() << " HoverEnter  knuckles::";
        }
        if(event->type() == QEvent::GraphicsSceneHoverLeave){
            //qDebug() << " HoverLeave  knuckles::";
        }

        if(event->type() == QEvent::GraphicsSceneMousePress){
            prepareToMove(b->data(0).toInt(), true);
            //m_moving = true;
        }
        if(event->type() == QEvent::GraphicsSceneMouseRelease){
                prepareToMove(b->data(0).toInt(), false);
                m_direction = false;
            //m_moving = false;
        }
        if(event->type() == QEvent::GraphicsSceneMouseMove){
            QGraphicsSceneMouseEvent *e = static_cast<QGraphicsSceneMouseEvent*>(event);
            if(m_moving){
                if( e->lastScenePos().x() < e->scenePos().x()){
                    m_direction = true;
                    QPointF pos1 = m_knuckles[m_value]->pos();
                    m_reset = false;
                    if( m_value + 1 < m_knuckles.count()){
                        QPointF pos2 = m_knuckles[m_value+1]->pos();
                        if((pos2.x() - pos1.x()) < 23){
                            m_reset = true;
                            return true;
                        }
                    }
                }
                else{
                    m_direction = false;
                    QPointF s1 = mapFromItem( m_knuckles[0], m_knuckles[0]->pos() );
                    QPointF s2 = mapFromParent(m_knuckles[0]->pos());
                    qreal s3 = s1.x() - s2.x(); // !
                    if(s3 <= 3)
                        return true;
                }
            }
        }
    }
    return false;
}

//---------------------------------------------------------------------------
QVariant Line::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
//    if( (change == ItemPositionChange)||(change==ItemPositionHasChanged)||(change==ItemScenePositionHasChanged) )
//        qDebug()<< change << " " << value.toPointF();
    if (change == ItemPositionChange && scene()) {
             // value is the new position.
             QPointF newPos;// = value.toPointF();
             newPos = this->mapToScene(value.toPointF());
             QRectF rect = scene()->sceneRect();
             if (!rect.contains(newPos)) {
                 // Keep the item inside the scene rect. TODO: переделать в inside разрешенных границ
                 newPos.setX(qMin(rect.right(), qMax(newPos.x(), rect.left())));
                 newPos.setY(qMin(rect.bottom(), qMax(newPos.y(), rect.top())));
                 return newPos;
             }
         }
         return QGraphicsItem::itemChange(change, value);
}

//---------------------------------------------------------------------------
void Line::prepareToMove(int current, bool start)
{
    if(current >= m_knuckles.count()) return;
    //qDebug() << __func__ << "(" << current << "," << start << ")";

    m_value = current;
    if(start){
        m_moving = true;
        //qreal x = m_knuckles[current]->pos().x();
        qreal y = m_knuckles[current]->pos().y();
        for(int i = 0; i < current; i++){
            m_knuckles[i]->setParentItem(m_knuckles[current]);
            m_knuckles[i]->setPos( - (current-i)*22, y);
            m_knuckles[i]->onhover(start);
        }

    }
    else{
        qreal x = m_knuckles[current]->pos().x();
        qreal y = m_knuckles[current]->pos().y();
        for(int i = 0; i < current; i++){
            m_knuckles[i]->setParentItem(this); //m_knuckles[i]->setPos((i)*21, 0);
            QPointF pf = m_knuckles[i]->mapToParent(m_knuckles[i]->pos());
            pf = QPointF(x - (current-i)*22, y);
            m_knuckles[i]->setPos(pf);
            m_knuckles[i]->onhover(start);
        }
        finishmove(5);
        if( m_reset)
            m_value = -1;
        emit sgValueChanged();
        m_reset = false;
        m_moving = false;
    }
}
