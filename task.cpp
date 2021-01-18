#include "task.h"

Task::Task(QObject *parent) : QObject(parent)
{

}

Task::Task(const QString &caption,
           const QString &status,
           const QDateTime &dateStarted,
           const QDateTime &dateFinished,
           const QTime &timeElapsed,
           const quint64 &daysElapsed,
           const bool &finished,
           QObject *parent) : QObject(parent)
{
    this->caption = caption;
    this->status = status;
    this->dateStarted = dateStarted;
    this->dateFinished = dateFinished;
    this->timeElapsed = timeElapsed;
    this->daysElapsed = daysElapsed;
    this->finished = finished;
}

Task::Task(const Task &other) : QObject(other.parent())
{
    caption = other.caption;
    status = other.status;
    dateStarted = other.dateStarted;
    dateFinished = other.dateFinished;
    timeElapsed = other.timeElapsed;
    daysElapsed = other.daysElapsed;
    finished = other.finished;
}

QString Task::getCaption() const
{
    return caption;
}

void Task::setCaption(const QString &value)
{
    caption = value;
}

QString Task::getStatus() const
{
    return status;
}

void Task::setStatus(const QString &value)
{
    status = value;
}

QDateTime Task::getDateStarted() const
{
    return dateStarted;
}

void Task::setDateStarted(const QDateTime &value)
{
    dateStarted = value;
}

QDateTime Task::getDateFinished() const
{
    return dateFinished;
}

void Task::setDateFinished(const QDateTime &value, bool update)
{
    dateFinished = value;
    if (update){
        updateTimeDaysElapsed();
    }
    finished = true;
}

QTime Task::getTimeElapsed() const
{
    return timeElapsed;
}

void Task::setTimeElapsed(const QTime &value)
{
    timeElapsed = value;
}

Task &Task::operator=(const Task &task)
{
    if (this == &task){
        return *this;
    }
    this->caption = task.caption;
    this->status = task.status;
    this->dateStarted = task.dateStarted;
    this->dateFinished = task.dateFinished;
    this->timeElapsed = task.timeElapsed;
    this->daysElapsed = task.daysElapsed;
    this->finished = task.finished;
    return *this;
}

quint64 Task::getDaysElapsed() const
{
    return daysElapsed;
}

void Task::setDaysElapsed(const quint64 &value)
{
    daysElapsed = value;
}

bool Task::isFinished() const
{
    return finished;
}

void Task::setFinished(const bool &value)
{
    finished = value;
}

void Task::updateTimeDaysElapsed()
{
    if (dateFinished > dateStarted){
        QTime time(0, 0);
        time = time.addSecs(getDateStarted().secsTo(getDateFinished()));
        setTimeElapsed(time);
        setDaysElapsed(getDateStarted().daysTo(getDateFinished()));
    }
    else{
        setTimeElapsed(QTime(0, 0));
        setDaysElapsed(0);
    }
}
