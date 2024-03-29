#ifndef GENERAL
#define GENERAL

#include <QApplication>
#include <QObject>
#include <QMainWindow>
#include <QDialog>
#include <QtDebug>
#include <QDebug>
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
#include <QTimer>
#include <QBuffer>
#include <QMessageBox>
#include <QStyleFactory>
#include <QStandardPaths>
#include <QPen>
#include <QPainter>
#include <QCalendarWidget>
#include <QSortFilterProxyModel>
#include <QIdentityProxyModel>
#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <QFile>

#include <QtSql/QSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QSqlField>

const int SIZE_WID_1 = 36;
const int SIZE_WID_2 = 24;

#if defined(_WIN32)
    #if defined(QT_DEBUG)
        const QString DB_PATH = QDir::currentPath () + "/ndc_db.sqlite3";
    #else
        const QString DB_PATH = QStandardPaths::writableLocation(QStandardPaths::DataLocation) + "/NDC/ndc_db.sqlite3";
    #endif
#else
    const QString DB_PATH = QDir::currentPath () + "/ndc_db.sqlite3";
#endif
const QString DATE_FORMAT = "dd.MM.yyyy";
const QString TIME_FORMAT = "hh:mm";
const QString SQL_DATE_FORMAT = "yyyy-MM-dd";
const QString SQL_DATE_TIME_FORMAT = "yyyy-MM-dd hh:mm:ss";
const QString FS_DATE_TIME_FORMAT = "yyyy-MM-dd hh-mm-ss";
// tables
const QString PATIENTS_TABLE = "patients";
const QString EVENTS_TABLE = "events";
const QString VISITS_TABLE = "visits";
const QString SERVICES_TABLE = "services";
const QString DISEASES_TABLE = "diseases";

// columns
const QString PATIENT_ID = "patient_id";
const QString PATIENT_INIT_DATE = "patient_init_date";
const QString PATIENT_LAST_CHANGES = "patient_last_changes";
const QString SURNAME = "surname";
const QString NAME = "name";
const QString F_NAME = "f_name";
const QString B_DATE = "b_date";
const QString SEX = "sex";
const QString CITY = "city";
const QString TEL_NUMBER = "tel_number";
const QString ILLNESSES = "illnesses";
const QString PATIENT_PHOTO = "patient_photo";

const QString EVENT_ID = "event_id";
const QString EVENT_INIT_DATE = "event_init_date";
const QString EVENT_LAST_CHANGES = "event_last_changes";
const QString EVENT_DATE = "event_date";
const QString EVENT_TIME_FROM = "event_time_from";
const QString EVENT_TIME_TO = "event_time_to";
const QString PATIENT = "patient";
const QString EVENT_STATUS = "event_status";
const QString COMMENT = "comment";

const QString VISIT_ID = "visit_id";
const QString VISIT_INIT_DATE = "visit_init_date";
const QString VISIT_LAST_CHANGES = "visit_last_changes";
const QString VISIT_DATE = "visit_date";
const QString PRICE = "price";
const QString VISIT_RESULT = "visit_result";

const QString CREATE_COPY_DB = "Зробити резервну копію бази даних";
const QString RESTORE_DB = "Відновити з резервної копії";

const QStringList PATIENTS_HEADERS = {
    "Код пацієта",
    "Дата заведення картки",
    "Дата останніх змін в картці",
    "Прізвище",
    "Ім'я",
    "По-батькові",
    "Дата народження",
    "Стать",
    "Місто/село",
    "Номер телефону",
    "Алергічні та хронічні захворювання",
    "Фото"
};

const QStringList EVENTS_TABLE_HEADERS = {
    "Код події",
    "Дата додавання події",
    "Дата останніх змін",
    "Дата",
    "З",
    "По",
    "Пацієнт",
    "Статус",
    "Примітка",
    "Код пацієнта"
};

const QStringList STATUS_LIST = {
    "Активний",
    "Скасовано",
    "Виконано"
};

const QStringList VISITS_TABLE_HEADERS = {
    "Код візиту",
    "Дата додавання візиту",
    "Дата останніх змін",
    "Дата візиту",
    "Пацієнт",
    "Вартість послуг, грн",
    "Результат огляду/лікування",
    "Код події"
};

const QStringList REPORTS_TYPES = {
    "Виконані прийоми за період"
};

enum PatientsCol {
    PATIENT_ID_COL,
    PATIENT_INIT_DATE_COL,
    PATIENT_LAST_CHANGES_COL,
    SURNAME_COL,
    NAME_COL,
    F_NAME_COL,
    B_DATE_COL,
    SEX_COL,
    CITY_COL,
    TEL_NUMBER_COL,
    ILLNESSES_COL,
    PATIENT_PHOTO_COL
};

enum EventsCol{
    EVENT_ID_COL,
    EVENT_INIT_DATE_COL,
    EVENT_LAST_CHANGES_COL,
    EVENT_DATE_COL,
    EVENT_TIME_FROM_COL,
    EVENT_TIME_TO_COL,
    PATIENT_COL,
    EVENT_STATUS_COL,
    COMMENT_COL,
    EVENT_PATIENT_ID_COL
};

enum EventsStatus {
    ACTIVE,
    CANCELED,
    EXECUTED
};

enum VisitsCol{
    VISIT_ID_COL,
    VISIT_INIT_DATE_COL,
    VISIT_LAST_CHANGES_COL,
    VISIT_DATE_COL,
    VISIT_PATIENT_COL,
    PRICE_COL,
    VISIT_RESULT_COL,
    VISIT_EVENT_ID_COL,
};

enum Role {
    ADD,
    EDIT,
    READ
};

enum ReportsTypes {
    VISITS_REPORT
};

#endif // GENERAL

