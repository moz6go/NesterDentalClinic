#include "copyrestoredbdialog.h"
#include "ui_copyrestoredbdialog.h"

CopyRestoreDbDialog::CopyRestoreDbDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CopyRestoreDbDialog)
{
    ui->setupUi(this);
    ui->options_cb->addItems (QStringList()<< CREATE_COPY_DB << RESTORE_DB);

    QObject::connect (ui->ok_pb, &QPushButton::clicked, this, &CopyRestoreDbDialog::accept);
    QObject::connect (ui->cancel_pb, &QPushButton::clicked, this, &CopyRestoreDbDialog::reject);
}

QString CopyRestoreDbDialog::GetOption() {
    return ui->options_cb->currentText ();
}

CopyRestoreDbDialog::~CopyRestoreDbDialog() {
    delete ui;
}
