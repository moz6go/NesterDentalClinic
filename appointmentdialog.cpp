#include "appointmentdialog.h"
#include "ui_appointmentdialog.h"


AppointmentDialog::AppointmentDialog(DataBase *data_base, QVariantList *curr_row, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AppointmentDialog)
{
    ui->setupUi(this);
    sdb = data_base;
    row = curr_row;
    if(row->size()) {
        ui->patient_le->setText (row->at(PATIENT_COL).toString ());
        ui->visit_date->setDate (QDate::currentDate ());
    }
    QObject::connect(ui->results_te, &QTextEdit::textChanged, this, &AppointmentDialog::EnabledOkButton);
    QObject::connect(ui->ok_pb, &QPushButton::clicked, this, &AppointmentDialog::accept);
    QObject::connect(ui->cancel_pb, &QPushButton::clicked, this, &AppointmentDialog::reject);
}

QString AppointmentDialog::GetVisitDate() {
    return ui->visit_date->date ().toString (SQL_DATE_FORMAT);
}

double AppointmentDialog::GetPrice() {
    return ui->price_sb->value();
}

QString AppointmentDialog::GetResults() {
    return ui->results_te->toPlainText();
}

void AppointmentDialog::EnabledOkButton(){
    ui->ok_pb->setEnabled(ui->results_te->toPlainText().size());
}

AppointmentDialog::~AppointmentDialog() {
    delete ui;
}
