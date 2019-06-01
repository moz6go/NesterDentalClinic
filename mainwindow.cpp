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
    ui->search_cb->addItems (QStringList() << PATIENTS_TABLE_HEADERS[2] << PATIENTS_TABLE_HEADERS[3] << PATIENTS_TABLE_HEADERS[4]) ;

    patients_model = new QSqlTableModel(this);
    patients_model->setTable (PATIENTS_TABLE);
    RenameHeaders(patients_model->columnCount(), patients_model, PATIENTS_TABLE_HEADERS);
    patients_filter_model = new QSortFilterProxyModel(this);
    patients_filter_model->setSourceModel (patients_model);
    patients_filter_model->setFilterCaseSensitivity (Qt::CaseInsensitive);
    patients_filter_model->setFilterKeyColumn (BY_SURNAME);
    ui->patients_table->setModel (patients_filter_model);

    events_model = new QSqlTableModel(this);
    events_model->setTable(EVENTS_TABLE);
    RenameHeaders(events_model->columnCount(), events_model, EVENTS_TABLE_HEADERS);
    events_filter_model = new QSortFilterProxyModel(this);
    events_filter_model->setSourceModel(events_model);
    events_filter_model->setFilterKeyColumn(1);
    events_filter_model->setFilterFixedString(QDate::currentDate().toString("yyyy-MM-dd"));
    ui->events_table->setModel(events_filter_model);

    for (int col = 0; col < patients_model->columnCount(); ++col) {
        ui->patients_table->setColumnWidth(col,  ui->patients_table->width () / 3);
        if (!(col == 2 || col == 3 || col == 4)){
            ui->patients_table->setColumnHidden(col, true);
        }
    }
    for (int col = 0; col < events_model->columnCount(); ++col) {
        if (!(col == 2 || col == 3 || col == 4 || col == 5)){
            ui->events_table->setColumnHidden(col, true);
        }
    }

    TableInit(ui->patients_table);
    TableInit(ui->events_table);

    QObject::connect (ui->search_cb, &QComboBox::currentTextChanged, this, &MainWindow::SetSearchType);
    QObject::connect (ui->search_le, &QLineEdit::textChanged, this, &MainWindow::SearchTextChanged);
    QObject::connect (ui->patients_table, &QTableView::clicked, this, &MainWindow::ShowPatientInfo);
    QObject::connect (ui->edit_client_pb, &QPushButton::clicked, this, &MainWindow::onEditClientClicked);

    QObject::connect (ui->calendar, &QCalendarWidget::clicked, this, &MainWindow::DateSelected );
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
    action_add_event->setDisabled (true);
    toolbar->addSeparator ();

    toolbar->setMovable (false);
    toolbar->setIconSize (QSize(SIZE_WID_1, SIZE_WID_1));
    addToolBar(Qt::TopToolBarArea, toolbar);
}

void MainWindow::RenameHeaders(int column_count, QSqlTableModel *model, const QStringList &headers_list) {
    for(int col = 0; col < column_count; ++col) {
        model->setHeaderData(col, Qt::Horizontal, headers_list[col]);
    }
}

void MainWindow::TableInit(QTableView *table) {
    table->verticalHeader ()->setSectionResizeMode (QHeaderView::Fixed);
    table->verticalHeader ()->setDefaultSectionSize (18);
    table->verticalHeader()->setVisible(false);
    table->setShowGrid(false);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->horizontalHeader()->setStretchLastSection(true);
    table->setSortingEnabled (true);
}

void MainWindow::Update(int row) {
    events_model->select();
    patients_model->select ();
    patients_model->sort (0, Qt::AscendingOrder);
    ui->patients_table->selectRow (row);
    ui->edit_client_pb->setEnabled (patients_model->rowCount ());
    ShowPatientInfo();
}

void MainWindow::onActionAddPatient() {
    AddPatientDialog* add_patient = new AddPatientDialog (QVariantList(), this);
    if(add_patient->exec () == QDialog::Accepted) {
        QPixmap pic(add_patient->GetPhotoPath ());
        QByteArray pic_byte_arr;
        QBuffer buff(&pic_byte_arr);
        buff.open (QIODevice::WriteOnly);
        pic.save (&buff, "JPG");

        QVariantList data = { QDateTime::currentDateTime ().toString ("yyyy-MM-dd hh:mm:ss"),
                              add_patient->GetSurname (),
                              add_patient->GetName (),
                              add_patient->GetFName (),
                              add_patient->GetBDate (),
                              add_patient->GetSex(),
                              add_patient->GetCity(),
                              add_patient->GetTelNumber (),
                              add_patient->GetIllnesses(),
                              pic_byte_arr };

        QStringList columns = { INIT_DATE, SURNAME, NAME, F_NAME, B_DATE, SEX, CITY, TEL_NUMBER, ILLNESSES, PATIENT_PHOTO };
        if (!sdb->UpdateInsertData (sdb->GenerateInsertQuery (PATIENTS_TABLE, columns),
                                       sdb->GenerateBindValues (columns),
                                       data)) {
            ui->statusBar->showMessage ("Невдалось створити картку пацієнта! Проблема з підключеням до бази даних");
            return;
        }
        Update(patients_filter_model->rowCount ());
        ui->statusBar->showMessage ("Додано пацієнта " + add_patient->GetSurname () + " " + add_patient->GetName ().left (1) + "." + add_patient->GetFName().left(1) + ".");
    }
}

