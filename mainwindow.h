#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QTime>
#include <QDate>
#include <QVector>
#include <QFile>
#include <QFileDialog>
#include <QMenu>
#include <QClipboard>
#include <QSettings>
#include <QMessageBox>
#include "datedialog.h"
#include "taskmodel.h"
#include "taskfiltermodel.h"
#include "aboutdialog.h"
#include "newtaskdialog.h"
#include "preferences.h"

#include <QDebug>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void closeEvent(QCloseEvent *event) override;

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
    void loadUnfinishedTask();
    void deleteUnfinishedTask();

    void exit();

    QMenu *contextMenu;
    QAction *deleteAction;
    QAction *copyAction;
    QAction *newTaskAction;
    void initContextMenu();

    QClipboard *clipBoard;

    QSettings *settings;
    void writeSettings();
    void writeDefaultSettings();
    void readSettings();

signals:
    void selectedDateChanged();

private slots:
    void updateTimer();
    void on_startButton_clicked();
    void on_stopButton_clicked();
    void on_pushButton_clicked();
    void on_actionLoad_triggered();
    void on_actionSave_as_triggered();
    void on_actionExit_triggered();
    void on_actionAbout_triggered();
    void on_tableView_customContextMenuRequested(const QPoint &pos);
    void deleteItem();
    void copyItem();
    void newItem();
    void on_actionPreferences_triggered();
};

#endif // MAINWINDOW_H
