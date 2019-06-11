#include "alleventsdialog.h"
#include "ui_alleventsdialog.h"
#include "mainwindow.h"

AllEventsDialog::AllEventsDialog(DataBase *data_base, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AllEventsDialog)
{
    ui->setupUi(this);
    sdb = data_base;
    events_model = new QSqlTableModel(this);
    events_model->setTable(EVENTS_TABLE);
    MainWindow::RenameHeaders(events_model->columnCount(), events_model, EVENTS_TABLE_HEADERS);
    events_filter_model = new MySortFilterProxyModel(this);
    events_filter_model->setSourceModel(events_model);
    events_filter_model->setFilterKeyColumn(PATIENT_COL);
//    events_filter_model->setFilterFixedString(QDate::currentDate().toString(SQL_DATE_FORMAT));
    ui->events_table->setModel(events_filter_model);

    for (int col = 0; col < events_model->columnCount(); ++col) {
        if (col == EVENT_ID_COL || col == INIT_DATE_COL || col == EVENT_PATIENT_ID_COL){
            ui->events_table->setColumnHidden(col, true);
        }
    }

    MainWindow::TableInit (ui->events_table);
    ui->events_table->setColumnWidth (EVENT_DATE_COL,70);
    ui->events_table->setColumnWidth(EVENT_TIME_FROM_COL, 40);
    ui->events_table->setColumnWidth(EVENT_TIME_TO_COL, 40);
    ui->events_table->setColumnWidth(PATIENT_COL, 250);
    events_model->select();

}

AllEventsDialog::~AllEventsDialog() {
    delete ui;
}
