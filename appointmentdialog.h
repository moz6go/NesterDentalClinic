#ifndef APPOINTMENTDIALOG_H
#define APPOINTMENTDIALOG_H

#include "general.h"
#include "database.h"

namespace Ui {
class AppointmentDialog;
}

class AppointmentDialog : public QDialog
{
    Q_OBJECT
    Ui::AppointmentDialog *ui;
    DataBase* sdb;
    QVariantList* row;
public:
    explicit AppointmentDialog(DataBase* data_base, QVariantList* curr_row,  QWidget *parent = nullptr);
    double GetPrice();
    QString GetResults();
    ~AppointmentDialog();
};

#endif // APPOINTMENTDIALOG_H
