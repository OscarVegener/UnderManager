#ifndef TASKFILTERMODEL_H
#define TASKFILTERMODEL_H

#include <QSortFilterProxyModel>
#include <QObject>
#include <QDate>

class TaskFilterModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    TaskFilterModel(QObject *parent = nullptr);

    void setDate(const QDate &date);

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;

private:
    QDate date;
};

#endif // TASKFILTERMODEL_H
