#include "newtaskdialog.h"
#include "ui_newtaskdialog.h"

NewTaskDialog::NewTaskDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewTaskDialog)
{
    ui->setupUi(this);
    ui->dateTimeEditStarted->setDateTime(QDateTime::currentDateTime());
    ui->dateTimeEditFinished->setDateTime(QDateTime::currentDateTime());
    QTime time(0, 0);
    ui->timeEdit->setTime(time);
    ui->labelDays->setHidden(true);
    ui->lineEditDays->setHidden(true);
    ui->lineEditDays->setText("0");
}

NewTaskDialog::~NewTaskDialog()
{
    delete ui;
}

void NewTaskDialog::updateTimeEstimated()
{
    QDateTime dateStarted = ui->dateTimeEditStarted->dateTime();
    QDateTime dateFinished = ui->dateTimeEditFinished->dateTime();
    if (dateFinished >= dateStarted){
        quint64 secs = dateStarted.secsTo(dateFinished);
        QTime timeElapsed(0, 0);
        timeElapsed = timeElapsed.addSecs(secs);
        ui->timeEdit->setTime(timeElapsed);
        quint64 days = dateStarted.daysTo(dateFinished);
        if (days > 0){
            ui->labelDays->setHidden(false);
            ui->lineEditDays->setHidden(false);
            ui->lineEditDays->setText(QString::number(days));
        }
        else{
            ui->labelDays->setHidden(true);
            ui->lineEditDays->setHidden(true);
        }
    }
    else{
        QTime timeElapsed(0, 0);
        ui->timeEdit->setTime(timeElapsed);
        ui->labelDays->setHidden(true);
        ui->lineEditDays->setHidden(true);
    }
}

void NewTaskDialog::on_buttonBox_accepted()
{
    QDateTime dateStarted = ui->dateTimeEditStarted->dateTime();
    QDateTime dateFinished = ui->dateTimeEditFinished->dateTime();
    QString caption = ui->lineEditCaption->text();
    QString status = ui->lineEditStatus->text();
    QTime timeElapsed = ui->timeEdit->time();
    quint64 daysElapsed = ui->lineEditDays->text().toULongLong();
    if (dateStarted.isValid() &&
            dateFinished.isValid() &&
            !caption.isEmpty() &&
            !status.isEmpty() &&
            timeElapsed.isValid()){
        emit taskCreated(Task(caption, status, dateStarted, dateFinished, timeElapsed, daysElapsed));
    }
}

void NewTaskDialog::on_dateTimeEditStarted_dateTimeChanged(const QDateTime &dateTime)
{
    Q_UNUSED(dateTime);
    updateTimeEstimated();
}

void NewTaskDialog::on_dateTimeEditFinished_dateTimeChanged(const QDateTime &dateTime)
{
    Q_UNUSED(dateTime);
    updateTimeEstimated();
}

void NewTaskDialog::on_dateTimeEditStarted_dateChanged(const QDate &date)
{
    Q_UNUSED(date);
    updateTimeEstimated();
}

void NewTaskDialog::on_dateTimeEditFinished_dateChanged(const QDate &date)
{
    Q_UNUSED(date);
    updateTimeEstimated();
}

void NewTaskDialog::on_dateTimeEditStarted_timeChanged(const QTime &time)
{
    Q_UNUSED(time);
    updateTimeEstimated();
}

void NewTaskDialog::on_dateTimeEditFinished_timeChanged(const QTime &time)
{
    Q_UNUSED(time);
    updateTimeEstimated();
}
