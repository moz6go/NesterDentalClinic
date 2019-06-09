#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "general.h"
#include "database.h"
#include "addpatientdialog.h"
#include "addeventdialog.h"
#include "mycalendarwidget.h"

namespace Ui {
class MainWindow;
}

class MySortFilterProxyModel : public QSortFilterProxyModel
{
public:
    MySortFilterProxyModel(QObject* parent = nullptr);
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
};

class MainWindow : public QMainWindow {
    Q_OBJECT
    Ui::MainWindow *ui;
    DataBase* sdb;
    QSqlTableModel* patients_model;
    QSqlTableModel* events_model;
    QSortFilterProxyModel* patients_filter_model;
    MySortFilterProxyModel* events_filter_model;
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

    QAction* action_appointment;

    void resizeEvent(QResizeEvent *event);
    void BuildToolBar();
    void RenameHeaders(int column_count, QSqlTableModel* model, const QStringList& headers_list);
    void TableInit(QTableView* table);
    void Update(int row);
    void UpdateButtons();
    void GetActiveEventsDateList();
    void CancelEvents();
private slots:
    void onActionAddPatient();
    void onActionEditClient();
    void onActionToothCard();
    void onActionVisitHistory();
    void onActionAddEvent();
    void onActionEditEvent();
    void onActionCancelEvent();
    void onActionAllEvents();
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
