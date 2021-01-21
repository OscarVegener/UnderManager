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
#include <QThread>
#include <QMutex>
#include <QMutexLocker>
#include <QSystemTrayIcon>
#include <QCloseEvent>
#include "datedialog.h"
#include "taskmodel.h"
#include "taskfiltermodel.h"
#include "aboutdialog.h"
#include "newtaskdialog.h"
#include "preferences.h"
#include "updatetodaytimeworker.h"

#define CURRENT_VERSION 210

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
    //Sets date that is used to display tasks
    void setDate(const QDate& date);

private:
    Ui::MainWindow *ui;
    QTimer *taskTimer;
    QDate selectedDate;
    TaskModel *model;
    TaskFilterModel *filterModel;

    QVector<Task> tasks;

    QString filename = "tasks.dat";

    //Functions to create tasks with standard start/stop mechanism
    void addNewTask();
    void finishTask();

    //File load/save related functions
    void save(const QString &path);
    void load(const QString &path);
    void loadUnfinishedTask();
    void deleteUnfinishedTask();

    //Function to call when exit
    //This function handles saving of tasks and settings
    void exit();

    //Context menu for TableView
    QMenu *contextMenu;
    QAction *deleteAction;
    QAction *copyAction;
    QAction *newTaskAction;
    void initContextMenu();

    //Clipboard
    QClipboard *clipBoard;

    //Settings
    QSettings *settings;
    void writeSettings();
    void writeDefaultSettings();
    void readSettings();

    //Thread for calculating time elapsed for today's finished tasks
    QThread workerThread;
    UpdateTodayTimeWorker *worker;
    //Mutex to lock when tasks is loading from file
    QMutex tasksMutex;

    //System tray
    QSystemTrayIcon *trayIcon;
    QMenu *trayContextMenu;
    QAction *closeAction;
    QAction *showAction;
    void initTrayContextMenu();

signals:
    //Signal to emit when need to update total time elapsed for today's finished tasks
    void updateTodayTimeElapsedNeeded();

private slots:
    //Slot to update time elapsed and days elapsed in ui
    void updateTimer();

    //auto slots
    void on_startButton_clicked();
    void on_stopButton_clicked();
    void on_pushButton_clicked();
    void on_actionLoad_triggered();
    void on_actionSave_as_triggered();
    void on_actionExit_triggered();
    void on_actionAbout_triggered();
    void on_tableView_customContextMenuRequested(const QPoint &pos);
    void on_actionPreferences_triggered();

    //slots for TableView context menu
    void deleteItem();
    void copyItem();
    void newItem();

    //slot that updates total time elapsed for today's finished tasks in ui
    void setTodayTimeElapsed(const QTime & time);

    //slots for tray context menu
    void trayShow();

};

#endif // MAINWINDOW_H
