#ifndef TASKFILTERMODEL_H
#define TASKFILTERMODEL_H

#include <QSortFilterProxyModel>
#include <QObject>

class TaskFilterModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    TaskFilterModel();
};

#endif // TASKFILTERMODEL_H
