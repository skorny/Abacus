#include <QDebug>
#include <QKeyEvent>
#include <QGraphicsSimpleTextItem>

#include "widget.h"
#include "ui_widget.h"

#include "knuckles.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{

    ui->setupUi(this);
    ui->lineEdit->installEventFilter(this);
    ui->pushButton->setVisible(false);

    m_scene.setItemIndexMethod(QGraphicsScene::NoIndex);
    st = m_scene.addSimpleText("");
    st->setBrush(QBrush(QColor(250,250,250)));

    QFont f = st->font();
    f.setBold(true);
    f.setPointSize(15);
    st->setFont(f);
    st->setPos(m_scene.sceneRect().right() - 120, m_scene.sceneRect().bottom() - 15);
    //m_scene.setSceneRect(0, 0, 200, 350);
    ui->graphicsView->setScene(&m_scene);
    ui->graphicsView->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    QLinearGradient linearGradient (0, 0, 350, 350);
    linearGradient.setColorAt (0.1, 0xC1C1C1);
    linearGradient.setColorAt (1.0, 0xA5845C);
    ui->graphicsView->setBackgroundBrush((QBrush(linearGradient)));

    QLinearGradient linearGradient2 (0, 0, 350, 0);
    linearGradient2.setColorAt (1.0, 0x010010);
    linearGradient2.setColorAt (0.1, 0xffffff);
    QPen pen;
    //pen.setColor(QColor(150,150,150,255));
    pen.setBrush((QBrush(linearGradient2)));
    for(int i = 0; i < 9; i++){
        m_scene.addLine(0, 40*i-20, 350, 40*i-20, pen);
    }

    qreal w = 22;
    qreal h = 50;
    qreal x = m_scene.sceneRect().right();
    m_10000 = new Line(10000.0);
    m_scene.addItem(m_10000);
    if(m_10000->childItems().count() > 0){
        auto _l = m_10000->childItems();
        auto chil = _l.begin();
        w = (*chil)->boundingRect().width();
        h = (*chil)->boundingRect().height();
    }
    x = x - m_10000->childItems().count()*(w) ;
    m_10000->setPos(x, 0);

    m_1000 = new Line(1000.0);
    m_scene.addItem(m_1000);
    m_1000->setPos(x, 40);

    m_100 = new Line(100.0);
    m_scene.addItem(m_100);
    m_100->setPos(x, 40*2);

    m_10 = new Line(10.0);
    m_scene.addItem(m_10);
    m_10->setPos(x, 40*3);

    m_1 = new Line(1.0);
    m_scene.addItem(m_1);
    m_1->setPos(x, 40*4);

    m_25 = new Line(0.25);
    m_scene.addItem(m_25);
    qreal x1 = m_scene.sceneRect().right() - m_25->childItems().count()*(w) + 5;
    m_25->setPos(x1, 40*5);

    m_01 = new Line(0.1);
    m_scene.addItem(m_01);
    m_01->setPos(x, 40*6);

    m_001 = new Line(50.01);
    m_scene.addItem(m_001);
    m_001->setPos(x, 40*7);

    m_l.push_back(m_1);
    m_l.push_back(m_10);
    m_l.push_back(m_100);
    m_l.push_back(m_1000);
    m_l.push_back(m_10000);

    m_l.push_back(m_25);
    m_l.push_back(m_01);
    m_l.push_back(m_001);

    foreach(auto it, m_l){
        it->installEventFilterForAll();
        it->st = st;
        connect(it, SIGNAL(sgValueChanged()), this , SLOT(onValueChanged()));
    }

    //QGraphicsSimpleTextItem *st = new QGraphicsSimpleTextItem(nullptr);
    //  st = m_scene.addSimpleText("scenePos:(,)");
    //st->setPos(m_scene.sceneRect().right() - 40, m_scene.sceneRect().bottom() - 10);

}

Widget::~Widget()
{
    qDebug() << __func__;
    delete ui;
    foreach(auto it, m_l){
        m_l.remove(0);
        delete it;
    }
    m_l.clear();
}

