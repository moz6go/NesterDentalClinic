#include "eventsproxymodel.h"

EventsProxyModel::EventsProxyModel(QObject *parent) : QSortFilterProxyModel (parent)
{
}

QVariant EventsProxyModel::data(const QModelIndex &item, int role) const
{
    if(!item.isValid()){
        return QVariant();
    }
    if (role == Qt::TextColorRole) {
        QColor color;
        if(this->data(this->index(item.row(), EVENT_STATUS_COL)).toString() == STATUS_LIST[CANCELED]) {
            color = Qt::darkGray;
        }
        else if (this->data(this->index(item.row(), EVENT_STATUS_COL)).toString() == STATUS_LIST[EXECUTED]) {
            color = Qt::darkGreen;
        }
        else if (this->data(this->index(item.row(), EVENT_PATIENT_ID_COL)).toString().isEmpty ()) {
            color = Qt::red;
        }
        return QBrush(color);
    }
    if (role == Qt::DisplayRole && item.column() == EVENT_DATE_COL) {
        return QDate::fromString(QSortFilterProxyModel::data(item,role).toString(), SQL_DATE_FORMAT).toString(DATE_FORMAT);
    }
    return QSortFilterProxyModel::data(item, role);
}
