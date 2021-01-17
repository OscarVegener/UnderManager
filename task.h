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
                  QObject *parent = nullptr);

    Task(const Task &other);

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

    Task &operator=(const Task &);

signals:

private:
    QString caption;
    QString status;
    QDateTime dateStarted;
    QDateTime dateFinished;
    QTime timeElapsed;

};

#endif // TASK_H
