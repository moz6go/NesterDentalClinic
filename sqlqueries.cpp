#include "sqlqueries.h"

SqlQueries::SqlQueries(){}

QString SqlQueries::GetMaxVisitDateQuery(const QString& patient_id) {
    return "SELECT max(" +  VISIT_DATE + ") FROM"
            " (SELECT " + VISITS_TABLE + "." + VISIT_DATE + ", " + EVENTS_TABLE + "." + PATIENT_ID + " FROM " + VISITS_TABLE +
            " INNER JOIN " + EVENTS_TABLE + " ON " + VISITS_TABLE + "." + EVENT_ID + " = " + EVENTS_TABLE + "." + EVENT_ID + ")"
            " WHERE " + PATIENT_ID + " = " + patient_id;
}

QString SqlQueries::GetPatientVisits(const QString& patient_id) {
    return "SELECT " +  VISIT_DATE + ", " + PRICE + ", " + VISIT_RESULT + " FROM"
           " (SELECT " + VISITS_TABLE + "." + VISIT_DATE + ", " + VISITS_TABLE + "." + PRICE + ", " + VISITS_TABLE + "." + VISIT_RESULT + ", " + EVENTS_TABLE + "." + PATIENT_ID + " FROM " + VISITS_TABLE +
           " INNER JOIN " + EVENTS_TABLE + " ON " + VISITS_TABLE + "." + EVENT_ID + " = " + EVENTS_TABLE + "." + EVENT_ID + ")"
           " WHERE " + PATIENT_ID + " = " + patient_id;
}
