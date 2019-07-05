#include "sqlqueries.h"

SqlQueries::SqlQueries(){}

QString SqlQueries::CreatePatientsTable() {
    return "CREATE TABLE " + PATIENTS_TABLE + " ("
            + PATIENT_ID + " INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, "
            + PATIENT_INIT_DATE + " TEXT NOT NULL, "
            + PATIENT_LAST_CHANGES + " TEXT NOT NULL, "
            + SURNAME + " TEXT NOT NULL, "
            + NAME + " TEXT NOT NULL, "
            + F_NAME + " TEXT NOT NULL, "
            + B_DATE + " TEXT NOT NULL, "
            + SEX + " TEXT NOT NULL, "
            + CITY + " TEXT NOT NULL, "
            + TEL_NUMBER + " TEXT NOT NULL, "
            + ILLNESSES + " TEXT NOT NULL, "
            + PATIENT_PHOTO + " BLOB)";
}

QString SqlQueries::CreateEventsTable() {
    return "CREATE TABLE " + EVENTS_TABLE + " ("
            + EVENT_ID + " INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, "
            + EVENT_INIT_DATE + " TEXT NOT NULL, "
            + EVENT_LAST_CHANGES + " TEXT NOT NULL, "
            + EVENT_DATE + " TEXT NOT NULL, "
            + EVENT_TIME_FROM + " TEXT NOT NULL, "
            + EVENT_TIME_TO + " TEXT NOT NULL, "
            + PATIENT + " TEXT NOT NULL, "
            + EVENT_STATUS + " TEXT NOT NULL, "
            + COMMENT + " TEXT, "
            + PATIENT_ID + " INTEGER)";
}

QString SqlQueries::CreateVisitsTable() {
    return "CREATE TABLE " + VISITS_TABLE + " ("
            + VISIT_ID + " INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, "
            + VISIT_INIT_DATE + " TEXT NOT NULL, "
            + VISIT_LAST_CHANGES + " TEXT NOT NULL, "
            + VISIT_DATE + " TEXT NOT NULL, "
            + PATIENT + " TEXT NOT NULL, "
            + PRICE + " REAL NOT NULL, "
            + VISIT_RESULT + " TEXT NOT NULL, "
            + EVENT_ID + " INTEGER NOT NULL)";
}

QString SqlQueries::MaxVisitDateQuery(const QString& patient_id) {
    return "SELECT max(" +  VISIT_DATE + ") FROM"
            " (SELECT " + VISITS_TABLE + "." + VISIT_DATE + ", " + EVENTS_TABLE + "." + PATIENT_ID + " FROM " + VISITS_TABLE +
            " INNER JOIN " + EVENTS_TABLE + " ON " + VISITS_TABLE + "." + EVENT_ID + " = " + EVENTS_TABLE + "." + EVENT_ID + ")"
            " WHERE " + PATIENT_ID + " = " + patient_id;
}

QString SqlQueries::PatientVisits(const QString& patient_id) {
    return "SELECT " +  VISIT_DATE + ", " + PRICE + ", " + VISIT_RESULT + " FROM"
           " (SELECT " + VISITS_TABLE + "." + VISIT_DATE + ", " + VISITS_TABLE + "." + PRICE + ", " + VISITS_TABLE + "." + VISIT_RESULT + ", " + EVENTS_TABLE + "." + PATIENT_ID + " FROM " + VISITS_TABLE +
           " INNER JOIN " + EVENTS_TABLE + " ON " + VISITS_TABLE + "." + EVENT_ID + " = " + EVENTS_TABLE + "." + EVENT_ID + ")"
                                                                                                                            " WHERE " + PATIENT_ID + " = " + patient_id;
}

QString SqlQueries::VisitsForReport(const QString &date_from, const QString &date_to)
{
    return "SELECT " +  VISIT_DATE + ", " + PATIENT + ", " + PRICE + ", " + VISIT_RESULT + " FROM " + VISITS_TABLE +
            " WHERE " +  VISIT_DATE + " BETWEEN '" + date_from + "' AND '" + date_to + "'";
}

QString SqlQueries::CheckDateTimeFrom(const QString &date, const QString &time_from, const QString &event_id) {
    return "SELECT " + PATIENT + " FROM " + EVENTS_TABLE + " WHERE " + EVENT_DATE + " = '" + date +
            "' AND (" + EVENT_TIME_FROM + " <= '" + time_from +
            "' AND " + EVENT_TIME_TO + " > '" + time_from + "')" +
            " AND " + EVENT_STATUS + " = '" + STATUS_LIST[ACTIVE] + "'" +
            " AND " + EVENT_ID + " != '" + event_id + "'";
}

QString SqlQueries::CheckDateTimeTo(const QString &date, const QString &time_to, const QString &event_id) {
    return "SELECT " + PATIENT + " FROM " + EVENTS_TABLE + " WHERE " + EVENT_DATE + " = '" + date +
            "' AND (" + EVENT_TIME_FROM + " < '" + time_to +
            "' AND " + EVENT_TIME_TO + " > '" + time_to + "')" +
            " AND " + EVENT_STATUS + " = '" + STATUS_LIST[ACTIVE] + "'" +
            " AND " + EVENT_ID + " != '" + event_id + "'";
}

QString SqlQueries::CheckPatientDuplication(const QString &surname, const QString &name, const QString &f_name, const QString &b_date) {
    return "SELECT " + PATIENT_ID + " FROM " + PATIENTS_TABLE +
            " WHERE " + SURNAME  + " = '" + surname + "' AND "+ NAME + " = '" + name + "' AND " + F_NAME + " = '" + f_name + "' AND " + B_DATE + " = '" + b_date + "'";
}
