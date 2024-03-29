#ifndef ADDEVENTDIALOG_H
#define ADDEVENTDIALOG_H

#include "general.h"
#include "database.h"
#include "sqlqueries.h"

namespace Ui {
class AddEventDialog;
}

class AddEventDialog : public QDialog
{
    Q_OBJECT
    Ui::AddEventDialog *ui;
    QVariantList* row;
    DataBase* sdb;
    QString event_id;
private slots:
    void SetTimeTo(QTime time_from);
//    void SetMinTimeFrom();
    void EnabledOkButton();
    void CheckFreeTime();
public:
    explicit AddEventDialog(DataBase* data_base, QVariantList* curr_row, Role role = ADD, QWidget *parent = nullptr);
    QString GetPatient();
    QString GetDate();
    QString GetTimeFrom();
    QString GetTimeTo();
    QString GetComment();
    QString GetPatientId();
    ~AddEventDialog();
};

#endif // ADDEVENTDIALOG_H
