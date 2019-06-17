#ifndef SQLQUERIES_H
#define SQLQUERIES_H
#include "general.h"

class SqlQueries
{
public:
    SqlQueries();
    static QString MaxVisitDateQuery(const QString& patient_id);
    static QString PatientVisits(const QString& patient_id);
    static QString VisitsForReport(const QString& date_from, const QString& date_to);
    static QString CheckDateTimeFrom(const QString& date, const QString& time_from, const QString& event_id);
    static QString CheckDateTimeTo(const QString& date, const QString& time_to, const QString& event_id);
    static QString CheckPatientDuplication(const QString& surname, const QString& name, const QString& f_name, const QString& b_date);
};

#endif // SQLQUERIES_H
