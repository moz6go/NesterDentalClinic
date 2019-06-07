#include "mycalendarwidget.h"

MyCalendarWidget::MyCalendarWidget(QWidget *parent) : QCalendarWidget (parent) {
    curr_date = QDate::currentDate();
    blue_pen.setColor (Qt::blue);
    red_pen.setColor(Qt::red);
    brush.setColor (Qt::transparent);
}

MyCalendarWidget::~MyCalendarWidget() {}

void MyCalendarWidget::SetDataList(const QList<QDate> &list) {
    date_list = list;
}

void MyCalendarWidget::paintCell (QPainter *painter, const QRect &rect, const QDate &date) const
{
    QCalendarWidget::paintCell(painter, rect, date);

    if (date == curr_date) {
        painter->setPen(blue_pen);
        painter->setBrush(brush);
        painter->drawRect(rect.adjusted(0, 0, -1, -1));
    }

    for (auto& d : date_list) {
        if (d == date){
            painter->setPen(red_pen);
            painter->setBrush(brush);
            painter->drawRect(rect.adjusted(0, 0, -1, -1));
        }
    }
}
