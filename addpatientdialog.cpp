#include "addpatientdialog.h"
#include "ui_addpatientdialog.h"


AddPatientDialog::AddPatientDialog(DataBase *data_base, const QVariantList& curr_row, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddPatientDialog)
{
    ui->setupUi(this);
    setModal (true);
    sdb = data_base;
    row = curr_row;
    photo_path = "";
    ui->sex_cb->addItems (QStringList() << "Чоловіча" << "Жіноча");
    ui->tel_number_le->setInputMask ("38 (000) 000 00 00;_");

    if (!row.isEmpty ()){
        ui->s_name_le->setText (row.at (SURNAME_COL).toString ());
        ui->name_le->setText (row.at (NAME_COL).toString ());
        ui->f_name_le->setText (row.at (F_NAME_COL).toString ());
        ui->b_date_de->setDate (row.at (B_DATE_COL).toDate ());
        ui->sex_cb->setCurrentText (row.at (SEX_COL).toString ());
        ui->city_le->setText (row.at (CITY_COL).toString ());
        ui->tel_number_le->setText (row.at (TEL_NUMBER_COL).toString ());
        ui->illnesses_te->setText(row.at(ILLNESSES_COL).toString());

        QPixmap photo;
        photo.loadFromData (row.at (PATIENT_PHOTO_COL).toByteArray ());
        if (!photo.isNull ()){
            ui->patient_photo_lbl->setPixmap(photo.scaledToWidth (ui->patient_photo_lbl->width ()));
        }
        else if (row.at (SEX_COL).toString () == "Чоловіча") {
            ui->patient_photo_lbl->setPixmap(QPixmap(":/action_icons/default_user.png"));
        }
        else {
            ui->patient_photo_lbl->setPixmap(QPixmap(":/action_icons/default_user_female.png"));
        }

        setWindowTitle ("Редагувати картку клієнта");
        EnableAddButton();
    }

    QObject::connect (ui->sex_cb, &QComboBox::currentTextChanged, this, &AddPatientDialog::SexChanged);
    QObject::connect (ui->load_photo_pb, &QPushButton::clicked, this, &AddPatientDialog::LoadPhoto);
    QObject::connect (ui->s_name_le, &QLineEdit::textChanged, this, &AddPatientDialog::EnableAddButton);
    QObject::connect (ui->name_le, &QLineEdit::textChanged, this, &AddPatientDialog::EnableAddButton);
    QObject::connect (ui->f_name_le, &QLineEdit::textChanged, this, &AddPatientDialog::EnableAddButton);
    QObject::connect (ui->b_date_de, &QDateEdit::dateChanged, this, &AddPatientDialog::EnableAddButton);
    QObject::connect (ui->add_pb, &QPushButton::clicked, this, &QDialog::accept);
    QObject::connect (ui->cancel_pb, &QPushButton::clicked, this, &QDialog::reject);
}

void AddPatientDialog::SexChanged(QString sex) {
    if(photo_path.isEmpty()){
        if(sex == "Чоловіча"){
            ui->patient_photo_lbl->setPixmap(QPixmap(":/action_icons/default_user.png"));
        }
        else {
            ui->patient_photo_lbl->setPixmap(QPixmap(":/action_icons/default_user_female.png"));
        }
    }
}

void AddPatientDialog::LoadPhoto() {
    photo_path = QFileDialog::getOpenFileName (this, "Виберіть фото моделі", QDir::homePath (), "*.jpg *.png *.bmp");
    if (QFile(photo_path).size () > 500000){
        QMessageBox::warning (this, "Розмір фото", "Неможливо завантажити вибране фото!\nРозмір файлу з фото не повинен перевищувати 500,0 Кбайт");
        return;
    }
    if (!photo_path.isEmpty ()) {
        ui->patient_photo_lbl->setPixmap (QPixmap(photo_path).scaledToWidth (ui->patient_photo_lbl->width ()));
    }
}

void AddPatientDialog::EnableAddButton() {
    QString duplicate_patient_id = sdb->Select(SqlQueries::CheckPatientDuplication (ui->s_name_le->text (), ui->name_le->text (), ui->f_name_le->text (), ui->b_date_de->date ().toString (SQL_DATE_FORMAT)));
    QString curr_patient_id = row.isEmpty () ? "" : row.at (PATIENT_ID_COL).toString ();
    bool duplicate_condition = row.isEmpty () ? duplicate_patient_id.isEmpty() : duplicate_patient_id.isEmpty() || curr_patient_id == duplicate_patient_id ;

    if (duplicate_condition) {
        ui->warning_lbl->setText ("");
    }
    else {
        ui->warning_lbl->setText ("Такий пацієнт вже існує!");
    }

    ui->add_pb->setEnabled (!ui->s_name_le->text ().isEmpty () && !ui->name_le->text ().isEmpty () && duplicate_condition);
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
    return ui->tel_number_le->text ().size () < 18 ? "" : ui->tel_number_le->text ();
}

QString AddPatientDialog::GetIllnesses() {
    return ui->illnesses_te->toPlainText();
}

AddPatientDialog::~AddPatientDialog() {
    delete ui;
}
