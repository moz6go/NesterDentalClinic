#include "patientvisitsdialog.h"
#include "ui_patientvisitsdialog.h"
#include "mainwindow.h"

PatientVisitsDialog::PatientVisitsDialog(DataBase *data_base, const QVariantList &row, QWidget *parent):
    QDialog(parent),
    ui(new Ui::PatientVisitsDialog)
{
    ui->setupUi(this);

    sdb = data_base;

    ui->patient_le->setText (row.at (SURNAME_COL).toString () + " " + row.at (NAME_COL).toString () + " " + row.at (F_NAME_COL).toString ());

    visits_model = new MySqlQueryModel(this);
    visits_model->setQuery (SqlQueries::PatientVisits(row.at (PATIENT_ID_COL).toString ()));
    visits_model->setHeaderData(0, Qt::Horizontal, VISITS_TABLE_HEADERS[VISIT_DATE_COL]);
    visits_model->setHeaderData(1, Qt::Horizontal, VISITS_TABLE_HEADERS[PRICE_COL]);
    visits_model->setHeaderData(2, Qt::Horizontal, VISITS_TABLE_HEADERS[VISIT_RESULT_COL]);
    ui->visits_table->setModel (visits_model);

    MainWindow::TableInit (ui->visits_table);
    ui->visits_table->verticalHeader ()->setSectionResizeMode (QHeaderView::ResizeToContents);
    ui->visits_table->horizontalHeader ()->setSectionResizeMode (QHeaderView::ResizeToContents);

    visits_model->sort (0, Qt::AscendingOrder);

}

PatientVisitsDialog::~PatientVisitsDialog() {
    delete ui;
}
