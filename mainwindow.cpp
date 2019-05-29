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
    toolbar->addSeparator ();

    toolbar->setMovable (false);
    toolbar->setIconSize (QSize(SIZE_WID_1, SIZE_WID_1));
    addToolBar(Qt::TopToolBarArea, toolbar);
}

void MainWindow::onActionAddPatient() {
    qDebug() << 123;
}

MainWindow::~MainWindow() {
    delete ui;
}
