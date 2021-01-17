#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    taskTimer = new QTimer(this);
    taskTimer->setInterval(1000);
    connect(taskTimer, &QTimer::timeout, this, &MainWindow::updateTimer);

    setDate(QDate::currentDate());
    model = new TaskModel(&tasks);
    ui->tableView->setModel(model);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setDate(const QDate &date){
    selectedDate = date;
    ui->dateEdit->setDate(date);
    emit selectedDateChanged();
}

void MainWindow::addNewTask()
{
    model->insertRows(0, 1);
    model->setData(model->index(0, 0), "Unnamed task");
    model->setData(model->index(0, 1), "Unknown status");
    model->setData(model->index(0, 2), QDateTime::currentDateTime());
}

void MainWindow::finishTask()
{
    model->setData(model->index(0, 3), QDateTime::currentDateTime());
    int secs = model->data(model->index(0, 2), Qt::DisplayRole).toDateTime().secsTo(QDateTime::currentDateTime());
    QTime time(0, 0);
    time = time.addSecs(secs);
    model->setData(model->index(0, 4), time);
}

void MainWindow::updateTimer()
{
    ui->timeEdit->setTime(ui->timeEdit->time().addMSecs(1000));
}


void MainWindow::on_startButton_clicked()
{
    taskTimer->start();
    QTime time(0, 0);
    ui->timeEdit->setTime(time);
    addNewTask();
}

void MainWindow::on_stopButton_clicked()
{
    taskTimer->stop();
    finishTask();
}

void MainWindow::on_pushButton_clicked()
{
    DateDialog dlg(this);
    dlg.setModal(true);
    connect(&dlg, &DateDialog::dateChanged, this, &MainWindow::setDate);
    dlg.exec();
}
