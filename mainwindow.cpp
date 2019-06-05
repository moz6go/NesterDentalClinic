#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(DataBase* data_base, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    sdb = data_base;
    patient_photo = QPixmap(":/action_icons/default_user.png");
    toolbar = new QToolBar(this);
    BuildToolBar ();
    ui->search_cb->addItems (QStringList()
                             << PATIENTS_HEADERS[SURNAME_COL]
                             << PATIENTS_HEADERS[NAME_COL]
                             << PATIENTS_HEADERS[F_NAME_COL]) ;

    patients_model = new QSqlTableModel(this);
    patients_model->setTable (PATIENTS_TABLE);
    RenameHeaders(patients_model->columnCount(), patients_model, PATIENTS_HEADERS);
    patients_filter_model = new QSortFilterProxyModel(this);
    patients_filter_model->setSourceModel (patients_model);
    patients_filter_model->setFilterCaseSensitivity (Qt::CaseInsensitive);
    patients_filter_model->setFilterKeyColumn (SURNAME_COL);
    ui->patients_table->setModel (patients_filter_model);

    events_model = new QSqlTableModel(this);
    events_model->setTable(EVENTS_TABLE);
    RenameHeaders(events_model->columnCount(), events_model, EVENTS_TABLE_HEADERS);
    events_filter_model = new QSortFilterProxyModel(this);
    events_filter_model->setSourceModel(events_model);
    events_filter_model->setFilterKeyColumn(EVENT_DATE_COL);
    events_filter_model->setFilterFixedString(QDate::currentDate().toString(SQL_DATE_FORMAT));
    ui->events_table->setModel(events_filter_model);

    for (int col = 0; col < patients_model->columnCount(); ++col) {
        ui->patients_table->setColumnWidth(col,  ui->patients_table->width () / 3);
        if (!(col == SURNAME_COL || col == NAME_COL || col == F_NAME_COL)){
            ui->patients_table->setColumnHidden(col, true);
        }
    }
    for (int col = 0; col < events_model->columnCount(); ++col) {
        if (col == EVENT_ID_COL || col == INIT_DATE_COL || col == EVENT_PATIENT_ID_COL){
            ui->events_table->setColumnHidden(col, true);
        }
    }

    TableInit(ui->patients_table);
    TableInit(ui->events_table);
    ui->patients_table->setShowGrid(false);
    ui->events_table->setColumnWidth (EVENT_DATE_COL,70);
    ui->events_table->setColumnWidth(EVENT_TIME_FROM_COL, 40);
    ui->events_table->setColumnWidth(EVENT_TIME_TO_COL, 40);
    ui->events_table->setColumnWidth(PATIENT_COL, 250);

    QObject::connect (ui->search_cb, &QComboBox::currentTextChanged, this, &MainWindow::SetSearchType);
    QObject::connect (ui->search_le, &QLineEdit::textChanged, this, &MainWindow::SearchTextChanged);
    QObject::connect (ui->patients_table, &QTableView::clicked, this, &MainWindow::ShowPatientInfo);
    QObject::connect (ui->patients_table, &QTableView::clicked, this, &MainWindow::ShowEventsBySelectedPatient);

    QObject::connect (ui->calendar, &QCalendarWidget::clicked, this, &MainWindow::ShowEventsBySelectedDate );

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
    action_edit_patient = toolbar->addAction(QPixmap(":/action_icons/edit_patient.png"), "Редагувати дані пацієнта", this, SLOT(onActionEditClient()));
    action_tooth_card = toolbar->addAction(QPixmap(":/action_icons/tooth_card.png"), "Переглянути зубну картку пацієнта", this, SLOT(onActionToothCard()));
    action_visit_history = toolbar->addAction(QPixmap(":/action_icons/med_journal.png"), "Переглянути зубну картку пацієнта", this, SLOT(onActionVisitHistory()));
    toolbar->addSeparator ();
    action_add_event = toolbar->addAction(QPixmap(":/action_icons/add_event.png"), "Записати на прийом", this, SLOT(onActionAddEvent()));

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
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->horizontalHeader()->setStretchLastSection(true);
    table->setSortingEnabled (true);
}

