#include "myeventsproxymodel.h"

MyEventsProxyModel::MyEventsProxyModel(QObject *parent) : QSortFilterProxyModel (parent)
{
}

QVariant MyEventsProxyModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid()){
        return QVariant();
    }
    if (role == Qt::TextColorRole) {
        QColor color;
        if(this->data(this->index(index.row(), EVENT_STATUS_COL)).toString() == STATUS_LIST[CANCELED]) {
            color = Qt::darkGray;
        }
        else if (this->data(this->index(index.row(), EVENT_STATUS_COL)).toString() == STATUS_LIST[EXECUTED]) {
            color = Qt::darkGreen;
        }
        else if (this->data(this->index(index.row(), EVENT_PATIENT_ID_COL)).toString().isEmpty ()) {
            color = Qt::red;
        }
        return QBrush(color);
    }
    if (role == Qt::DisplayRole && index.column() == EVENT_DATE_COL) {
        return QDate::fromString(QSortFilterProxyModel::data(index,role).toString(), SQL_DATE_FORMAT).toString(DATE_FORMAT);
    }
    return QSortFilterProxyModel::data(index, role);
}
