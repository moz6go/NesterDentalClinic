#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "general.h"

#include "addpatientdialog.h"
#include "addeventdialog.h"
#include "mycalendarwidget.h"
#include "alleventsdialog.h"
#include "eventsproxymodel.h"
#include "appointmentdialog.h"
#include "bindeventdialog.h"
#include "sqlqueries.h"
#include "patientvisitsdialog.h"

class AllEventsDialog;
class BindEventDialog;
class PatientVisitsDialog;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
    friend AllEventsDialog;
    friend BindEventDialog;
    friend PatientVisitsDialog;

    int patient_active_row;
    int event_active_row;

    Ui::MainWindow *ui;
    DataBase* sdb;
    QSqlTableModel* patients_model;
    QSqlTableModel* events_model;
    QSortFilterProxyModel* patients_filter_model;
    EventsProxyModel* events_filter_model;
    QToolBar* toolbar;
    QPixmap patient_photo;

    QAction* action_add_patient;
    QAction* action_edit_patient;
    QAction* action_tooth_card;
    QAction* action_visit_history;

    QAction* action_add_event;
    QAction* action_edit_event;
    QAction* action_cancel_event;
    QAction* action_all_active_events;
    QAction* action_bind_event;

    QAction* action_appointment;

    void resizeEvent(QResizeEvent *event);
    void BuildToolBar();
    static void RenameHeaders(int column_count, QSqlTableModel* model, const QStringList& headers_list);
    static void TableInit(QTableView* table);
    void Update(int row);
    void UpdateButtons();
    void GetActiveEventsDateList();
    void CancelEvents();
private slots:
    void onActionAddPatient();
    void onActionEditPatient();
    void onActionToothCard();
    void onActionVisitHistory();
    void onActionAddEvent();
    void onActionEditEvent();
    void onActionCancelEvent();
    void onActionAllEvents();
    void onActionBindEvent();
    void onActionAppointment();
    void ShowPatientInfo();
    void SetSearchType(QString type);
    void SearchTextChanged(QString text);
    void ShowEventsBySelectedDate();
    void ShowEventsBySelectedPatient();
public:
    explicit MainWindow(DataBase* data_base, QWidget *parent = nullptr);
    ~MainWindow();
};

#endif // MAINWINDOW_H
