#include "database.h"

DataBase::DataBase(const QStringList& create_tables_queries, QObject *parent) : QObject(parent) {
    create_queries = create_tables_queries;
}

DataBase::~DataBase() {
    CloseDataBase();
}

QString DataBase::LastError() {
    return last_error;
}

bool DataBase::ConnectToDataBase (QString db_path) {
    if(!QFile(db_path).exists()){
        if(!RestoreDataBase()){
            return false;
        }
        return true;
    }
    else {
        if (!OpenDataBase(db_path)) {
            return false;
        }
        return true;
    }
}

bool DataBase::OpenDataBase(QString db_path) {
    sdb = QSqlDatabase::addDatabase("QSQLITE");
    sdb.setDatabaseName(db_path);
    if(sdb.open()) {
        return true;
    }
    else {
        last_error = sdb.lastError ().text ();
        return false;
    }
}

bool DataBase::RestoreDataBase() {
    if(OpenDataBase (DB_PATH)){
        if(!CreateDataBase ()) {
            return false;
        }
        return true;
    }
    else {
        return false;
    }
}

bool DataBase::CreateDataBase() {
    QSqlQuery query;

    for (auto& cr_query : create_queries) {
        if(!query.exec (cr_query))  {
            last_error = sdb.lastError ().text ();
            return false;
        }
    }
    return true;
}

void DataBase::CloseDataBase(){
    QString connection = sdb.connectionName();
    sdb.close();
    sdb = QSqlDatabase();
    sdb.removeDatabase(connection);
}

bool DataBase::UpdateInsertData (const QString& query_str, const QStringList& bind_values_list, const QVariantList& data) {
    QSqlQuery query;
    query.prepare (query_str);
    for (int i = 0; i < bind_values_list.size (); ++i) {
        query.bindValue (bind_values_list[i], data[i]);
    }
    if(!query.exec ()) {
        qDebug() << query.lastError ();
        return false;
    }
    else {
        return true;
    }
}
QString DataBase::Select(const QString &query) {
    QSqlQuery sel_query;
    sel_query.exec (query);
    sel_query.next ();
    return  sel_query.isValid () ? sel_query.value(0).toString () : QString();
}

int DataBase::SelectCount(const QString &query) {
    QSqlQuery sel_query;
    sel_query.exec (query);
    sel_query.next ();
    return sel_query.isValid () ? sel_query.value (0).toInt (): 0;
}

double DataBase::SelectSum(const QString &query) {
    QSqlQuery sel_query;
    sel_query.exec (query);
    sel_query.next ();
    return sel_query.isValid () ? sel_query.value (0).toDouble () : 0.0;
}

QVector<QVariantList> DataBase::SelectTable(const QString &str_query) {
    QVector<QVariantList> table;
    QSqlQuery query;
    query.exec (str_query);
    QSqlRecord rec = query.record ();

    QVariantList header;
    for(int col = 0; col < rec.count(); ++col){
        QSqlField field = rec.field(col);
        header.append (field.name());
    }
    table.append (header);

    while(query.next ()){
        QVariantList row;
        for(int col = 0; col < rec.count(); ++col) {
            if (rec.field(col).type () == QVariant::Double){
                row.append (QVariant(query.value (col).toString ().replace(" ", "").replace (".", ",")));
            }
            else {
                row.append (query.value (col));
            }
        }
        table.append (row);
    }
    return table;
}

int DataBase::SelectCount(const QString &from, const QString &where, const QString& equal_sign, const QString &equal) {
    QSqlQuery sel_query;
    sel_query.exec ("SELECT COUNT(*) FROM " + from + " WHERE " + where + " " + equal_sign + " '" + equal + "'");
    sel_query.next ();
    return sel_query.isValid () ? sel_query.value (0).toInt (): 0;
}

