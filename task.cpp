#include "task.h"

Task::Task(QObject *parent) : QObject(parent)
{

}

Task::Task(const QString &caption,
           const QString &status,
           const QDateTime &dateStarted,
           const QDateTime &dateFinished,
           const QTime &timeElapsed,
           QObject *parent) : QObject(parent)
{
    this->caption = caption;
    this->status = status;
    this->dateStarted = dateStarted;
    this->dateFinished = dateFinished;
    this->timeElapsed = timeElapsed;
}

Task::Task(const Task &other) : QObject(other.parent())
{
    this->setCaption(other.caption);
    this->setStatus(other.status);
    this->setDateStarted(other.dateStarted);
    this->setDateFinished(other.dateFinished);
    this->setTimeElapsed(other.timeElapsed);
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

void Task::setDateFinished(const QDateTime &value)
{
    dateFinished = value;
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
    this->setCaption(task.caption);
    this->setStatus(task.status);
    this->setDateStarted(task.dateStarted);
    this->setDateFinished(task.dateFinished);
    this->setTimeElapsed(task.timeElapsed);
    return *this;
}
