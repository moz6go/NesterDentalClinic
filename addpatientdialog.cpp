#include "addpatientdialog.h"
#include "ui_addpatientdialog.h"


AddPatientDialog::AddPatientDialog(DataBase* data_base, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddPatientDialog)
{
    ui->setupUi(this);
    setModal (true);
    photo_path = "";
    sdb = data_base;

    ui->sex_cb->addItems (QStringList() << "Чоловіча" << "Жіноча");
    ui->tel_number_le->setInputMask ("38 (000) 000 00 00;_");

    QObject::connect (ui->load_photo_pb, &QPushButton::clicked, this, &AddPatientDialog::LoadPhoto);
    QObject::connect (ui->s_name_le, &QLineEdit::textChanged, this, &AddPatientDialog::EnableAddButton);
    QObject::connect (ui->name_le, &QLineEdit::textChanged, this, &AddPatientDialog::EnableAddButton);
    QObject::connect (ui->f_name_le, &QLineEdit::textChanged, this, &AddPatientDialog::EnableAddButton);
    QObject::connect (ui->add_pb, &QPushButton::clicked, this, &QDialog::accept);
    QObject::connect (ui->cancel_pb, &QPushButton::clicked, this, &QDialog::reject);
}

void AddPatientDialog::LoadPhoto() {
    photo_path = QFileDialog::getOpenFileName (this, "Виберіть фото моделі", QDir::homePath (), "*.jpg *.png *.bmp");
    if (!photo_path.isEmpty ()){
        ui->patient_photo_lbl->setPixmap (QPixmap(photo_path).scaledToWidth (ui->patient_photo_lbl->width ()));
    }
}

void AddPatientDialog::EnableAddButton() {
    ui->add_pb->setEnabled (!ui->s_name_le->text ().isEmpty () &&
                            !ui->name_le->text ().isEmpty () &&
                            !ui->f_name_le->text ().isEmpty ());
}

QString AddPatientDialog::GetPhotoPath() {
    return photo_path;
}

QString AddPatientDialog::GetSurname() {
    return ui->s_name_le->text ();
}

QString AddPatientDialog::GetName() {
    return ui->name_le->text ();
}

QString AddPatientDialog::GetFName() {
    return ui->f_name_le->text ();
}

QString AddPatientDialog::GetBDate() {
    return ui->b_date_de->date ().toString ("yyyy-MM-dd");
}

QString AddPatientDialog::GetSex() {
    return  ui->sex_cb->currentText ();
}

QString AddPatientDialog::GetCity() {
    return ui->city_le->text ();
}

QString AddPatientDialog::GetTelNumber() {
    return ui->tel_number_le->text ();
}

AddPatientDialog::~AddPatientDialog() {
    delete ui;
}
