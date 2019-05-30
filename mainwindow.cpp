#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    toolbar = new QToolBar(this);
    BuildToolBar ();
}

void MainWindow::BuildToolBar() {
    action_add_patient = toolbar->addAction(QPixmap(":/action_icons/add_patient.png"), "Додати пацієнта", this, SLOT(onActionAddPatient()));
    action_add_event = toolbar->addAction(QPixmap(":/action_icons/add_event.png"), "Записати на прийом", this, SLOT(onActionAddEvent()));
    toolbar->addSeparator ();

    toolbar->setMovable (false);
    toolbar->setIconSize (QSize(SIZE_WID_1, SIZE_WID_1));
    addToolBar(Qt::TopToolBarArea, toolbar);
}

void MainWindow::onActionAddPatient() {
    qDebug() << "patient";
}

void MainWindow::onActionAddEvent() {
    qDebug() << "event";
}

MainWindow::~MainWindow() {
    delete ui;
}
