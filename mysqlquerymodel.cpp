#include "mysqlquerymodel.h"

MySqlQueryModel::MySqlQueryModel(QObject *parent) : QSqlQueryModel (parent) { }

QVariant MySqlQueryModel::data(const QModelIndex &index, int role) const {
    if(!index.isValid()){
        return QVariant();
    }
    if (role == Qt::DisplayRole ) {
        if (index.column() == 0 /*VISIT_DATE_COL*/){
            return QDate::fromString(QSqlQueryModel::data(index,role).toString(), SQL_DATE_FORMAT).toString(DATE_FORMAT);
        }
        if (index.column() == 1 /*PRICE_COL*/){
            QLocale loc(QLocale::Ukrainian, QLocale::Ukraine);
            return loc.toString(QSqlQueryModel::data(index,role).toDouble (), 'f', 2);
        }
    }
    return QSqlQueryModel::data(index, role);
}
