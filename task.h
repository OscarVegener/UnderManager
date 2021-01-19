#ifndef TASK_H
#define TASK_H

#include <QObject>
#include <QDateTime>

class Task : public QObject
{
    Q_OBJECT
public:
    explicit Task(QObject *parent = nullptr);

    explicit Task(const QString &caption,
                  const QString &status,
                  const QDateTime &dateStarted,
                  const QDateTime &dateFinished,
                  const QTime &timeElapsed,
                  const quint64 &daysElapsed,
                  const bool &finished = true,
                  QObject *parent = nullptr);

    explicit Task(const Task &other);

    QString getCaption() const;
    void setCaption(const QString &value);

    QString getStatus() const;
    void setStatus(const QString &value);

    QDateTime getDateStarted() const;
    void setDateStarted(const QDateTime &value);

    QDateTime getDateFinished() const;
    void setDateFinished(const QDateTime &value);

    QTime getTimeElapsed() const;
    void setTimeElapsed(const QTime &value);

    quint64 getDaysElapsed() const;
    void setDaysElapsed(const quint64 &value);

    bool isFinished() const;
    void setFinished(const bool &value);

    Task &operator=(const Task &);

signals:

private:
    QString caption;
    QString status;
    QDateTime dateStarted;
    QDateTime dateFinished;
    QTime timeElapsed;
    quint64 daysElapsed;
    bool finished = false;

    //void updateTimeDaysElapsed();

};

inline QDataStream &operator<<(QDataStream &out, const Task &task){
    out << task.getCaption();
    out << task.getStatus();
    out << task.getDateStarted();
    out << task.getDateFinished();
    out << task.getTimeElapsed();
    out << QString::number(task.getDaysElapsed());
    if (task.isFinished()){
        out << QString("True");
    }
    else{
        out << QString("False");
    }
    return out;
}

inline QDataStream &operator>>(QDataStream &in, Task &task){
    QString tempString;
    in >> tempString;
    task.setCaption(tempString);
    in >> tempString;
    task.setStatus(tempString);
    QDateTime tempDate;
    in >> tempDate;
    task.setDateStarted(tempDate);
    in >> tempDate;
    task.setDateFinished(tempDate);
    QTime tempTime;
    in >> tempTime;
    task.setTimeElapsed(tempTime);
    in >> tempString;
    task.setDaysElapsed(tempString.toULongLong());
    in >> tempString;
    if (tempString == "True"){
        task.setFinished(true);
    }
    else{
        task.setFinished(false);
    }
    return in;
}

#endif // TASK_H
