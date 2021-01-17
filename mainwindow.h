#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QTime>
#include <QDate>
#include <QVector>
#include <QFile>
#include <QFileDialog>
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

    QString filename = "tasks.dat";

    void addNewTask();

    void finishTask();

    void save(const QString &path);

    void load(const QString &path);

signals:
    void selectedDateChanged();

private slots:
    void updateTimer();
    void on_startButton_clicked();
    void on_stopButton_clicked();
    void on_pushButton_clicked();
    void on_actionLoad_triggered();
    void on_actionSave_as_triggered();
};
#endif // MAINWINDOW_H
