#include "bindeventdialog.h"
#include "ui_bindeventdialog.h"
#include "mainwindow.h"

BindEventDialog::BindEventDialog(DataBase *data_base, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BindEventDialog)
{
    ui->setupUi(this);
    sdb = data_base;

    ui->search_cb->addItems (QStringList()
                             << PATIENTS_HEADERS[SURNAME_COL]
                             << PATIENTS_HEADERS[NAME_COL]
                             << PATIENTS_HEADERS[F_NAME_COL]) ;

    patients_model = new QSqlTableModel(this);
    patients_model->setTable (PATIENTS_TABLE);
    MainWindow::RenameHeaders(patients_model->columnCount(), patients_model, PATIENTS_HEADERS);
    patients_filter_model = new QSortFilterProxyModel(this);
    patients_filter_model->setSourceModel (patients_model);
    patients_filter_model->setFilterCaseSensitivity (Qt::CaseInsensitive);
    patients_filter_model->setFilterKeyColumn (SURNAME_COL);
    ui->patients_table->setModel (patients_filter_model);

    for (int col = 0; col < patients_model->columnCount(); ++col) {
        if (!(col == SURNAME_COL || col == NAME_COL || col == F_NAME_COL)){
            ui->patients_table->setColumnHidden(col, true);
        }
        else {
            ui->patients_table->setColumnWidth(col,  ui->patients_table->width () / 3);
        }
    }

    MainWindow::TableInit(ui->patients_table);
    ui->patients_table->setShowGrid(false);

    QObject::connect (ui->search_cb, &QComboBox::currentTextChanged, this, &BindEventDialog::SetSearchType);
    QObject::connect (ui->search_le, &QLineEdit::textChanged, this, &BindEventDialog::SearchTextChanged);

    QObject::connect (ui->ok_pb, &QPushButton::clicked, this, &QDialog::accept);
    QObject::connect (ui->cancel_pb, &QPushButton::clicked, this, &QDialog::reject);
    patients_model->select ();
}

QString BindEventDialog::GetPatientId(){
    return patients_filter_model->data(patients_filter_model->index (ui->patients_table->currentIndex ().row (), PATIENT_ID_COL)).toString ();
}

QString BindEventDialog::GetPatient(){
    return patients_filter_model->data(patients_filter_model->index (ui->patients_table->currentIndex ().row (), SURNAME_COL)).toString () + " "
           + patients_filter_model->data(patients_filter_model->index (ui->patients_table->currentIndex ().row (), NAME_COL)).toString () + " "
           + patients_filter_model->data(patients_filter_model->index (ui->patients_table->currentIndex ().row (), F_NAME_COL)).toString ();
}

void BindEventDialog::SetSearchType(QString type) {
    if(type == PATIENTS_HEADERS[SURNAME_COL]) {
        patients_filter_model->setFilterKeyColumn (SURNAME_COL);
    }
    else if (type == PATIENTS_HEADERS[NAME_COL]) {
        patients_filter_model->setFilterKeyColumn (NAME_COL);
    }
    else if (type == PATIENTS_HEADERS[F_NAME_COL]) {
        patients_filter_model->setFilterKeyColumn (F_NAME_COL);
    }
}

void BindEventDialog::SearchTextChanged(QString text) {
    patients_filter_model->setFilterFixedString (text);
}
BindEventDialog::~BindEventDialog() {
    delete ui;
}
