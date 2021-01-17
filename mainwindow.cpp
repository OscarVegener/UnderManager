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

    load(filename);

    initContextMenu();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    exit();
    QMainWindow::closeEvent(event);
}

void MainWindow::setDate(const QDate &date){
    selectedDate = date;
    ui->dateEdit->setDate(date);
    filterModel->setDate(date);
}

void MainWindow::initContextMenu()
{
    contextMenu = new QMenu(this);
    deleteAction = new QAction(tr("&Delete"), contextMenu);
    connect(deleteAction, &QAction::triggered, this, &MainWindow::deleteTask);
    contextMenu->addAction(deleteAction);
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
    save(filename);
}

void MainWindow::save(const QString &path)
{
    QFile file(path);
    if (file.open(QIODevice::WriteOnly)){
        QDataStream out(&file);
        out << tasks;
        file.close();
    }
}

void MainWindow::load(const QString &path)
{
    QFile file(path);
    if (file.open(QIODevice::ReadOnly)){
        QDataStream in(&file);
        in >> tasks;
        file.close();
        filterModel->invalidate();
    }
}

void MainWindow::exit()
{
    if (!model->data(model->index(0, 3), Qt::DisplayRole).toDateTime().isValid()){
        finishTask();
    }
}

void MainWindow::deleteTask()
{
    QModelIndex index = ui->tableView->selectionModel()->selectedIndexes().first();
    filterModel->removeRows(index.row(), 1, QModelIndex());
    save(filename);
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
    dlg.setWindowFlags(dlg.windowFlags() & ~Qt::WindowContextHelpButtonHint);
    connect(&dlg, &DateDialog::dateChanged, this, &MainWindow::setDate);
    dlg.exec();
}

void MainWindow::on_actionLoad_triggered()
{
    filename = QFileDialog::getOpenFileName(this, tr("Open tasks"), QDir::currentPath());
    load(filename);
}

void MainWindow::on_actionSave_as_triggered()
{
    QString filename = QFileDialog::getSaveFileName(this, tr("Save tasks"), QDir::currentPath());
    save(filename);
}

void MainWindow::on_actionExit_triggered()
{
    exit();
    QApplication::exit(0);
}

void MainWindow::on_actionAbout_triggered()
{
    AboutDialog dlg(this);
    dlg.setModal(true);
    dlg.setWindowFlags(dlg.windowFlags() | Qt::MSWindowsFixedSizeDialogHint);
    dlg.setWindowFlags(dlg.windowFlags() & ~Qt::WindowContextHelpButtonHint);
    dlg.exec();
}

void MainWindow::on_tableView_customContextMenuRequested(const QPoint &pos)
{
    QModelIndex index = ui->tableView->indexAt(pos);
    if (index.isValid() && filterModel->data(filterModel->index(index.row(), 3), Qt::DisplayRole).toDateTime().isValid()){
        contextMenu->popup(ui->tableView->viewport()->mapToGlobal(pos));
    }
}
