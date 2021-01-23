#include "updatetodaytimeworker.h"

UpdateTodayTimeWorker::UpdateTodayTimeWorker(const QVector<Task> *const tasks, QObject *parent) : QObject(parent)
{
    vec = tasks;
}

void UpdateTodayTimeWorker::calculateTodayTime()
{

    QTime time(0, 0);
    quint64 secs = 0;
    QDateTime startDate = QDateTime::currentDateTime();
    startDate.setTime(time);
    if (vec->size() > 0){
        for (int i = 0; i < vec->size(); ++i){
            if (vec->at(i).isFinished() && vec->at(i).getDateStarted().date() == QDate::currentDate()){
                secs += time.secsTo(vec->at(i).getTimeElapsed());
            }
        }
    }
    time = time.addSecs(secs);
    emit timeCalculated(time);
}
