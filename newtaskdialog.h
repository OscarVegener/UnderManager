#ifndef NEWTASKDIALOG_H
#define NEWTASKDIALOG_H

#include <QDialog>
#include "task.h"

namespace Ui {
class NewTaskDialog;
}

class NewTaskDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewTaskDialog(QWidget *parent = nullptr);
    ~NewTaskDialog();

signals:
    void taskCreated(const Task &task);

private:
    void updateTimeEstimated();

private slots:
    void on_buttonBox_accepted();

    void on_dateTimeEditStarted_dateTimeChanged(const QDateTime &dateTime);

    void on_dateTimeEditFinished_dateTimeChanged(const QDateTime &dateTime);

    void on_dateTimeEditStarted_dateChanged(const QDate &date);

    void on_dateTimeEditFinished_dateChanged(const QDate &date);

    void on_dateTimeEditStarted_timeChanged(const QTime &time);

    void on_dateTimeEditFinished_timeChanged(const QTime &time);

private:
    Ui::NewTaskDialog *ui;
};

#endif // NEWTASKDIALOG_H
