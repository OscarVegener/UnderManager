#include "taskmodel.h"

TaskModel::TaskModel(QVector<Task> *vec, QObject *parent) : QAbstractTableModel(parent)
{
    this->vec = vec;
}

int TaskModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return vec->size();
}

int TaskModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 6;
}

QVariant TaskModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()){
        return QVariant();
    }
    if (index.row() >= vec->size()){
        return QVariant();
    }
    if ((role == Qt::DisplayRole || role == Qt::EditRole) && index.column() == 0){
        return vec->at(index.row()).getCaption();
    }
    if ((role == Qt::DisplayRole || role == Qt::EditRole) && index.column() == 1){
        return vec->at(index.row()).getStatus();
    }
    if (role == Qt::DisplayRole && index.column() == 2){
        return vec->at(index.row()).getDateStarted();
    }
    if (role == Qt::DisplayRole && index.column() == 3){
        if (vec->at(index.row()).isFinished()){
            return vec->at(index.row()).getDateFinished();
        }
        else{
            return QVariant();
        }
    }
    if (role == Qt::DisplayRole && index.column() == 4){
        if (vec->at(index.row()).isFinished()){
            return vec->at(index.row()).getTimeElapsed().toString();
        }
        else{
            return QVariant();
        }
    }
    if (role == Qt::DisplayRole && index.column() == 5){
        if (vec->at(index.row()).isFinished()){
            return vec->at(index.row()).getDaysElapsed();
        }
        else{
            return QVariant();
        }
    }
    else{
        return QVariant();
    }
}

QVariant TaskModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole){
        return QVariant();
    }
    if (orientation == Qt::Horizontal && section == 0){
        return QString("Caption");
    }
    if (orientation == Qt::Horizontal && section == 1){
        return QString("Status");
    }
    if (orientation == Qt::Horizontal && section == 2){
        return QString("Date started");
    }
    if (orientation == Qt::Horizontal && section == 3){
        return QString("Date finished");
    }
    if (orientation == Qt::Horizontal && section == 4){
        return QString("Time elapsed");
    }
    if (orientation == Qt::Horizontal && section == 5){
        return QString("Days elapsed");
    }
    return QVariant();
}

Qt::ItemFlags TaskModel::flags(const QModelIndex &index) const
{
    if (!index.isValid()){
        return Qt::ItemIsEnabled;
    }
    if (index.column() == 0 || index.column() == 1){
        return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
    }
    return QAbstractItemModel::flags(index);
}

bool TaskModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid()) {
        if (index.column() == 0){
            (*vec)[index.row()].setCaption(value.toString());
            emit dataChanged(index, index, {role});
            return true;
        }
        if (index.column() == 1){
            (*vec)[index.row()].setStatus(value.toString());
            emit dataChanged(index, index, {role});
            return true;
        }
        if (index.column() == 2){
            (*vec)[index.row()].setDateStarted(value.toDateTime());
            emit dataChanged(index, index, {role});
            return true;
        }
        if (index.column() == 3){
            (*vec)[index.row()].setDateFinished(value.toDateTime());
            emit dataChanged(index, this->index(index.row(), 5), {role});
            return true;
        }
        if (index.column() == 4){
            (*vec)[index.row()].setTimeElapsed(value.toTime());
            emit dataChanged(index, index, {role});
            return true;
        }
        if (index.column() == 5){
            (*vec)[index.row()].setDaysElapsed(value.toULongLong());
            emit dataChanged(index, index, {role});
            return true;
        }
    }
    return false;
}

bool TaskModel::insertRows(int position, int rows, const QModelIndex &index)
{
    Q_UNUSED(index);
    beginInsertRows(QModelIndex(), position, position+rows-1);
    for (int row = 0; row < rows; ++row){
        vec->insert(position, Task());
    }
    endInsertRows();
    return true;
}

bool TaskModel::removeRows(int position, int rows, const QModelIndex &index)
{
    Q_UNUSED(index);
    beginRemoveRows(QModelIndex(), position, position+rows-1);
    for (int row = 0; row < rows; ++row){
        vec->removeAt(position);
    }
    endRemoveRows();
    return true;
}

void TaskModel::addTask(const Task &task)
{
    if (vec->at(0).isFinished()){
        beginInsertRows(QModelIndex(), 0, 0);
        vec->insert(0, task);
        endInsertRows();
    }
    else{
        beginInsertRows(QModelIndex(), 1, 1);
        vec->insert(1, task);
        endInsertRows();
    }
}
