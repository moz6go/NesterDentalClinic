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

const int SIZE_WID_1 = 36;
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

// tables
const QString PATIENTS_TABLE = "patients";
const QString VISITS_TABLE = "visits";
const QString SERVICES_TABLE = "services";
const QString DISEASES_TABLE = "diseases";

// columns
const QString PATIENT_ID = "patient_id";
const QString INIT_DATE = "init_date";
const QString SURNAME = "surname";
const QString NAME = "name";
const QString F_NAME = "f_name";
const QString B_DATE = "b_date";
const QString SEX = "sex";
const QString CITY = "city";
const QString TEL_NUMBER = "tel_number";
const QString PATIENT_PHOTO = "patient_photo";

const QString VISIT_ID = "visit_id";
const QString VISIT_DATE = "visit_date";

// create queries
const QString CREATE_PATIENTS_TABLE = "CREATE TABLE " + PATIENTS_TABLE + " ("
              + PATIENT_ID + " INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, "
              + INIT_DATE + " TEXT NOT NULL, "
              + SURNAME + " TEXT NOT NULL, "
              + NAME + " TEXT NOT NULL, "
              + F_NAME + " TEXT NOT NULL, "
              + B_DATE + " TEXT NOT NULL, "
              + SEX + " TEXT NOT NULL, "
              + CITY + " TEXT NOT NULL, "
              + TEL_NUMBER + " TEXT NOT NULL, "
              + PATIENT_PHOTO + " BLOB)";

#endif // GENERAL

