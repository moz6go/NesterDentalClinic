#ifndef ADDPATIENTDIALOG_H
#define ADDPATIENTDIALOG_H

#include "general.h"
#include "database.h"
#include "sqlqueries.h"

namespace Ui {
class AddPatientDialog;
}

class AddPatientDialog : public QDialog
{
    Q_OBJECT
    QString photo_path;
    DataBase* sdb;
    QVariantList row;
    bool isPicDel;
private slots:
    void SexChanged(QString sex);
    void LoadPhoto();
    void EnableAddButton();
    void onClosePbClick();
public:
    QString GetPhotoPath();
    QString GetSurname();
    QString GetName();
    QString GetFName();
    QString GetBDate();
    QString GetSex();
    QString GetCity();
    QString GetTelNumber();
    QString GetIllnesses();
    bool isPicDeleted();
    explicit AddPatientDialog(DataBase* data_base, const QVariantList& row, QWidget *parent = nullptr);
    ~AddPatientDialog();

private:
    Ui::AddPatientDialog *ui;
};

#endif // ADDPATIENTDIALOG_H
