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
    QSqlTableModel* patients_model;
    QSqlTableModel* events_model;
    QSortFilterProxyModel* patients_filter_model;
    QSortFilterProxyModel* events_filter_model;
    QToolBar* toolbar;
    QPixmap patient_photo;

    QAction* action_add_patient;
    QAction* action_add_event;
    void resizeEvent(QResizeEvent *event);
    void BuildToolBar();
    void RenameHeaders(int column_count, QSqlTableModel* model, const QStringList& headers_list);
    void TableInit(QTableView* table);
    void Update(int row);
private slots:
    void onActionAddPatient();
    void onActionAddEvent();
    void ShowPatientInfo();
    void SetSearchType(QString type);
    void SearchTextChanged(QString text);
    void onEditClientClicked();
    void DateSelected();
public:
    explicit MainWindow(DataBase* data_base, QWidget *parent = nullptr);
    ~MainWindow();
};

#endif // MAINWINDOW_H
