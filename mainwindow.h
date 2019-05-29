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
    QToolBar* toolbar;

    QAction* action_add_patient;

    void BuildToolBar();
private slots:
    void onActionAddPatient();

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
};

#endif // MAINWINDOW_H
