#ifndef GENERAL
#define GENERAL

#include <QApplication>
#include <QMainWindow>
#include <QtDebug>
#include <QDebug>
#include <QObject>
#include <QToolBar>
#include <QLabel>
#include <QLineEdit>
#include <QHeaderView>
#include <QTableView>
#include <QTableWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpinBox>
#include <QTextEdit>
#include <QLineEdit>
#include <QComboBox>
#include <QFileDialog>
#include <QDate>
#include <QBuffer>
#include <QMessageBox>
#include <QStyleFactory>
#include <QStandardPaths>

#include <QtSql/QSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlTableModel>
#include <QSortFilterProxyModel>
#include <QSqlQueryModel>
#include <QSqlError>
#include <QSqlField>

const int SIZE_WID_1 = 40;
const int SIZE_WID_2 = 24;

#if defined(_WIN32)
    #if defined(QT_DEBUG)
        const QString DB_PATH = QDir::currentPath () + "/ndc_db.sqlite3";
    #else
        const QString DB_PATH = QStandardPaths::writableLocation(QStandardPaths::DataLocation) + "/NesterDentalClinic/ndc_db.sqlite3";
    #endif
#else
    const QString DB_PATH = QDir::currentPath () + "/ndc_db.sqlite3";
#endif

#endif // GENERAL

