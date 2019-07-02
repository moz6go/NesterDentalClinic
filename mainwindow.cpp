#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(DataBase* data_base, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    sdb = data_base;

    patient_active_row = 0;
    event_active_row = 0;

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
    events_filter_model = new MyEventsProxyModel(this);
    events_filter_model->setSourceModel(events_model);
    events_filter_model->setFilterKeyColumn(EVENT_DATE_COL);
    events_filter_model->setFilterFixedString(QDate::currentDate().toString(SQL_DATE_FORMAT));
    ui->events_table->setModel(events_filter_model);

    for (int col = 0; col < patients_model->columnCount(); ++col) {
        if (!(col == SURNAME_COL || col == NAME_COL || col == F_NAME_COL)){
            ui->patients_table->setColumnHidden(col, true);
        }
        else {
            ui->patients_table->setColumnWidth(col, ui->patients_table->width () / 3);
        }
    }

    for (int col = 0; col < events_model->columnCount(); ++col) {
        if (col == EVENT_ID_COL || col == PATIENT_INIT_DATE_COL || col == EVENT_LAST_CHANGES_COL || col == EVENT_PATIENT_ID_COL){
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

    QTimer* timer = new QTimer(this);
    QObject::connect (timer, &QTimer::timeout, this, [=] { Update(ui->patients_table->currentIndex ().row ()); });
    QObject::connect (ui->search_cb, &QComboBox::currentTextChanged, this, &MainWindow::SetSearchType);
    QObject::connect (ui->search_le, &QLineEdit::textChanged, this, &MainWindow::SearchTextChanged);
    QObject::connect (ui->patients_table, &QTableView::clicked, this, &MainWindow::ShowEventsBySelectedPatient);
    QObject::connect (ui->patients_table, &QTableView::clicked, this,  [=] { Update(ui->patients_table->currentIndex ().row ()); });
    QObject::connect (ui->calendar, &QCalendarWidget::clicked, this, &MainWindow::ShowEventsBySelectedDate );
    timer->start (60000);
    Update(0);
}

void MainWindow::resizeEvent(QResizeEvent *event) {
    if (!patient_photo.isNull ()){
        ui->patient_photo_lbl->setPixmap (patient_photo.scaledToWidth (ui->patient_photo_lbl->width ()));
    }
    QMainWindow::resizeEvent(event);
}

void MainWindow::BuildToolBar() {
    action_add_patient = toolbar->addAction(QPixmap(":/action_icons/add_patient.png"), "Створити картку пацієнта", this, SLOT(onActionAddPatient()));
    action_edit_patient = toolbar->addAction(QPixmap(":/action_icons/edit_patient.png"), "Редагувати картку пацієнта", this, SLOT(onActionEditPatient()));
//    action_tooth_card = toolbar->addAction(QPixmap(":/action_icons/tooth_card.png"), "Переглянути зубну картку пацієнта", this, SLOT(onActionToothCard()));
    action_visit_history = toolbar->addAction(QPixmap(":/action_icons/med_journal.png"), "Переглянути історію візитів пацієнта", this, SLOT(onActionVisitHistory()));
    toolbar->addSeparator ();
    action_add_event = toolbar->addAction(QPixmap(":/action_icons/add_event.png"), "Записати на прийом", this, SLOT(onActionAddEvent()));
    action_edit_event = toolbar->addAction(QPixmap(":/action_icons/edit_event.png"), "Змінити дані прийому", this, SLOT(onActionEditEvent()));
    action_cancel_event = toolbar->addAction(QPixmap(":/action_icons/cancel_event.png"), "Скасувати прийом", this, SLOT(onActionCancelEvent()));
    action_all_active_events = toolbar->addAction(QPixmap(":/action_icons/all_events.png"), "Переглянути всі записи на прийом", this, SLOT(onActionAllEvents()));
    action_bind_event = toolbar->addAction(QPixmap(":/action_icons/bind_event.png"), "Прив'язати прийом до картки клієнта", this, SLOT(onActionBindEvent()));
    toolbar->addSeparator ();
    action_appointment = toolbar->addAction(QPixmap(":/action_icons/appointment.png"), "Записати дані про прийом", this, SLOT(onActionAppointment()));
    toolbar->addSeparator ();
    action_report = toolbar->addAction(QPixmap(":/action_icons/report.png"), "Сформувати звіт", this, SLOT(onActionReport()));
    toolbar->addSeparator ();
    action_copy_restore = toolbar->addAction(QPixmap(":/action_icons/db_copy_restore.png"), "Резервна копія/відновлення бази даних", this, SLOT(onActionCopyRestoreDb()));

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
    patient_active_row = row;
    event_active_row = ui->events_table->currentIndex().row();

    events_model->select();
    patients_model->select ();
    patients_filter_model->sort (PATIENT_ID_COL, Qt::AscendingOrder);
    ui->patients_table->selectRow (patient_active_row);
    ui->events_table->selectRow(event_active_row);
    ShowPatientInfo();
    CancelEvents();
    UpdateButtons();
    GetActiveEventsDateList();
}

void MainWindow::UpdateButtons() {
    action_edit_patient->setEnabled (patients_model->rowCount ());
    action_visit_history->setEnabled (ui->last_visit_date_le->text ().size ());
    action_edit_event->setEnabled(events_filter_model->rowCount());
    action_cancel_event->setEnabled(events_filter_model->rowCount());
    action_all_active_events->setEnabled(events_model->rowCount());
    action_bind_event->setEnabled (events_filter_model->rowCount());
    action_appointment->setEnabled (events_filter_model->rowCount ());
}

void MainWindow::GetActiveEventsDateList() {
    QList<QDate> list;
    for (int r = 0; r < events_model->rowCount(); ++r) {
        list.append(events_model->data(events_model->index(r, EVENT_DATE_COL)).toDate());
    }
    ui->calendar->SetActiveDataList(list);
}

void MainWindow::CancelEvents() {
    for (int row = 0; row < events_model->rowCount (); ++row) {
        QString event_id = events_model->data(events_model->index (row, EVENT_ID_COL)).toString();
        QDateTime event_date_time  = QDateTime(events_model->data(events_model->index (row, EVENT_DATE_COL)).toDate (), events_model->data(events_model->index (row, EVENT_TIME_TO_COL)).toTime ());
        QString event_status = events_model->data(events_model->index (row, EVENT_STATUS_COL)).toString();
        if ( event_date_time < QDateTime::currentDateTime () && event_status == STATUS_LIST[ACTIVE]){
            QVariantList data = { QDateTime::currentDateTime ().toString(SQL_DATE_TIME_FORMAT), STATUS_LIST[CANCELED] };
            QStringList columns = { EVENT_LAST_CHANGES, EVENT_STATUS };
            sdb->UpdateInsertData (sdb->GenerateUpdateQuery (EVENTS_TABLE, columns, EVENT_ID, event_id),
                                   sdb->GenerateBindValues (columns),
                                   data);
        }
    }
}

void MainWindow::CreateReportCSV(const QVector<QVariantList> &table, const QString &path) {
    QFile report_csv(path + "/report " + QDateTime::currentDateTime ().toString (SQL_DATE_TIME_FORMAT).replace (":", "-") + ".csv");
        if(report_csv.open(QIODevice::WriteOnly)){
            QTextStream fout(&report_csv);

            #if defined(_WIN32)
                fout << "sep =,\n";
            #endif

            for (auto& row : table) {
                for(const QVariant& cell : row ) {
                    fout << "\"" + cell.toString () + "\",";
                }
                fout << '\n';
            }
            report_csv.close ();
            ui->statusBar->showMessage ("Звіт сформовано і збережено в папці " + path);
        }
}



void MainWindow::onActionAddPatient() {
    AddPatientDialog* add_patient = new AddPatientDialog (sdb, QVariantList(), this);
    if(add_patient->exec () == QDialog::Accepted) {
        QByteArray pic_byte_arr;
        if(add_patient->GetPhotoPath ().size ()){
            QPixmap pic(add_patient->GetPhotoPath ());
            QBuffer buff(&pic_byte_arr);
            buff.open (QIODevice::WriteOnly);
            pic.save (&buff, "JPG");
        }

        QVariantList data = { QDateTime::currentDateTime ().toString (SQL_DATE_TIME_FORMAT),
                              QDateTime::currentDateTime ().toString (SQL_DATE_TIME_FORMAT),
                              add_patient->GetSurname (),
                              add_patient->GetName (),
                              add_patient->GetFName (),
                              add_patient->GetBDate (),
                              add_patient->GetSex(),
                              add_patient->GetCity(),
                              add_patient->GetTelNumber (),
                              add_patient->GetIllnesses(),
                              pic_byte_arr };

        QStringList columns = { PATIENT_INIT_DATE, PATIENT_LAST_CHANGES, SURNAME, NAME, F_NAME, B_DATE, SEX, CITY, TEL_NUMBER, ILLNESSES, PATIENT_PHOTO };
        if (!sdb->UpdateInsertData (sdb->GenerateInsertQuery (PATIENTS_TABLE, columns),
                                       sdb->GenerateBindValues (columns),
                                       data)) {
            QMessageBox::critical (this, "Error!", "Невдалось створити картку пацієнта! Проблема з підключеням до бази даних");
            ui->statusBar->showMessage ("Невдалось створити картку пацієнта! Проблема з підключеням до бази даних");
            return;
        }
        Update(patients_filter_model->rowCount ());
        ui->statusBar->showMessage ("Додано пацієнта " + add_patient->GetSurname () + " " + add_patient->GetName ().left (1) + "." + add_patient->GetFName().left(1) + ".");
    }
}

void MainWindow::onActionEditPatient() {
    QVariantList row = sdb->SelectRow ("*", PATIENTS_TABLE, PATIENT_ID, patients_filter_model->data(patients_filter_model->index (ui->patients_table->currentIndex ().row (), PATIENT_ID_COL)).toString (), patients_model->columnCount());
    AddPatientDialog* edit_patient = new AddPatientDialog (sdb, row, this);
    if(edit_patient->exec() == QDialog::Accepted){
        QVariantList patient_data = { QDateTime::currentDateTime ().toString (SQL_DATE_TIME_FORMAT),
                              edit_patient->GetSurname (),
                              edit_patient->GetName (),
                              edit_patient->GetFName (),
                              edit_patient->GetBDate (),
                              edit_patient->GetSex(),
                              edit_patient->GetCity(),
                              edit_patient->GetTelNumber (),
                              edit_patient->GetIllnesses() };
        QStringList patient_columns = { PATIENT_LAST_CHANGES, SURNAME, NAME, F_NAME, B_DATE, SEX, CITY, TEL_NUMBER, ILLNESSES };

        if (edit_patient->GetPhotoPath ().isEmpty () && edit_patient->isPicDeleted ()){
            patient_data.append (QByteArray());
            patient_columns.append (PATIENT_PHOTO);
        }
        else if (!edit_patient->GetPhotoPath ().isEmpty ()) {
            QPixmap pic(edit_patient->GetPhotoPath ());
            QByteArray pic_byte_arr;
            QBuffer buff(&pic_byte_arr);
            buff.open (QIODevice::WriteOnly);
            pic.save (&buff, "JPG");

            patient_data.append ( pic_byte_arr);
            patient_columns.append (PATIENT_PHOTO);
        }

        if (!sdb->UpdateInsertData (sdb->GenerateUpdateQuery (PATIENTS_TABLE, patient_columns, PATIENT_ID, row.at(PATIENT_ID_COL).toString ()),
                                    sdb->GenerateBindValues (patient_columns),
                                    patient_data)) {
            QMessageBox::critical (this, "Error!", "Невдалось відредагувати картку пацієнта! Проблема з підключеням до бази даних");
            ui->statusBar->showMessage ("Невдалось відредагувати картку пацієнта! Проблема з підключеням до бази даних");
            return;
        }
        QVariantList event_data = { QDateTime::currentDateTime ().toString (SQL_DATE_TIME_FORMAT),
                                    edit_patient->GetSurname () + " " + edit_patient->GetName () + " " + edit_patient->GetFName () };
        QStringList event_columns = { EVENT_LAST_CHANGES, PATIENT };


        if(!sdb->UpdateInsertData(sdb->GenerateUpdateQuery (EVENTS_TABLE, event_columns, PATIENT_ID, row.at(PATIENT_ID_COL).toString () ),
                                  sdb->GenerateBindValues (event_columns),
                                  event_data)){
            return;
        }
        Update(ui->patients_table->currentIndex ().row ());
        ui->statusBar->showMessage ("Відредаговано картку пацієнта " + edit_patient->GetSurname () + " " + edit_patient->GetName ().left (1) + "." + edit_patient->GetFName().left(1) + ".");
    }
}

void MainWindow::onActionToothCard() {

}

void MainWindow::onActionVisitHistory() {
    QVariantList row = patients_model->rowCount () ? sdb->SelectRow ("*", PATIENTS_TABLE, PATIENT_ID, patients_filter_model->data(patients_filter_model->index (ui->patients_table->currentIndex ().row (), PATIENT_ID_COL)).toString (), patients_model->columnCount()) : QVariantList();
    PatientVisitsDialog* patient_visits = new PatientVisitsDialog(sdb, row, this);
    patient_visits->exec ();
}

void MainWindow::onActionAddEvent() {
    QVariantList row = patients_model->rowCount () ? sdb->SelectRow ("*", PATIENTS_TABLE, PATIENT_ID, patients_filter_model->data(patients_filter_model->index (ui->patients_table->currentIndex ().row (), PATIENT_ID_COL)).toString (), patients_model->columnCount()) : QVariantList();
    AddEventDialog* add_event = new AddEventDialog(sdb, &row, ADD, this);
    if(add_event->exec () == QDialog::Accepted){
        QVariantList data = { QDateTime::currentDateTime ().toString (SQL_DATE_TIME_FORMAT),
                              QDateTime::currentDateTime ().toString (SQL_DATE_TIME_FORMAT),
                              add_event->GetDate (),
                              add_event->GetTimeFrom (),
                              add_event->GetTimeTo (),
                              add_event->GetPatient (),
                              STATUS_LIST[ACTIVE],
                              add_event->GetComment (),
                              add_event->GetPatientId ()
                            };
        QStringList columns = { EVENT_INIT_DATE, EVENT_LAST_CHANGES, EVENT_DATE, EVENT_TIME_FROM, EVENT_TIME_TO, PATIENT, EVENT_STATUS, COMMENT, PATIENT_ID };

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

void MainWindow::onActionEditEvent() {
    QString event_id = events_filter_model->data(events_filter_model->index(ui->events_table->currentIndex().row(), EVENT_ID_COL)).toString();
    QString event_status = events_filter_model->data(events_filter_model->index(ui->events_table->currentIndex().row(), EVENT_STATUS_COL)).toString();
    if(event_id.isEmpty()) {
        QMessageBox::information(this, "Редагування прийому", "Виберіть прийом, який хочете відредагувати");
        return;
    }
    if(event_status != STATUS_LIST[ACTIVE]) {
        QMessageBox::information(this, "Редагування прийому", "Неможливо відредагувати прийом зі статусом Скасовано або Виконано!");
        return;
    }

    QVariantList row = events_filter_model->rowCount() ? sdb->SelectRow("*", EVENTS_TABLE, EVENT_ID, event_id, events_model->columnCount()) : QVariantList();
    AddEventDialog* edit_event = new AddEventDialog(sdb, &row, EDIT, this);
    if(edit_event->exec() == QDialog::Accepted){
        QVariantList data = { QDateTime::currentDateTime ().toString (SQL_DATE_TIME_FORMAT),
                              edit_event->GetDate(),
                              edit_event->GetTimeFrom(),
                              edit_event->GetTimeTo(),
                              edit_event->GetComment()
                            };
        QStringList columns = { EVENT_LAST_CHANGES, EVENT_DATE, EVENT_TIME_FROM, EVENT_TIME_TO, COMMENT};

        if (!sdb->UpdateInsertData (sdb->GenerateUpdateQuery (EVENTS_TABLE, columns, EVENT_ID, row.at(EVENT_ID_COL).toString ()),
                                    sdb->GenerateBindValues (columns),
                                    data)) {
            QMessageBox::critical (this, "Error!", "Невдалось відредагувати дані прийому! Проблема з підключеням до бази даних");
            ui->statusBar->showMessage ("Невдалось відредагувати дані прийому! Проблема з підключеням до бази даних");
            return;
        }
        Update(ui->patients_table->currentIndex ().row ());
        ui->statusBar->showMessage ("Відредаговано дані прийому пацієнта " + row.at(PATIENT_COL).toString());
    }
}

void MainWindow::onActionCancelEvent() {
    QString event_id = events_filter_model->data (events_filter_model->index (ui->events_table->currentIndex().row(), EVENT_ID_COL)).toString ();
    QString patient = events_filter_model->data (events_filter_model->index (ui->events_table->currentIndex().row(), PATIENT_COL)).toString ();
    QString status = events_filter_model->data (events_filter_model->index (ui->events_table->currentIndex().row(), EVENT_STATUS_COL)).toString ();
    if(event_id.isEmpty() || status == STATUS_LIST[CANCELED] || status == STATUS_LIST[EXECUTED]) {
        QMessageBox::information(this, "Скасування прийому", "Скасувати можна лише прийом зі статусом Активний!");
        return;
    }

    QMessageBox msgbox(QMessageBox::Question,
                       "Скасування прийому",
                       "Ви дійсно бажаєте скасувати прийом " + patient,
                       QMessageBox::Yes | QMessageBox::No,
                       this);
    msgbox.setButtonText (QMessageBox::Yes, tr("Так"));
    msgbox.setButtonText (QMessageBox::No, tr("Ніт"));

    if(msgbox.exec () == QMessageBox::Yes){
        QVariantList data = { QDateTime::currentDateTime ().toString (SQL_DATE_TIME_FORMAT),
                              STATUS_LIST[CANCELED]
                            };
        QStringList columns = { EVENT_LAST_CHANGES, EVENT_STATUS };

        if (!sdb->UpdateInsertData (sdb->GenerateUpdateQuery (EVENTS_TABLE, columns, EVENT_ID, event_id),
                                    sdb->GenerateBindValues (columns),
                                    data)) {
            QMessageBox::critical (this, "Error!", "Невдалось скасувати прийом! Проблема з підключеням до бази даних");
            ui->statusBar->showMessage ("Невдалось скасувати прийом! Проблема з підключеням до бази даних");
            return;
        }
        Update(ui->patients_table->currentIndex ().row ());
        ui->statusBar->showMessage ("Прийом пацієнта " + patient + " скасовано!");
    }
}

void MainWindow::onActionAllEvents() {
    AllEventsDialog* all_events = new AllEventsDialog(sdb, this);
    all_events->exec ();
}

void MainWindow::onActionBindEvent() {
    QString event_id = events_filter_model->data(events_filter_model->index(ui->events_table->currentIndex().row(), EVENT_ID_COL)).toString();
    QString patient_id = events_filter_model->data(events_filter_model->index(ui->events_table->currentIndex().row(), EVENT_PATIENT_ID_COL)).toString();

    if(event_id.isEmpty()) {
        QMessageBox::information(this, "Прив'язати прийом до картки клієнта", "Виберіть з переліку прийом, який бажаєте прив'язати!");
        return;
    }
    if(!patient_id.isEmpty()) {
        QMessageBox::information(this, "Прив'язати прийом до картки клієнта", "Вибераний пацієнт вже має заповнену картку!");
        return;
    }
    BindEventDialog* bind_event = new BindEventDialog(sdb, this);
    if(bind_event->exec () == QDialog::Accepted){
        QVariantList data = { QDateTime::currentDateTime ().toString (SQL_DATE_TIME_FORMAT), bind_event->GetPatient (), bind_event->GetPatientId () };
        QStringList columns = { EVENT_LAST_CHANGES, PATIENT, PATIENT_ID };
        if (!sdb->UpdateInsertData (sdb->GenerateUpdateQuery (EVENTS_TABLE, columns, EVENT_ID, event_id),
                                    sdb->GenerateBindValues (columns),
                                    data)) {
            QMessageBox::critical (this, "Error!", "Невдалось прив'язати прийом до картки клієнта! Проблема з підключеням до бази даних");
            ui->statusBar->showMessage ("Невдалось прив'язати прийом до картки клієнта! Проблема з підключеням до бази даних");
            return;
        }
        Update(ui->patients_table->currentIndex ().row ());
        ui->statusBar->showMessage ("Прийом прив'язано до пацієнта " + bind_event->GetPatient ());
    }
}


void MainWindow::onActionAppointment() {
    QString event_id = events_filter_model->data(events_filter_model->index(ui->events_table->currentIndex().row(), EVENT_ID_COL)).toString();
    QString patient_id = events_filter_model->data(events_filter_model->index(ui->events_table->currentIndex().row(), EVENT_PATIENT_ID_COL)).toString();
    QString event_status = events_filter_model->data(events_filter_model->index(ui->events_table->currentIndex().row(), EVENT_STATUS_COL)).toString();

    if(event_id.isEmpty()) {
        QMessageBox::information(this, "Записати дані про прийом", "Виберіть з переліку прийом!");
        return;
    }
    if (event_status == STATUS_LIST[EXECUTED]){
        QMessageBox::information(this, "Записати дані про прийом", "Цей прийом вже відбувся!");
        return;
    }
    if(patient_id.isEmpty ()){
        QMessageBox::information(this, "Записати дані про прийом", "У вибраного вами пацієнта відсутня картка!\nСтворіть картку пацієнта та прив'яжіть до неї вибраний прийом!");
        return;
    }

    QVariantList row = events_filter_model->rowCount() ? sdb->SelectRow("*", EVENTS_TABLE, EVENT_ID, event_id, events_model->columnCount()) : QVariantList();
    AppointmentDialog* appointment = new AppointmentDialog(sdb, &row, this);
    if(appointment->exec() == QDialog::Accepted){
        QVariantList data_events = { QDateTime::currentDateTime ().toString (SQL_DATE_TIME_FORMAT), STATUS_LIST[EXECUTED] };
        QStringList columns_events = { EVENT_LAST_CHANGES, EVENT_STATUS};

        QVariantList data_visits = { QDateTime::currentDateTime ().toString (SQL_DATE_TIME_FORMAT),
                                     QDateTime::currentDateTime ().toString (SQL_DATE_TIME_FORMAT),
                                     row.at (PATIENT_COL).toString (),
                                     appointment->GetVisitDate (),
                                     appointment->GetPrice (),
                                     appointment->GetResults (),
                                     event_id
                                   };
        QStringList columns_visits = { VISIT_INIT_DATE, VISIT_LAST_CHANGES, PATIENT, VISIT_DATE, PRICE, VISIT_RESULT, EVENT_ID};


        if (!sdb->UpdateInsertData (sdb->GenerateUpdateQuery (EVENTS_TABLE, columns_events, EVENT_ID, event_id),
                                    sdb->GenerateBindValues (columns_events),
                                    data_events) ||
            !sdb->UpdateInsertData (sdb->GenerateInsertQuery (VISITS_TABLE, columns_visits),
                                    sdb->GenerateBindValues (columns_visits),
                                    data_visits))
        {
            QMessageBox::critical (this, "Error!", "Невдалось записати дані про візит! Проблема з підключеням до бази даних");
            ui->statusBar->showMessage ("Невдалось записати дані про візит! Проблема з підключеням до бази даних");
            return;
        }
        Update(ui->patients_table->currentIndex ().row ());
        ui->statusBar->showMessage ("Дані про візит пацієнта " + row.at (PATIENT_COL).toString () + " збережено!");
    }
}

void MainWindow::onActionReport() {
    ReportDialog* report_dialog = new ReportDialog(sdb, this);
    if(report_dialog->exec () == QDialog::Accepted) {
        QString path = QFileDialog::getExistingDirectory(this, tr("Зберегти звіт в ..."),
                                                        QDir::homePath (),
                                                        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
        if(!path.isEmpty ()) {
            QVector<QVariantList> table;
            switch (report_dialog->GetReportType ()) {
            case VISITS_REPORT:
                table = sdb->SelectTable (SqlQueries::VisitsForReport(report_dialog->GetDateFrom (), report_dialog->GetDateTo ()));
                for (int row = 0; row < table.at(0).size (); ++row) {
                    table[0][row] = QVariant(VISITS_TABLE_HEADERS.at (row + VISIT_DATE_COL));
                }
                break;
            }
            CreateReportCSV (table, path);
        }
    }
}

void MainWindow::onActionReserveCopy() {
    QString path = QFileDialog::getExistingDirectory(this, tr("Зберегти базу даних в ..."),
                                                    QDir::homePath (),
                                                    QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if(QFile::copy (DB_PATH, path + "/ndc_db " + QDateTime::currentDateTime ().toString (FS_DATE_TIME_FORMAT) + ".sqlite3")){
        ui->statusBar->showMessage ("Резервну копію бази даних збережено в папці " + path );
    }
    else {
        ui->statusBar->showMessage ("Не вдалось зробити копію бази даних!");
    }
}

void MainWindow::onActionRestore() {
    QMessageBox msgbox;
    msgbox.setIcon (QMessageBox::Warning);
    msgbox.setWindowTitle ("Відновлення бази даних");
    msgbox.setText ("УВАГА!!! Після здійснення відновлення бази даних, поточні дані видаляться!!!\n\nРекомендуємо перед відновленням зберегти резервну копію поточної бази даних.");
    msgbox.addButton ("Відновити без збереження", QMessageBox::AcceptRole);
    msgbox.addButton ("Скасувати", QMessageBox::RejectRole);

    if(msgbox.exec () == QMessageBox::AcceptRole) {
        QString path = QFileDialog::getOpenFileName (this, "Виберіть файл бази даних", QDir::homePath (), "*.sqlite3");
        if (!path.isEmpty ()){
            sdb->CloseDataBase ();
            if (QFile::remove (DB_PATH)) {
                if(QFile::copy (path, DB_PATH)){
                    //sdb = new DataBase();
                    if (!sdb->ConnectToDataBase (DB_PATH)) {
                        QMessageBox::critical (this, "Error!", "Неможливо з'єднатись з базою даних!", QMessageBox::Ok);
                    }
                    Update(0);
                    ui->statusBar->showMessage ("Базу даних відновлено!");
                    QMessageBox::information (this, "NDC", "Для коректної роботи після відновлення бази даних, рекомендується перезапутити програму", QMessageBox::Ok);
                }
                else {
                    QMessageBox::critical (this, "Error!", "Не вдалось відновити базу даних!", QMessageBox::Ok);
                }
            }
            else {
                QMessageBox::critical (this, "Error!", "Не вдалось відновити базу даних!", QMessageBox::Ok);
            }
        }
    }
}

void MainWindow::onActionCopyRestoreDb() {

}

void MainWindow::ShowEventsBySelectedDate() {
    events_filter_model->setFilterKeyColumn(EVENT_DATE_COL);
    events_filter_model->setFilterFixedString(ui->calendar->selectedDate().toString(SQL_DATE_FORMAT));
    events_filter_model->sort (EVENT_TIME_FROM_COL);
    UpdateButtons();
}

void MainWindow::ShowEventsBySelectedPatient() {
    QString patient_id = patients_filter_model->data(patients_filter_model->index (ui->patients_table->currentIndex ().row (), PATIENT_ID_COL)).toString ();
    events_filter_model->setFilterKeyColumn(EVENT_PATIENT_ID_COL);
    events_filter_model->setFilterRegExp(QString("^%1$").arg(patient_id));
    events_filter_model->sort (EVENT_TIME_FROM_COL);
    events_filter_model->sort (EVENT_DATE_COL);
}

void MainWindow::ShowPatientInfo() {
    patient_photo.loadFromData (patients_filter_model->data(patients_filter_model->index (ui->patients_table->currentIndex ().row (), PATIENT_PHOTO_COL)).toByteArray ());

    if (!patient_photo.isNull ()) {
        ui->patient_photo_lbl->setPixmap (patient_photo.scaledToWidth (ui->patient_photo_lbl->width ()));
    }
    else if(patients_filter_model->data(patients_filter_model->index (ui->patients_table->currentIndex ().row (), SEX_COL)).toString () == "Чоловіча") {
        ui->patient_photo_lbl->setPixmap (QPixmap(":/action_icons/default_user.png"));
    }
    else {
        ui->patient_photo_lbl->setPixmap (QPixmap(":/action_icons/default_user_female.png"));
    }

    QDate b_date = QDate::fromString (patients_filter_model->data(patients_filter_model->index (ui->patients_table->currentIndex ().row (), B_DATE_COL)).toString (), SQL_DATE_FORMAT);
    int age = QDate::currentDate ().addYears ( - b_date.year ()).addMonths ( - b_date.month () + 1).addDays ( - b_date.day () + 1).year ();
    QString patient_id = patients_filter_model->data(patients_filter_model->index (ui->patients_table->currentIndex ().row (), PATIENT_ID_COL)).toString ();

    ui->surname_le->        setText(patients_filter_model->data(patients_filter_model->index (ui->patients_table->currentIndex ().row (), SURNAME_COL)).toString ());
    ui->name_le->           setText(patients_filter_model->data(patients_filter_model->index (ui->patients_table->currentIndex ().row (), NAME_COL)).toString ());
    ui->f_name_le->         setText(patients_filter_model->data(patients_filter_model->index (ui->patients_table->currentIndex ().row (), F_NAME_COL)).toString ());
    ui->b_date_le->         setText(b_date.toString (DATE_FORMAT) + " (" + QString::number (age) + "р.)");
    ui->sex_le->            setText(patients_filter_model->data(patients_filter_model->index (ui->patients_table->currentIndex ().row (), SEX_COL)).toString ());
    ui->city_le->           setText(patients_filter_model->data(patients_filter_model->index (ui->patients_table->currentIndex ().row (), CITY_COL)).toString ());
    ui->tel_number_le->     setText(patients_filter_model->data(patients_filter_model->index (ui->patients_table->currentIndex ().row (), TEL_NUMBER_COL)).toString ());
    ui->last_visit_date_le->setText (QDate::fromString (sdb->Select (SqlQueries::MaxVisitDateQuery (patient_id)), SQL_DATE_FORMAT).toString (DATE_FORMAT));
    ui->illnesses_le->      setText(patients_filter_model->data(patients_filter_model->index (ui->patients_table->currentIndex ().row (), ILLNESSES_COL)).toString ());
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
