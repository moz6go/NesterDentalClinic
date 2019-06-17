#include "addeventdialog.h"
#include "ui_addeventdialog.h"


AddEventDialog::AddEventDialog(DataBase* data_base, QVariantList* curr_row, Role role, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddEventDialog)
{
    ui->setupUi(this);
    sdb = data_base;
    row = curr_row;
    ui->date_de->setMinimumDate (QDate::currentDate ());

    if (role == ADD){
        event_id = "";
        ui->time_from_te->setMinimumTime (QTime::currentTime ());
        ui->time_to_te->setMinimumTime(QTime::currentTime());

        if(row->size()) {
            ui->patient_le->setText ((row->at(SURNAME_COL).toString () + " " + row->at (NAME_COL).toString () + " " + row->at (F_NAME_COL).toString ()).simplified ());
        }
        ui->date_de->setDate (QDate::currentDate ());
        ui->time_from_te->setTime (QTime(QTime::currentTime ().hour () + 1, 0));
        ui->time_to_te->setMaximumTime (QTime(23,59));
        if(ui->time_from_te->time ().addSecs (3600) < ui->time_from_te->time ()) {
            ui->time_to_te->setTime (QTime(23,59));
        }
        else {
            ui->time_to_te->setTime (ui->time_from_te->time ().addSecs (3600));
        }
    }
    else {
        setWindowTitle ("Редагування прийому");
        event_id = row->at (EVENT_ID_COL).toString ();
        ui->patient_le->setText(row->at(PATIENT_COL).toString());
        ui->patient_le->setDisabled(true);
        ui->date_de->setDate(row->at(EVENT_DATE_COL).toDate());
        ui->time_from_te->setTime(row->at(EVENT_TIME_FROM_COL).toTime());
        ui->time_to_te->setTime(row->at(EVENT_TIME_TO_COL).toTime());
    }

    QObject::connect (ui->patient_le, &QLineEdit::textChanged, this, &AddEventDialog::EnabledOkButton);
    QObject::connect (ui->time_from_te, &QTimeEdit::timeChanged, this, &AddEventDialog::CheckFreeTime);
    QObject::connect (ui->time_to_te, &QTimeEdit::timeChanged, this, &AddEventDialog::CheckFreeTime);
    QObject::connect (ui->date_de, &QDateEdit::dateChanged, this, &AddEventDialog::CheckFreeTime);
    QObject::connect (ui->date_de, &QDateEdit::dateChanged, this, &AddEventDialog::SetMinTimeFrom);
    QObject::connect (ui->time_from_te, &QTimeEdit::timeChanged, this, &AddEventDialog::SetTimeTo);
    QObject::connect (ui->ok_pb, &QPushButton::clicked, this, &QDialog::accept);
    QObject::connect (ui->cancel_pb, &QPushButton::clicked, this, &QDialog::reject);
    CheckFreeTime();
}

void AddEventDialog::SetTimeTo(QTime time_from){
    ui->time_to_te->setMinimumTime (time_from);
    ui->time_to_te->setTime(time_from.addSecs (3600));
}

void AddEventDialog::SetMinTimeFrom() {
    if (ui->date_de->date() > QDate::currentDate ()){
         ui->time_from_te->setMinimumTime(QTime(0, 0));
         ui->time_to_te->setMinimumTime(QTime(0, 0));
    }
}

void AddEventDialog::EnabledOkButton() {
    ui->ok_pb->setEnabled (ui->patient_le->text ().size () && ui->warning_lbl->text ().isEmpty ());
}

void AddEventDialog::CheckFreeTime() {
    QString patient = sdb->Select(SqlQueries::CheckDateTimeFrom (ui->date_de->date ().toString (SQL_DATE_FORMAT), ui->time_from_te->time ().toString (TIME_FORMAT), event_id));

    if (patient.isEmpty ()){
        patient = sdb->Select(SqlQueries::CheckDateTimeTo (ui->date_de->date ().toString (SQL_DATE_FORMAT),  ui->time_to_te->time ().toString (TIME_FORMAT), event_id));
    }

    if (patient.size ()){
        ui->warning_lbl->setText ("Час зайнятий пацієнтом " + patient);
    }
    else {
        ui->warning_lbl->setText (patient);
    }
    EnabledOkButton();
}

QString AddEventDialog::GetPatient() {
    return ui->patient_le->text ();
}

QString AddEventDialog::GetDate() {
    return ui->date_de->date ().toString (SQL_DATE_FORMAT);
}

QString AddEventDialog::GetTimeFrom() {
    return ui->time_from_te->time ().toString (TIME_FORMAT);
}

QString AddEventDialog::GetTimeTo() {
    return ui->time_to_te->time ().toString (TIME_FORMAT);
}

QString AddEventDialog::GetComment() {
    return ui->comment_te->toPlainText ();
}

QString AddEventDialog::GetPatientId() {
    return ui->patient_le->text () == (row->at(SURNAME_COL).toString () + " " + row->at (NAME_COL).toString () + " " + row->at (F_NAME_COL).toString ()).simplified () ? row->at(PATIENT_ID_COL).toString (): "";
}

AddEventDialog::~AddEventDialog() {
    delete ui;
}
