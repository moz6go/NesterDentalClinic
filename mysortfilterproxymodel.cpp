#include "mysortfilterproxymodel.h"

MySortFilterProxyModel::MySortFilterProxyModel(QObject *parent) : QSortFilterProxyModel (parent)
{
}

QVariant MySortFilterProxyModel::data(const QModelIndex &index, int role) const
{
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
    return QSortFilterProxyModel::data(index, role);
}