void MainWindow::Update(int row) {
    events_model->select();
    patients_model->select ();
    patients_model->sort (PATIENT_ID_COL, Qt::AscendingOrder);
    ui->patients_table->selectRow (row);
    action_edit_patient->setEnabled (patients_model->rowCount ());
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

        QVariantList data = { QDateTime::currentDateTime ().toString (SQL_DATE_TIME_FORMAT),
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
    QVariantList row = patients_model->rowCount () ? sdb->SelectRow ("*", PATIENTS_TABLE, PATIENT_ID, patients_filter_model->data(patients_filter_model->index (ui->patients_table->currentIndex ().row (), PATIENT_ID_COL)).toString (), patients_model->columnCount()) : QVariantList();
    AddEventDialog* add_event = new AddEventDialog(sdb, &row, this);
    if(add_event->exec () == QDialog::Accepted){
        QVariantList data = { QDateTime::currentDateTime ().toString (SQL_DATE_TIME_FORMAT),
                              add_event->GetDate (),
                              add_event->GetTimeFrom (),
                              add_event->GetTimeTo (),
                              add_event->GetPatient (),
                              STATUS_LIST[ACTIVE],
                              add_event->GetComment (),
                              add_event->GetPatientId ()
                            };
        QStringList columns = { EVENT_INIT_DATE, EVENT_DATE, EVENT_TIME_FROM, EVENT_TIME_TO, PATIENT, EVENT_STATUS, COMMENT, PATIENT_ID };

        if (!sdb->UpdateInsertData (sdb->GenerateInsertQuery (EVENTS_TABLE, columns),
                                    sdb->GenerateBindValues (columns),
                                    data)) {
            QMessageBox::critical (this, "Error!", "Невдалось записати на прийом! Проблема з підключеням до бази даних");
            ui->statusBar->showMessage ("Невдалось записати на прийом! Проблема з підключеням до бази даних");
            return;
        }
        Update(ui->patients_table->currentIndex ().row ());
        ui->statusBar->showMessage (add_event->GetPatient () + " записано на прийом " + add_event->GetDate () + " о " + add_event->GetTimeFrom ());
    }
}

void MainWindow::onActionEditClient() {
    QVariantList row = sdb->SelectRow ("*", PATIENTS_TABLE, PATIENT_ID, patients_filter_model->data(patients_filter_model->index (ui->patients_table->currentIndex ().row (), PATIENT_ID_COL)).toString (), patients_model->columnCount());
    AddPatientDialog* edit_patient = new AddPatientDialog (row, this);
    if(edit_patient->exec() == QDialog::Accepted){
        QPixmap pic(edit_patient->GetPhotoPath ());
        QByteArray pic_byte_arr;
        QBuffer buff(&pic_byte_arr);
        buff.open (QIODevice::WriteOnly);
        pic.save (&buff, "JPG");
        QVariantList data = { QDateTime::currentDateTime ().toString (SQL_DATE_TIME_FORMAT),
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

void MainWindow::onActionToothCard() {

}

void MainWindow::onActionVisitHistory() {

}

void MainWindow::ShowEventsBySelectedDate() {
    events_filter_model->setFilterKeyColumn(EVENT_DATE_COL);
    events_filter_model->setFilterFixedString(ui->calendar->selectedDate().toString(SQL_DATE_FORMAT));
}

void MainWindow::ShowEventsBySelectedPatient() {
    events_filter_model->setFilterKeyColumn(EVENT_PATIENT_ID_COL);
    events_filter_model->setFilterFixedString(patients_filter_model->data(patients_filter_model->index (ui->patients_table->currentIndex ().row (), PATIENT_ID_COL)).toString ());
}

void MainWindow::ShowPatientInfo() {
    patient_photo.loadFromData (patients_filter_model->data(patients_filter_model->index (ui->patients_table->currentIndex ().row (), PATIENT_PHOTO_COL)).toByteArray ());
    if (!patient_photo.isNull ()) {
        ui->patient_photo_lbl->setPixmap (patient_photo.scaledToWidth (ui->patient_photo_lbl->width ()));
    }
    else {
        ui->patient_photo_lbl->setPixmap (QPixmap(":/action_icons/default_user.png"));
    }
    ui->surname_le->    setText(patients_filter_model->data(patients_filter_model->index (ui->patients_table->currentIndex ().row (), SURNAME_COL)).toString ());
    ui->name_le->       setText(patients_filter_model->data(patients_filter_model->index (ui->patients_table->currentIndex ().row (), NAME_COL)).toString ());
    ui->f_name_le->     setText(patients_filter_model->data(patients_filter_model->index (ui->patients_table->currentIndex ().row (), F_NAME_COL)).toString ());
    ui->b_date_le->     setText(QDate::fromString (patients_filter_model->data(patients_filter_model->index (ui->patients_table->currentIndex ().row (), B_DATE_COL)).toString (), SQL_DATE_FORMAT).toString (DATE_FORMAT));
    ui->sex_le->        setText(patients_filter_model->data(patients_filter_model->index (ui->patients_table->currentIndex ().row (), SEX_COL)).toString ());
    ui->city_le->       setText(patients_filter_model->data(patients_filter_model->index (ui->patients_table->currentIndex ().row (), CITY_COL)).toString ());
    ui->tel_number_le-> setText(patients_filter_model->data(patients_filter_model->index (ui->patients_table->currentIndex ().row (), TEL_NUMBER_COL)).toString ());
    ui->illnesses_le->  setText(patients_filter_model->data(patients_filter_model->index (ui->patients_table->currentIndex ().row (), ILLNESSES_COL)).toString ());
}

void MainWindow::SetSearchType(QString type) {
    if(type == PATIENTS_HEADERS[SURNAME_COL]) {
        patients_filter_model->setFilterKeyColumn (SURNAME_COL);
    }
    else if (type == PATIENTS_HEADERS[NAME_COL]) {
        patients_filter_model->setFilterKeyColumn (NAME_COL);
    }
    else if (type == PATIENTS_HEADERS[F_NAME_COL]) {
        patients_filter_model->setFilterKeyColumn (F_NAME_COL);
    }
}

void MainWindow::SearchTextChanged(QString text) {
    patients_filter_model->setFilterFixedString (text);
    Update(ui->patients_table->currentIndex ().row ());
}

MainWindow::~MainWindow() {
    delete ui;
}

