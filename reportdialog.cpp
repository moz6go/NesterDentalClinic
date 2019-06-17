#include "reportdialog.h"
#include "ui_reportdialog.h"


ReportDialog::ReportDialog(DataBase* data_base, QWidget *parent) :
    QDialog(parent),
    ui_report(new Ui::ReportDialog)
{
    ui_report->setupUi(this);
    setModal (true);
    sdb = data_base;

    ui_report->report_cb->addItems (REPORTS_TYPES);

    ui_report->date_from->setCalendarPopup (true);
    ui_report->date_from->setDisplayFormat (DATE_FORMAT);
    ui_report->date_from->setDate (QDate::currentDate ().addMonths (-1));

    ui_report->date_to->setCalendarPopup (true);
    ui_report->date_to->setDisplayFormat (DATE_FORMAT);
    ui_report->date_to->setDate (QDate::currentDate ());

    QObject::connect (ui_report->ok_pb, &QPushButton::clicked, this, &ReportDialog::accept);
    QObject::connect (ui_report->cancel_pb, &QPushButton::clicked, this, &ReportDialog::reject);
}

QString ReportDialog::GetDateFrom() {
    return ui_report->date_from->date ().toString (SQL_DATE_FORMAT);
}

QString ReportDialog::GetDateTo() {
    return ui_report->date_to->date ().toString (SQL_DATE_FORMAT);
}

int ReportDialog::GetReportType() {
    return ui_report->report_cb->currentIndex ();
}

ReportDialog::~ReportDialog() {
    delete ui_report;
}
