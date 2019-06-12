#ifndef BINDEVENTDIALOG_H
#define BINDEVENTDIALOG_H

#include "general.h"
#include "database.h"

namespace Ui {
class BindEventDialog;
}

class BindEventDialog : public QDialog
{
    Q_OBJECT
    Ui::BindEventDialog *ui;
    DataBase* sdb;
    QSqlTableModel* patients_model;
    QSortFilterProxyModel* patients_filter_model;
private slots:
    void SetSearchType(QString type);
    void SearchTextChanged(QString text);
public:
    explicit BindEventDialog(DataBase* data_base, QWidget *parent = nullptr);
    QString GetPatientId();
    QString GetPatient();
    ~BindEventDialog();
};

#endif // BINDEVENTDIALOG_H
