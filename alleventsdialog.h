#ifndef ALLEVENTSDIALOG_H
#define ALLEVENTSDIALOG_H

#include "general.h"
#include "mysortfilterproxymodel.h"
#include "database.h"

namespace Ui {
class AllEventsDialog;
}

class AllEventsDialog : public QDialog
{
    Q_OBJECT
    Ui::AllEventsDialog *ui;
    DataBase* sdb;
    QSqlTableModel* events_model;
    MySortFilterProxyModel* events_filter_model;

public:
    explicit AllEventsDialog(DataBase* data_base, QWidget *parent = nullptr);
    ~AllEventsDialog();
};

#endif // ALLEVENTSDIALOG_H
