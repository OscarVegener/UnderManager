#include "taskfiltermodel.h"

TaskFilterModel::TaskFilterModel(QObject *parent) : QSortFilterProxyModel(parent)
{

}

void TaskFilterModel::setDate(const QDate &date)
{
    this->date = date;
    invalidateFilter();
}

bool TaskFilterModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    Q_UNUSED(sourceParent);
    if (sourceModel()->data(sourceModel()->index(sourceRow, 2)).toDate() == date){
        return true;
    }
    if (sourceModel()->data(sourceModel()->index(sourceRow, 3)).toDate() == date){
        return true;
    }
    if (sourceModel()->data(sourceModel()->index(sourceRow, 2)).toDate() < date
            && !sourceModel()->data(sourceModel()->index(sourceRow, 3)).toDate().isValid()){
        return true;
    }
    return false;
}
