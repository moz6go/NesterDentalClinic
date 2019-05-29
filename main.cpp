#include "mainwindow.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    //qDebug() << QStyleFactory::keys ();
    QApplication::setStyle(QStyleFactory::create("Fusion"));

#if defined _WIN32 && !defined QT_DEBUG
    if (!QDir(QStandardPaths::writableLocation(QStandardPaths::DataLocation)).exists ()){
        QDir().mkdir(QStandardPaths::writableLocation(QStandardPaths::DataLocation));
    }
#endif

    DataBase* sdb = new DataBase();
    if (!sdb->ConnectToDataBase (DB_PATH)) {
        QMessageBox::critical (nullptr, "Error", "Неможливо з'єднатись з базою даних!", QMessageBox::Ok);
        return -1;
    }
    MainWindow w;
    w.show();

    return a.exec();
}
