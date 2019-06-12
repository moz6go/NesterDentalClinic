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
    }

    QObject::connect(ui->ok_pb, &QPushButton::clicked, this, &AppointmentDialog::accept);
    QObject::connect(ui->cancel_pb, &QPushButton::clicked, this, &AppointmentDialog::reject);
}

double AppointmentDialog::GetPrice() {
    return ui->price_sb->value();
}

QString AppointmentDialog::GetResults() {
    return ui->results_te->toPlainText();
}

AppointmentDialog::~AppointmentDialog() {
    delete ui;
}
