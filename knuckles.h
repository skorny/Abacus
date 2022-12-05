#ifndef LINE_H
#define LINE_H

#include <QGraphicsItem>
#include <QGraphicsEllipseItem>
#include <QBrush>
#include <QEasingCurve>

// класс костяшки
class QPropertyAnimation;

 class bone : public QObject, public QGraphicsEllipseItem
 {
     Q_OBJECT
     Q_PROPERTY(QPointF pos READ pos WRITE setPos)
     public:

     explicit bone(const QRectF &rect, QGraphicsItem *parent = nullptr);
     explicit bone(qulonglong c1, qulonglong c2, qreal x, qreal y, qreal w, qreal h, QGraphicsItem *parent = nullptr, int number = 0);
     explicit bone(QGraphicsItem *parent = nullptr);
     virtual ~bone();

 protected:
     void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
     void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

 public:
     void setDuration(int);
     void setEndValue(QPointF);
     void setCurve(QEasingCurve = QEasingCurve::Linear);
     void startAnimation();
     void onhover(bool);

 private:
     QPropertyAnimation     *anim;
     QGraphicsTextItem      *t;
     QBrush                 m_brush;
     QBrush                 m_brush_selected;
     qreal                  m_y = 0;
     qreal                  m_x = 0;
 };
//----------------------------------------------------------------------------------------//

 /// класс одной линии
class Line : public QObject, public QGraphicsEllipseItem
{
    Q_OBJECT
public:
    Line(float, QGraphicsEllipseItem* Pparent = nullptr);
    virtual ~Line();

    // анимированное выставление числа, набранного в поле ввода
    void move(int, int deltadur);

    // завершение движения нескольких(одной) костяшек, начатое пользователем влево
    // TODO можно сделать завершение движения вправо
    void finishmove(int deltadur);

    // сброс
    void flush();


    void installEventFilterForAll();

    const float getRang( ){return m_rang;};
    const int  getValue( ){return m_value + 1;};

    // по хорошему надо спрятать и написать гетер/сетер
    QGraphicsSimpleTextItem* st;    // выставленное значение

signals:
    void sgValueChanged(); // эмитится, при изменении значения

 protected:
    bool sceneEventFilter(QGraphicsItem *watched, QEvent *event) override;
    QVariant itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value) override;


private:
    void init();
    float               m_rang = -1.0;
    int                 m_value = -1;
    QVector<bone*>      m_knuckles;
    bool                m_moving = false;
    bool                m_direction = false;
    bool                m_reset = false;

    // current : индекс выбранной костяшки
    //start == true -> при нажатии на промежуточной костяшке, отобразить выбранным цветом все предшествующие, установить им нажатого парентом
    //start == false -> завершить движение
    void prepareToMove(int current, bool start);
};

#endif // LINE_H
