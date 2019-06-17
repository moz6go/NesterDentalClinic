#ifndef MYSQLQUERYMODEL_H
#define MYSQLQUERYMODEL_H

#include "general.h"

class MySqlQueryModel : public QSqlQueryModel
{
public:
    MySqlQueryModel(QObject* parent = nullptr);
    QVariant data(const QModelIndex &index, int role) const;
};

#endif // MYSQLQUERYMODEL_H
