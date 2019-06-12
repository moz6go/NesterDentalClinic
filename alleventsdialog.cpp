#include "alleventsdialog.h"
#include "ui_alleventsdialog.h"
#include "mainwindow.h"


AllEventsDialog::AllEventsDialog(DataBase *data_base, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AllEventsDialog)
{
    ui->setupUi(this);
    sdb = data_base;
    ui->search_cb->addItems (QStringList() << EVENTS_TABLE_HEADERS[PATIENT_COL]
                             << EVENTS_TABLE_HEADERS[EVENT_DATE_COL]
                             << EVENTS_TABLE_HEADERS[EVENT_TIME_FROM_COL]
                             << EVENTS_TABLE_HEADERS[EVENT_TIME_TO_COL]
                             << EVENTS_TABLE_HEADERS[EVENT_STATUS_COL]
                             << EVENTS_TABLE_HEADERS[COMMENT_COL]);

    ui->search_de->setDate(QDate::currentDate());
    ui->status_cb->addItems(STATUS_LIST);

    events_model = new QSqlTableModel(this);
    events_model->setTable(EVENTS_TABLE);
    MainWindow::RenameHeaders(events_model->columnCount(), events_model, EVENTS_TABLE_HEADERS);
    events_filter_model = new MySortFilterProxyModel(this);
    events_filter_model->setSourceModel(events_model);
    events_filter_model->setFilterKeyColumn(PATIENT_COL);
    events_filter_model->setFilterCaseSensitivity(Qt::CaseInsensitive);
    ui->events_table->setModel(events_filter_model);

    for (int col = 0; col < events_model->columnCount(); ++col) {
        if (col == EVENT_ID_COL || col == INIT_DATE_COL || col == EVENT_PATIENT_ID_COL){
            ui->events_table->setColumnHidden(col, true);
        }
    }

    MainWindow::TableInit (ui->events_table);
    ui->events_table->setColumnWidth (EVENT_DATE_COL,70);
    ui->events_table->setColumnWidth(EVENT_TIME_FROM_COL, 40);
    ui->events_table->setColumnWidth(EVENT_TIME_TO_COL, 40);
    ui->events_table->setColumnWidth(PATIENT_COL, 250);
    events_filter_model->sort (EVENT_TIME_FROM_COL);
    events_filter_model->sort (EVENT_DATE_COL);
    events_model->select();

    QObject::connect(ui->search_cb, &QComboBox::currentTextChanged, this, &AllEventsDialog::SetSearchType);
    QObject::connect(ui->search_de, &QDateEdit::dateChanged, this, &AllEventsDialog::SearchDateChanged);
    QObject::connect(ui->search_le, &QLineEdit::textChanged, this, &AllEventsDialog::SearchTextChanged);
    QObject::connect(ui->search_te, &QTimeEdit::timeChanged, this, &AllEventsDialog::SearchTimeChanged);
    QObject::connect(ui->status_cb, &QComboBox::currentTextChanged, this, &AllEventsDialog::SearchStatusChanged);
}

void AllEventsDialog::SetSearchType(QString type) {
    if(type == EVENTS_TABLE_HEADERS[PATIENT_COL]) {
        ui->stacked_wgt->setCurrentWidget(ui->s_text);
        events_filter_model->setFilterKeyColumn (PATIENT_COL);
        SearchTextChanged(ui->search_le->text());
    }
    else if (type == EVENTS_TABLE_HEADERS[EVENT_DATE_COL]) {
        ui->stacked_wgt->setCurrentWidget(ui->s_date);
        events_filter_model->setFilterKeyColumn (EVENT_DATE_COL);
        SearchDateChanged(ui->search_de->date());
    }
    else if (type == EVENTS_TABLE_HEADERS[EVENT_TIME_FROM_COL]) {
        ui->stacked_wgt->setCurrentWidget(ui->s_time);
        events_filter_model->setFilterKeyColumn (EVENT_TIME_FROM_COL);
        SearchTimeChanged(ui->search_te->time());
    }
    else if (type == EVENTS_TABLE_HEADERS[EVENT_TIME_TO_COL]) {
        ui->stacked_wgt->setCurrentWidget(ui->s_time);
        events_filter_model->setFilterKeyColumn (EVENT_TIME_TO_COL);
        SearchTimeChanged(ui->search_te->time());
    }
    else if (type == EVENTS_TABLE_HEADERS[EVENT_STATUS_COL]) {
        ui->stacked_wgt->setCurrentWidget(ui->s_combo);
        events_filter_model->setFilterKeyColumn (EVENT_STATUS_COL);
        SearchTextChanged(ui->status_cb->currentText());
    }
    else if (type == EVENTS_TABLE_HEADERS[COMMENT_COL]) {
        ui->stacked_wgt->setCurrentWidget(ui->s_text);
        events_filter_model->setFilterKeyColumn (COMMENT_COL);
        SearchTextChanged(ui->search_le->text());
    }
}

void AllEventsDialog::SearchTextChanged(QString text) {
    events_filter_model->setFilterFixedString (text);
}

void AllEventsDialog::SearchDateChanged(QDate date) {
    events_filter_model->setFilterFixedString (date.toString(SQL_DATE_FORMAT));
}

void AllEventsDialog::SearchTimeChanged(QTime time) {
    events_filter_model->setFilterFixedString (time.toString(TIME_FORMAT));
}

void AllEventsDialog::SearchStatusChanged(QString text){
    events_filter_model->setFilterFixedString (text);
}

AllEventsDialog::~AllEventsDialog() {
    delete ui;
}
