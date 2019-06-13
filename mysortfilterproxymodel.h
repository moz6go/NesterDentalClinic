#ifndef MYSORTFILTERPROXYMODEL_H
#define MYSORTFILTERPROXYMODEL_H

#include "general.h"

class MySortFilterProxyModel : public QSortFilterProxyModel
{
public:
    MySortFilterProxyModel(QObject* parent = nullptr);
    QVariant data(const QModelIndex &item, int role = Qt::DisplayRole) const;
};

#endif // MYSORTFILTERPROXYMODEL_H
