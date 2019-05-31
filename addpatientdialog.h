#ifndef ADDPATIENTDIALOG_H
#define ADDPATIENTDIALOG_H

#include <QDialog>
#include "general.h"
#include "database.h"

namespace Ui {
class AddPatientDialog;
}

class AddPatientDialog : public QDialog
{
    Q_OBJECT
    DataBase* sdb;
    QString photo_path;
private slots:
    void LoadPhoto();
    void EnableAddButton();
public:
    QString GetPhotoPath();
    QString GetSurname();
    QString GetName();
    QString GetFName();
    QString GetBDate();
    QString GetSex();
    QString GetCity();
    QString GetTelNumber();
    explicit AddPatientDialog(DataBase* data_base, QWidget *parent = nullptr);
    ~AddPatientDialog();

private:
    Ui::AddPatientDialog *ui;
};

#endif // ADDPATIENTDIALOG_H
