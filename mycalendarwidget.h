#ifndef MYCALENDARWIDGET_H
#define MYCALENDARWIDGET_H

#include "general.h"

class MyCalendarWidget : public QCalendarWidget
{
    Q_OBJECT
    QDate curr_date;
    QList<QDate> active_date_list;
protected:
    virtual void paintCell(QPainter *painter, const QRect &rect, const QDate &date) const;
public:
    MyCalendarWidget(QWidget *parent = nullptr);
    ~MyCalendarWidget();
    void SetActiveDataList(const QList<QDate>& list);
};

#endif // MYCALENDARWIDGET_H
