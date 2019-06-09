#ifndef MYCALENDARWIDGET_H
#define MYCALENDARWIDGET_H

#include <QObject>
#include <QPen>
#include <QPainter>
#include <QCalendarWidget>

class MyCalendarWidget : public QCalendarWidget
{
    Q_OBJECT
    QDate curr_date;
    QList<QDate> date_list;
//    QPen blue_pen;
//    QPen red_pen;
//    QBrush brush;
protected:
    virtual void paintCell(QPainter *painter, const QRect &rect, const QDate &date) const;
public:
    MyCalendarWidget(QWidget *parent = nullptr);
    ~MyCalendarWidget();
    void SetDataList(const QList<QDate>& list);
};

#endif // MYCALENDARWIDGET_H
