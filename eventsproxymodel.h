#ifndef EVENTSPROXYMODEL_H
#define EVENTSPROXYMODEL_H

#include "general.h"

class EventsProxyModel : public QSortFilterProxyModel
{
public:
    EventsProxyModel(QObject* parent = nullptr);
    QVariant data(const QModelIndex &item, int role = Qt::DisplayRole) const;
};

#endif // EVENTSPROXYMODEL_H
