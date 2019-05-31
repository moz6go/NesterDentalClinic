#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "general.h"
#include "database.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
    Ui::MainWindow *ui;
    DataBase* sdb;
    QSqlTableModel* sql_model;
    QSortFilterProxyModel* filter_model;
    QToolBar* toolbar;
    QPixmap patient_photo;

    QAction* action_add_patient;
    QAction* action_add_event;
    void resizeEvent(QResizeEvent *event);
    void BuildToolBar();
    void PatientTableInit();
    void Update(int row);
private slots:
    void onActionAddPatient();
    void onActionAddEvent();
    void ShowPatientInfo();
    void SetSearchType(QString type);
    void SearchTextChanged(QString text);
public:
    explicit MainWindow(DataBase* data_base, QWidget *parent = nullptr);
    ~MainWindow();
};

#endif // MAINWINDOW_H