bool Widget::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress){
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if((keyEvent->key() == Qt::Key_Enter) || (keyEvent->key() == Qt::Key_Return)){
            on_pushButton_clicked();
        }
    }
    // standard event processing
    return QObject::eventFilter(obj, event);
}


void Widget::on_pushButton_clicked()
{
    QStringList l = ui->lineEdit->text().split(".");
    foreach(auto s, l)
        qDebug() << s;

    QString snum = l.at(0);
    if(snum.count() > 5) return;
    //int i = 10-snum.count()+1;
    int count = snum.count();

    for(int i = 0; i < count; i++){
        int index = count - 1 - i;
        bool q = true;
        QString temp = snum.at(index);
        int k = temp.toInt();
        m_l.at(i)->move(k, i*15);
    }

    // дробные части  5,6,7
    if(l.count() > 1){
        QString sx = l.at(1);
        if(sx.count() == 1)
            sx+="0";
        if(sx.count() != 2) return;

        int k = sx.toInt() / 25;
        m_l.at(5)->move(k, 15);
        int ix = sx.toInt() - k*25;
        int x100=0, x10=0;
        sx = QString::number(ix);
        if(sx.count() == 1){
            QString tempstr = sx.at(0);
            x100 = tempstr.toInt();
        }
        if(sx.count() == 2){
            QString tempstr = sx.at(0);
            x10 = tempstr.toInt();
            tempstr = sx.at(1);
            x100 = tempstr.toInt();
        }
        m_l.at(6)->move(x10, 15);
        m_l.at(7)->move(x100, 15);
    }
    onValueChanged();
}


void Widget::on_pushButton_2_clicked()
{
    ui->lineEdit->setText("");
    st->setText("");
    foreach(auto it, m_l){
        it->flush();
    }
}


void Widget::on_lineEdit_textChanged(const QString &arg1)
{
    foreach(auto it, m_l){
        it->flush();
    }
}

void Widget::onValueChanged()
{
    int additional = 0;
    int _25 = m_l[5]->getValue();
    int _sot = 0;
    int _ten = 0;
    if(_25 == 4)
        additional++;
    else{
        _25 = _25*25;
        QString temp = QString::number(_25);
        if( temp.count() > 1){
            QString temp_sot = temp.at(1);
            QString temp_ten = temp.at(0);
            _sot = temp_sot.toInt();
            _ten = temp_ten.toInt();
        }
    }
    // сотые
    int add = 0;
    int sot = 0;
    int sot_val = m_l[7]->getValue();
    if( sot_val + _sot > 9){
        add++; // учтутся в десятках
        sot = sot_val + _sot - 10;
    }
    else{
        sot = sot_val + _sot;
    }
    QString text_sot = QString::number(sot);
    if(sot == 0) text_sot = "";

    // десятые
    int ten = 0;
    int ten_val = m_l[6]->getValue();
    if((ten_val + _ten + add) > 9){
        additional++;
        ten = ten_val+_ten + add - 10;
    }else{
        ten = ten_val+_ten + add;
    }
    QString text_ten = QString::number(ten);

    int res = 0;
    for(int i = 0; i < 5; i++){
        int   v = m_l[i]->getValue();
        if( v + additional > 9){
            v = v + additional - 10;
            additional = 1;
        }else{
            v = v + additional;
            additional = 0;
        }
        int line_val = v*m_l[i]->getRang();
        res += line_val;
    }
    if(additional > 0){res += 100000;
    }

    QString text = QString::number(res);
    int i = text.count()-1;
    int j = 1;
    QString new_str("");
    while(i >= 0){
        new_str.push_front(text.at(i));\
        if(j == 3){
            new_str.push_front(" ");
            j = 1;
        }else
            j++;
        i--;
    }
    text = new_str + "." + text_ten + text_sot;

//    QString text = "";
//    text = QString::number(res) + "." + text_ten + text_sot;

    st->setText(text);
    qDebug() <<  "  " << st->text();
}

