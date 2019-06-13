#ifndef ALLEVENTSDIALOG_H
#define ALLEVENTSDIALOG_H

#include "general.h"
#include "database.h"
#include "eventsproxymodel.h"

namespace Ui {
class AllEventsDialog;
}

class AllEventsDialog : public QDialog
{
    Q_OBJECT
    Ui::AllEventsDialog *ui;
    DataBase* sdb;
    QSqlTableModel* events_model;
    EventsProxyModel* events_filter_model;
private slots:
    void SetSearchType(QString type);
    void SearchTextChanged(QString text);
    void SearchDateChanged(QDate date);
    void SearchTimeChanged(QTime time);
    void SearchStatusChanged(QString text);
public:
    explicit AllEventsDialog(DataBase* data_base, QWidget *parent = nullptr);
    ~AllEventsDialog();
};

#endif // ALLEVENTSDIALOG_H