void MainWindow::onActionAddEvent() {

}

void MainWindow::onEditClientClicked() {
    QVariantList row = sdb->SelectRow ("*", PATIENTS_TABLE, PATIENT_ID, patients_filter_model->data(patients_filter_model->index (ui->patients_table->currentIndex ().row (), 0)).toString (), patients_model->columnCount());
    AddPatientDialog* edit_patient = new AddPatientDialog (row, this);
    if(edit_patient->exec() == QDialog::Accepted){
        QPixmap pic(edit_patient->GetPhotoPath ());
        QByteArray pic_byte_arr;
        QBuffer buff(&pic_byte_arr);
        buff.open (QIODevice::WriteOnly);
        pic.save (&buff, "JPG");
        QVariantList data = { QDateTime::currentDateTime ().toString ("yyyy-MM-dd hh:mm:ss"),
                              edit_patient->GetSurname (),
                              edit_patient->GetName (),
                              edit_patient->GetFName (),
                              edit_patient->GetBDate (),
                              edit_patient->GetSex(),
                              edit_patient->GetCity(),
                              edit_patient->GetTelNumber (),
                              edit_patient->GetIllnesses() };
        QStringList columns = { INIT_DATE, SURNAME, NAME, F_NAME, B_DATE, SEX, CITY, TEL_NUMBER, ILLNESSES };

        if (!edit_patient->GetPhotoPath ().isEmpty ()){
            data.append ( pic_byte_arr);
            columns.append (PATIENT_PHOTO);
        }

        if (!sdb->UpdateInsertData (sdb->GenerateUpdateQuery (PATIENTS_TABLE, columns, PATIENT_ID, row.at(0).toString ()),
                                    sdb->GenerateBindValues (columns),
                                    data)) {
            ui->statusBar->showMessage ("Невдалось відредагувати картку пацієнта! Проблема з підключеням до бази даних");
            return;
        }
        Update(ui->patients_table->currentIndex ().row ());
        ui->statusBar->showMessage ("Відредаговано картку пацієнта " + edit_patient->GetSurname () + " " + edit_patient->GetName ().left (1) + "." + edit_patient->GetFName().left(1) + ".");
    }
}

void MainWindow::DateSelected() {
    events_filter_model->setFilterFixedString(ui->calendar->selectedDate().toString("yyyy-MM-dd"));
}

void MainWindow::ShowPatientInfo() {
    patient_photo.loadFromData (patients_filter_model->data(patients_filter_model->index (ui->patients_table->currentIndex ().row (), 10)).toByteArray ());
    if (!patient_photo.isNull ()) {
        ui->patient_photo_lbl->setPixmap (patient_photo.scaledToWidth (ui->patient_photo_lbl->width ()));
    }
    else {
        ui->patient_photo_lbl->setPixmap (QPixmap(":/action_icons/default_user.png"));
    }
    ui->surname_le->setText(patients_filter_model->data(patients_filter_model->index (ui->patients_table->currentIndex ().row (), 2)).toString ());
    ui->name_le->setText(patients_filter_model->data(patients_filter_model->index (ui->patients_table->currentIndex ().row (), 3)).toString ());
    ui->f_name_le->setText(patients_filter_model->data(patients_filter_model->index (ui->patients_table->currentIndex ().row (), 4)).toString ());
    ui->b_date_le->setText(QDate::fromString (patients_filter_model->data(patients_filter_model->index (ui->patients_table->currentIndex ().row (), 5)).toString (), "yyyy-MM-dd").toString ("dd.MM.yyyy"));
    ui->sex_le->setText(patients_filter_model->data(patients_filter_model->index (ui->patients_table->currentIndex ().row (), 6)).toString ());
    ui->city_le->setText(patients_filter_model->data(patients_filter_model->index (ui->patients_table->currentIndex ().row (), 7)).toString ());
    ui->tel_number_le->setText(patients_filter_model->data(patients_filter_model->index (ui->patients_table->currentIndex ().row (), 8)).toString ());
    ui->illnesses_le->setText(patients_filter_model->data(patients_filter_model->index (ui->patients_table->currentIndex ().row (), 9)).toString ());
}

void MainWindow::SetSearchType(QString type) {
    if(type == PATIENTS_TABLE_HEADERS[2]) {
        patients_filter_model->setFilterKeyColumn (BY_SURNAME);
    }
    else if (type == PATIENTS_TABLE_HEADERS[3]) {
        patients_filter_model->setFilterKeyColumn (BY_NAME);
    }
    else if (type == PATIENTS_TABLE_HEADERS[4]) {
        patients_filter_model->setFilterKeyColumn (BY_F_NAME);
    }
}

void MainWindow::SearchTextChanged(QString text) {
    patients_filter_model->setFilterFixedString (text);
    Update(ui->patients_table->currentIndex ().row ());
}

MainWindow::~MainWindow() {
    delete ui;
}

