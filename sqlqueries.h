#ifndef SQLQUERIES_H
#define SQLQUERIES_H
#include "general.h"

class SqlQueries
{
public:
    SqlQueries();
    static QString GetMaxVisitDateQuery(const QString& patient_id);
    static QString GetPatientVisits(const QString& patient_id);
};

#endif // SQLQUERIES_H
