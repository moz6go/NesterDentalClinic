#ifndef ADDEVENTDIALOG_H
#define ADDEVENTDIALOG_H

#include <QDialog>
#include "general.h"

namespace Ui {
class AddEventDialog;
}

class AddEventDialog : public QDialog
{
    Q_OBJECT
    Ui::AddEventDialog *ui;
    QVariantList row;
public:
    explicit AddEventDialog(const QVariantList& curr_row, QWidget *parent = nullptr);
    QString GetPatient();
    QString GetDate();
    QString GetTime();
    QString GetComment();
    QString GetPatientId();
    ~AddEventDialog();
};

#endif // ADDEVENTDIALOG_H
