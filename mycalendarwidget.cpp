#include "mycalendarwidget.h"
#include <QtDebug>
MyCalendarWidget::MyCalendarWidget(QWidget *parent) : QCalendarWidget (parent) {
    curr_date = QDate::currentDate();
//    blue_pen.setColor (Qt::blue);
//    red_pen.setColor(Qt::red);
//    brush.setColor (Qt::transparent);
}

MyCalendarWidget::~MyCalendarWidget() {}

void MyCalendarWidget::SetDataList(const QList<QDate> &list) {
    date_list = list;
}

void MyCalendarWidget::paintCell (QPainter *painter, const QRect &rect, const QDate &date) const
{
    QCalendarWidget::paintCell(painter, rect, date);
    if(date_list.contains (date)){
        painter->setPen(QPen(Qt::darkCyan, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
//        painter->setBrush(brush);
        painter->drawRect(rect.adjusted (0, 0, -1, -1));
    }
    if (date == curr_date) {
        painter->setPen(QPen(Qt::cyan, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
//        painter->setBrush(brush);
        painter->drawRect(rect.adjusted (0, 0, -1, -1));
    }

}
