#ifndef REPORTDIALOG_H
#define REPORTDIALOG_H

#include "general.h"
#include "database.h"

namespace Ui {
class ReportDialog;
}

class ReportDialog : public QDialog
{
    Q_OBJECT
    Ui::ReportDialog *ui_report;
    DataBase* sdb;
public:
    explicit ReportDialog(DataBase* data_base, QWidget *parent = nullptr);
    QString GetDateFrom();
    QString GetDateTo();
    int GetReportType();
    ~ReportDialog();
};

#endif // REPORTDIALOG_H
