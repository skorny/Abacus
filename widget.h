#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QGraphicsScene>
#include "txscene.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Line;

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;


private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_lineEdit_textChanged(const QString &arg1);

    void onValueChanged();

private:
    Ui::Widget *ui;
    txScene m_scene;
    QGraphicsSimpleTextItem *st;
    QBrush bg;

    Line *m_10000;
    Line *m_1000;
    Line *m_100;
    Line *m_10;
    Line *m_1;
    Line *m_25;
    Line *m_01;
    Line *m_001;
    QVector<Line*> m_l;

    //QHash<float, qreal> m_val;

};
#endif // WIDGET_H
