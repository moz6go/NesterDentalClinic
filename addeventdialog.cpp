#include "addeventdialog.h"
#include "ui_addeventdialog.h"

AddEventDialog::AddEventDialog(const QVariantList &curr_row, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddEventDialog)
{
    ui->setupUi(this);
    row = curr_row;
    ui->date_de->setDate (QDate::currentDate ());
    ui->time_te->setTime (QTime(12,00));
    ui->patient_le->setText ((row.at(SURNAME_COL).toString () + " " + row.at (NAME_COL).toString () + " " + row.at (F_NAME_COL).toString ()).simplified ());

    QObject::connect (ui->ok_pb, &QPushButton::clicked, this, &QDialog::accept);
    QObject::connect (ui->cancel_pb, &QPushButton::clicked, this, &QDialog::reject);

}

QString AddEventDialog::GetPatient() {
    return ui->patient_le->text ();
}

QString AddEventDialog::GetDate() {
    return ui->date_de->date ().toString (SQL_DATE_FORMAT);
}

QString AddEventDialog::GetTime() {
    return ui->time_te->time ().toString (SQL_TIME_FORMAT);
}

QString AddEventDialog::GetComment() {
    return ui->comment_te->toPlainText ();
}

QString AddEventDialog::GetPatientId() {
    return ui->patient_le->text () == (row.at(SURNAME_COL).toString () + " " + row.at (NAME_COL).toString () + " " + row.at (F_NAME_COL).toString ()).simplified () ? row.at(PATIENT_ID_COL).toString (): "";
}



AddEventDialog::~AddEventDialog() {
    delete ui;
}
