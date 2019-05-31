#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "addpatientdialog.h"


MainWindow::MainWindow(DataBase* data_base, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    sdb = data_base;
    patient_photo = QPixmap(":/action_icons/default_user.png");
    toolbar = new QToolBar(this);
    BuildToolBar ();

    sql_model = new QSqlTableModel(this);
    sql_model->setTable (PATIENTS_TABLE);

    filter_model = new QSortFilterProxyModel(this);
    filter_model->setSourceModel (sql_model);
    filter_model->setFilterCaseSensitivity (Qt::CaseInsensitive);
    filter_model->setFilterKeyColumn (BY_SURNAME);
    ui->patients_table->setModel (filter_model);

    for(int col = 0; col < 3; ++col) {
        sql_model->setHeaderData(col + 2, Qt::Horizontal, PATIENTS_TABLE_HEADERS[col]);
    }
    ui->search_cb->addItems (PATIENTS_TABLE_HEADERS);
    PatientTableInit();

    QObject::connect (ui->search_cb, &QComboBox::currentTextChanged, this, &MainWindow::SetSearchType);
    QObject::connect (ui->search_le, &QLineEdit::textChanged, this, &MainWindow::SearchTextChanged);
    QObject::connect (ui->patients_table, &QTableView::clicked, this, &MainWindow::ShowPatientInfo);
    Update(0);
}

void MainWindow::resizeEvent(QResizeEvent *event) {
    if (!patient_photo.isNull ()){
        ui->patient_photo_lbl->setPixmap (patient_photo.scaledToWidth (ui->patient_photo_lbl->width ()));
    }
    QMainWindow::resizeEvent(event);
}

void MainWindow::BuildToolBar() {
    action_add_patient = toolbar->addAction(QPixmap(":/action_icons/add_patient.png"), "Додати пацієнта", this, SLOT(onActionAddPatient()));
    action_add_event = toolbar->addAction(QPixmap(":/action_icons/add_event.png"), "Записати на прийом", this, SLOT(onActionAddEvent()));
    toolbar->addSeparator ();

    toolbar->setMovable (false);
    toolbar->setIconSize (QSize(SIZE_WID_1, SIZE_WID_1));
    addToolBar(Qt::TopToolBarArea, toolbar);
}

void MainWindow::PatientTableInit() {
    for (int col = 0; col < sql_model->columnCount(); ++col) {
        ui->patients_table->setColumnWidth(col,  ui->patients_table->width () / 3);
        if (!(col == 2 || col == 3 || col == 4)){
            ui->patients_table->setColumnHidden(col, true);
        }
    }
    ui->patients_table->verticalHeader ()->setSectionResizeMode (QHeaderView::Fixed);
    ui->patients_table->verticalHeader ()->setDefaultSectionSize (18);
    ui->patients_table->verticalHeader()->setVisible(false);
    ui->patients_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->patients_table->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->patients_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->patients_table->horizontalHeader()->setStretchLastSection(true);
    ui->patients_table->setSortingEnabled (true);
}

void MainWindow::Update(int row) {
    sql_model->select ();
    sql_model->sort (0, Qt::AscendingOrder);
    ui->patients_table->selectRow (row);
    ShowPatientInfo();
}

void MainWindow::onActionAddPatient() {
    AddPatientDialog* add_patient = new AddPatientDialog (sdb, this);
    if(add_patient->exec () == QDialog::Accepted) {
        QPixmap pic(add_patient->GetPhotoPath ());
        QByteArray pic_byte_arr;
        QBuffer buff(&pic_byte_arr);
        buff.open (QIODevice::WriteOnly);
        pic.save (&buff, "JPG");

        QVariantList data = QVariantList() << QDateTime::currentDateTime ().toString ("yyyy-MM-dd hh:mm:ss")
                                           << add_patient->GetSurname ()
                                           << add_patient->GetName ()
                                           << add_patient->GetFName ()
                                           << add_patient->GetBDate ()
                                           << add_patient->GetSex()
                                           << add_patient->GetCity()
                                           << add_patient->GetTelNumber ()
                                           << pic_byte_arr;
        QStringList columns = {INIT_DATE, SURNAME, NAME, F_NAME, B_DATE, SEX, CITY, TEL_NUMBER, PATIENT_PHOTO};
        if (!sdb->InsertDataIntoTable (sdb->GenerateInsertQuery (PATIENTS_TABLE, columns),
                                       sdb->GenerateBindValues (columns),
                                       data)) {
            ui->statusBar->showMessage ("Невдалось створити картку пацієнта! Проблема з підключеням до бази даних");
            return;
        }
        Update(filter_model->rowCount ());
        ui->statusBar->showMessage ("Додано пацієнта " + add_patient->GetSurname () + " " + add_patient->GetName ().left (1) + ".");
    }
}

void MainWindow::onActionAddEvent() {

}

void MainWindow::ShowPatientInfo() {
    patient_photo.loadFromData (filter_model->data(filter_model->index (ui->patients_table->currentIndex ().row (), 9)).toByteArray ());
    if (!patient_photo.isNull ()) {
        ui->patient_photo_lbl->setPixmap (patient_photo.scaledToWidth (ui->patient_photo_lbl->width ()));
    }
    else {
        ui->patient_photo_lbl->setPixmap (QPixmap(":/action_icons/default_user.png"));
    }
    ui->surname_le->setText(filter_model->data(filter_model->index (ui->patients_table->currentIndex ().row (), 2)).toString ());
    ui->name_le->setText(filter_model->data(filter_model->index (ui->patients_table->currentIndex ().row (), 3)).toString ());
    ui->f_name_le->setText(filter_model->data(filter_model->index (ui->patients_table->currentIndex ().row (), 4)).toString ());
    ui->b_date_le->setText(filter_model->data(filter_model->index (ui->patients_table->currentIndex ().row (), 5)).toString ());
    ui->sex_le->setText(filter_model->data(filter_model->index (ui->patients_table->currentIndex ().row (), 6)).toString ());
    ui->city_le->setText(filter_model->data(filter_model->index (ui->patients_table->currentIndex ().row (), 7)).toString ());
    ui->tel_number_le->setText(filter_model->data(filter_model->index (ui->patients_table->currentIndex ().row (), 8)).toString ());
}

void MainWindow::SetSearchType(QString type) {
    if(type == PATIENTS_TABLE_HEADERS[0]) {
        filter_model->setFilterKeyColumn (BY_SURNAME);
    }
    else if (type == PATIENTS_TABLE_HEADERS[1]) {
        filter_model->setFilterKeyColumn (BY_NAME);
    }
    else if (type == PATIENTS_TABLE_HEADERS[2]) {
        filter_model->setFilterKeyColumn (BY_F_NAME);
    }
}

void MainWindow::SearchTextChanged(QString text) {
    filter_model->setFilterFixedString (text);
    Update(ui->patients_table->currentIndex ().row ());
}

MainWindow::~MainWindow() {
    delete ui;
}
