#ifndef EVENTSPROXYMODEL_H
#define EVENTSPROXYMODEL_H

#include "general.h"

class MyEventsProxyModel : public QSortFilterProxyModel
{
public:
    MyEventsProxyModel(QObject* parent = nullptr);
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
};

#endif // EVENTSPROXYMODEL_H
