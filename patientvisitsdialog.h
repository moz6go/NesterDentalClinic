#ifndef PATIENTVISITSDIALOG_H
#define PATIENTVISITSDIALOG_H

#include "general.h"
#include "database.h"
#include "sqlqueries.h"

namespace Ui {
class PatientVisitsDialog;
}

class PatientVisitsDialog : public QDialog
{
    Q_OBJECT
    Ui::PatientVisitsDialog *ui;
    DataBase* sdb;
    QSqlQueryModel* visits_model;
    QSortFilterProxyModel* visit_proxy_model;
public:
    explicit PatientVisitsDialog(DataBase* data_base, const QVariantList& row, QWidget *parent = nullptr);
    ~PatientVisitsDialog();

};

#endif // PATIENTVISITSDIALOG_H
