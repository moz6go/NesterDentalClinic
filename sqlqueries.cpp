#include "sqlqueries.h"

SqlQueries::SqlQueries(){}

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
