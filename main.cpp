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
    QStringList create_tables_queries = {
        SqlQueries::CreatePatientsTable (),
        SqlQueries::CreateEventsTable (),
        SqlQueries::CreateVisitsTable (),
    };

    DataBase* sdb = new DataBase(create_tables_queries);
    if (!sdb->ConnectToDataBase (DB_PATH)) {
        QMessageBox::critical (nullptr, "Error", "Неможливо з'єднатись з базою даних!\n\nПомилка:\n" + sdb->LastError (), QMessageBox::Ok);
        return -1;
    }
    MainWindow w(sdb);
    w.show();

    return a.exec();
}
