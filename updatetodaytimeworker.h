#ifndef UPDATETODAYTIMEWORKER_H
#define UPDATETODAYTIMEWORKER_H

#include <QObject>
#include "task.h"

class UpdateTodayTimeWorker : public QObject
{
    Q_OBJECT
public:
    explicit UpdateTodayTimeWorker(const QVector<Task> *const tasks, QObject *parent = nullptr);

    void calculateTodayTime();

private:
    const QVector<Task> *vec;

signals:
    void timeCalculated(const QTime &time);

};

#endif // UPDATETODAYTIMEWORKER_H