int DataBase::SelectCount(const QString& from, const QString& where1, const QString& where2, const QString& equal_sign1, const QString& equal_sign2, const QString& equal1, const QString& equal2) {
    QSqlQuery sel_query;
    sel_query.exec ("SELECT COUNT(*) FROM " + from + " WHERE " + where1 + " " + equal_sign1 + " '" + equal1 + "' AND " + where2 + " " + equal_sign2 + " " + equal2 );
    sel_query.next ();
    return sel_query.isValid () ? sel_query.value (0).toInt (): 0;
}

bool DataBase::DeleteRow(const QString &from, const QString &where, const QString &equal) {
    QSqlQuery query;
    return query.exec ("DELETE FROM " + from + " WHERE " + where + " = '" + equal + "'");
}

QByteArray DataBase::SelectPic(const QString &from, const QString &where, const QString &equal) {
    QSqlQuery query;
    query.exec ("SELECT pic FROM " + from + " WHERE " + where + " = '" + equal + "'");
    QSqlRecord rec = query.record ();
    query.next ();
    return query.value (rec.indexOf ("pic")).toByteArray ();
}

QString DataBase::Select(const QString &select, const QString &from, const QString &where, const QString &equal) {
    QSqlQuery query;
    query.exec ("SELECT " + select + " FROM " + from + " WHERE " + where + " = '" + equal + "'");
    QSqlRecord rec = query.record ();
    query.next ();
    return  query.isValid () ? query.value(rec.indexOf(select)).toString () : QString();
}

QString DataBase::SelectMultiEqual(const QString &select, const QString &from, const QString &expresion) {
    QSqlQuery query;
    query.exec ("SELECT " + select + " FROM " + from + " WHERE " + expresion);
    QSqlRecord rec = query.record ();
    query.next ();
    return  query.isValid () ? query.value(rec.indexOf(select)).toString () : QString();
}

QVariantList DataBase::SelectRow(const QString &select,
                                 const QString &from,
                                 const QString &where,
                                 const QString &equal,
                                 const int& col_count)
{
    QSqlQuery query;
    query.exec ("SELECT " + select + " FROM "+ from + " WHERE " + where + " = '" + equal + "'");
    QVariantList data;
    query.next ();
    for (int i = 0; i < col_count; ++ i) {
        data.append (query.value (i));
    }
    return data;
}

QVariantList DataBase::SelectRow(const QString &select,
                                 const QString &from,
                                 const QString &where1,
                                 const QString &where2,
                                 const QString &equal1,
                                 const QString &equal2,
                                 const int& col_count)
{
    QSqlQuery query;
    query.exec ("SELECT " + select + " FROM "+ from + " WHERE " + where1 + " = '" + equal1 + "' AND " + where2 + " = '" + equal2 + "'");
    QVariantList data;
    query.next ();
    for (int i = 0; i < col_count; ++ i) {
        data.append (query.value (i));
    }
    return data;
}

QString DataBase::GenerateInsertQuery(const QString& table, const QStringList& columns) {
    QString query;
    query = "INSERT INTO " + table + " (";

    for (int col = 0; col < columns.size(); ++col) {
        if(col == columns.size () - 1) {
            query += columns[col] + ") ";
        }
        else {
            query += columns[col] + ", ";
        }
    }

    query += "VALUES (";

    for (int col = 0; col < columns.size(); ++col) {
        if(col == columns.size () - 1){
            query += ":" + columns[col] + ")";
        }
        else {
            query += ":" + columns[col] + ", ";
        }
    }
    return query;
}

QString DataBase::GenerateUpdateQuery(const QString &table, const QStringList &columns, const QString &where, const QString &equal) {
    QString query = "UPDATE " + table + " SET ";
    for (int col = 0; col < columns.size (); ++col) {
        if(col == columns.size () - 1) {
            query += columns.at(col) + " = :" + columns.at(col);
        }
        else {
            query += columns.at(col) + " = :" + columns.at(col) + ", ";
        }
    }
    query += " WHERE " + where + " = " + equal;
    return query;
}

QStringList DataBase::GenerateBindValues(QStringList columns) {
    QStringList bind_val;
    for (int i = 0; i < columns.size (); ++i) {
        bind_val.append (":" + columns[i]);
    }
    return bind_val;
}


