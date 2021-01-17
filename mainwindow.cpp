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

    model = new TaskModel(&tasks, this);
    filterModel = new TaskFilterModel(this);
    filterModel->setSourceModel(model);
    setDate(QDate::currentDate());

    QTime time(0, 0);
    ui->timeEdit->setTime(time);
    ui->tableView->setModel(filterModel);
    ui->stopButton->setHidden(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setDate(const QDate &date){
    selectedDate = date;
    ui->dateEdit->setDate(date);
    filterModel->setDate(date);
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
    addNewTask();
    ui->startButton->setHidden(true);
    ui->stopButton->setHidden(false);
}

void MainWindow::on_stopButton_clicked()
{
    taskTimer->stop();
    finishTask();
    QTime time(0, 0);
    ui->timeEdit->setTime(time);
    ui->stopButton->setHidden(true);
    ui->startButton->setHidden(false);
}

void MainWindow::on_pushButton_clicked()
{
    DateDialog dlg(selectedDate, this);
    dlg.setModal(true);
    connect(&dlg, &DateDialog::dateChanged, this, &MainWindow::setDate);
    dlg.exec();
}
