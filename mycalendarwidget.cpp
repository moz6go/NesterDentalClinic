#include "mycalendarwidget.h"
#include <QtDebug>
MyCalendarWidget::MyCalendarWidget(QWidget *parent) : QCalendarWidget (parent) {
    curr_date = QDate::currentDate();
}

MyCalendarWidget::~MyCalendarWidget() {}

void MyCalendarWidget::SetActiveDataList(const QList<QDate> &list) {
    active_date_list = list;
}

void MyCalendarWidget::paintCell (QPainter *painter, const QRect &rect, const QDate &date) const
{
    QCalendarWidget::paintCell(painter, rect, date);
    if(active_date_list.contains (date) && date < QDate::currentDate ()){
        painter->setPen(QPen(Qt::gray, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        painter->drawRect(rect.adjusted (0, 0, -1, -1));
    }
    else if (active_date_list.contains (date) && date > QDate::currentDate ()) {
        painter->setPen(QPen(Qt::darkCyan, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        painter->drawRect(rect.adjusted (0, 0, -1, -1));
    }
    else if (date == curr_date) {
        painter->setPen(QPen(Qt::cyan, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        painter->drawRect(rect.adjusted (0, 0, -1, -1));
    }
}
