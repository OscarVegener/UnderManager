#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QTime>
#include <QDate>
#include <QVector>
#include <QDebug>
#include "datedialog.h"
#include "taskmodel.h"
#include "taskfiltermodel.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void setDate(const QDate& date);

private:
    Ui::MainWindow *ui;
    QTimer *taskTimer;
    QDate selectedDate;
    TaskModel *model;
    TaskFilterModel *filterModel;

    QVector<Task> tasks;

    void addNewTask();

    void finishTask();

signals:
    void selectedDateChanged();

private slots:
    void updateTimer();
    void on_startButton_clicked();
    void on_stopButton_clicked();
    void on_pushButton_clicked();
};
#endif // MAINWINDOW_H
