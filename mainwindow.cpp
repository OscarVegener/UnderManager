#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // construct objects
    taskTimer = new QTimer(this);
    taskTimer->setInterval(1000);
    connect(taskTimer, &QTimer::timeout, this, &MainWindow::updateTimer);

    model = new TaskModel(&tasks, this);
    filterModel = new TaskFilterModel(this);
    filterModel->setSourceModel(model);
    setDate(QDate::currentDate());

    settings = new QSettings(QSettings::IniFormat,
                             QSettings::UserScope,
                             QCoreApplication::organizationName(),
                             QCoreApplication::applicationName(),
                             this);

    //set up another thread to calculate total time elapsed today
    worker = new UpdateTodayTimeWorker(&tasks);
    worker->moveToThread(&workerThread);
    connect(&workerThread, &QThread::finished, worker, &QObject::deleteLater);
    connect(this, &MainWindow::updateTodayTimeElapsedNeeded, worker, &UpdateTodayTimeWorker::calculateTodayTime);
    connect(worker, &UpdateTodayTimeWorker::timeCalculated, this, &MainWindow::setTodayTimeElapsed);
    workerThread.start();

    //set up ui
    QTime time(0, 0);
    ui->timeEdit->setTime(time);
    ui->tableView->setModel(filterModel);
    ui->stopButton->setHidden(true);
    ui->label->setHidden(true);
    ui->lineEdit->setHidden(true);
    ui->lineEdit->setText("0");
    ui->tableView->horizontalHeader()->setSectionResizeMode(5, QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->tableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->tableView->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);

    initContextMenu();

    clipBoard = QGuiApplication::clipboard();

    readSettings();

    load(filename);
}

MainWindow::~MainWindow()
{
    workerThread.quit();
    workerThread.wait();
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
    connect(deleteAction, &QAction::triggered, this, &MainWindow::deleteItem);
    contextMenu->addAction(deleteAction);
    copyAction = new QAction(tr("&Copy"), contextMenu);
    connect(copyAction, &QAction::triggered, this, &MainWindow::copyItem);
    contextMenu->addAction(copyAction);
    newTaskAction = new QAction(tr("&New task..."), contextMenu);
    connect(newTaskAction, &QAction::triggered, this, &MainWindow::newItem);
    contextMenu->addAction(newTaskAction);
}

void MainWindow::writeSettings()
{
    settings->beginGroup("MainWindow");
    settings->setValue("size", size());
    settings->setValue("pos", pos());
    if (windowState().testFlag(Qt::WindowMaximized)){
        settings->setValue("fullscreenState", true);
    }
    else{
        settings->setValue("fullscreenState", false);
    }
    settings->endGroup();
    settings->beginGroup("General");
    settings->setValue("timeElapsed", ui->timeEdit->time());
    settings->setValue("daysElapsed", ui->lineEdit->text());
    settings->endGroup();
}

void MainWindow::writeDefaultSettings()
{
    settings->beginGroup("MainWindow");
    settings->setValue("size", size());
    settings->setValue("pos", pos());
    if (windowState().testFlag(Qt::WindowMaximized)){
        settings->setValue("fullscreenState", true);
    }
    else{
        settings->setValue("fullscreenState", false);
    }
    settings->endGroup();
    settings->beginGroup("General");
    settings->setValue("timeElapsed", ui->timeEdit->time());
    settings->setValue("daysElapsed", ui->lineEdit->text());
    settings->setValue("autoFinishTask", true);
    settings->endGroup();
}

void MainWindow::readSettings()
{
    if (QFile::exists(settings->fileName())){
        settings->beginGroup("MainWindow");
        resize(settings->value("size", QSize(400, 400)).toSize());
        move(settings->value("pos", QPoint(200, 200)).toPoint());
        if (settings->value("fullscreenState").toBool()){
            setWindowState(windowState().setFlag(Qt::WindowMaximized, true));
        }
        settings->endGroup();
        settings->beginGroup("General");
        settings->endGroup();
    }
    else{
        writeDefaultSettings();
    }
}

void MainWindow::addNewTask()
{
    tasks.push_front(Task());
    tasks[0].setCaption("Unnamed task");
    tasks[0].setStatus("Unknown status");
    tasks[0].setDateStarted(QDateTime::currentDateTime());
    filterModel->invalidate();
}

void MainWindow::finishTask()
{
    tasks[0].setDateFinished(QDateTime::currentDateTime());
    tasks[0].setTimeElapsed(ui->timeEdit->time());
    tasks[0].setDaysElapsed(ui->lineEdit->text().toULongLong());
    filterModel->invalidate();
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
        QMutexLocker locker(&tasksMutex);
        QDataStream in(&file);
        in >> tasks;
        file.close();
        filterModel->invalidate();
        if (tasks.size() != 0 && !tasks.at(0).isFinished()){
            loadUnfinishedTask();
        }
        emit updateTodayTimeElapsedNeeded();
    }   
}

void MainWindow::loadUnfinishedTask()
{
    if (settings->contains("General/timeElapsed") && settings->contains("General/daysElapsed")){
        ui->startButton->setHidden(true);
        ui->stopButton->setHidden(false);
        ui->timeEdit->setTime(settings->value("General/timeElapsed").toTime());
        if (quint64 days = settings->value("General/daysElapsed").toULongLong() > 0){
            ui->lineEdit->setHidden(false);
            ui->lineEdit->setText(QString::number(days));
        }
        taskTimer->start();
    }
    else{
        QMessageBox *msg = new QMessageBox(this);
        msg->setModal(true);
        msg->setIcon(QMessageBox::Warning);
        msg->setText("Can't load unfinished task. Last task has been deleted.");
        msg->setStandardButtons(QMessageBox::Ok);
        msg->exec();
        delete msg;
        deleteUnfinishedTask();
    }
}

void MainWindow::deleteUnfinishedTask()
{
    tasks.removeAt(0);
    filterModel->invalidate();
}

void MainWindow::exit()
{
    if (settings->value("General/autoFinishTask").toBool()){
        if (!filterModel->data(filterModel->index(0, 3), Qt::DisplayRole).toDateTime().isValid()){
            finishTask();
        }
    }
    if (QFile::exists(settings->fileName())){
        writeSettings();
    }
    else{
        writeDefaultSettings();
    }
    save(filename);
}

void MainWindow::deleteItem()
{
    QModelIndex index = ui->tableView->selectionModel()->selectedIndexes().first();
    filterModel->removeRows(index.row(), 1, QModelIndex());
    save(filename);
    emit updateTodayTimeElapsedNeeded();
}

void MainWindow::copyItem()
{
    QModelIndex index = ui->tableView->selectionModel()->selectedIndexes().first();
    QString str = filterModel->data(index, Qt::DisplayRole).toString();
    clipBoard->setText(str);
}

void MainWindow::newItem()
{
    NewTaskDialog dlg(this);
    dlg.setModal(true);
    dlg.setWindowFlags(dlg.windowFlags() & ~Qt::WindowContextHelpButtonHint);
    dlg.setWindowFlags(dlg.windowFlags() | Qt::MSWindowsFixedSizeDialogHint);
    connect(&dlg, &NewTaskDialog::taskCreated, model, &TaskModel::addTask);
    dlg.exec();
    emit updateTodayTimeElapsedNeeded();
}

void MainWindow::updateTimer()
{
    ui->timeEdit->setTime(ui->timeEdit->time().addMSecs(1000));
    QDateTime dateStarted = filterModel->data(filterModel->index(0, 2), Qt::DisplayRole).toDateTime();
    quint64 days = dateStarted.daysTo(QDateTime::currentDateTime());
    if (days > 0){
        ui->label->setHidden(false);
        ui->lineEdit->setHidden(false);
        ui->lineEdit->setText(QString::number(days));
    }
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
    ui->label->setHidden(true);
    ui->lineEdit->setHidden(true);
    ui->lineEdit->setText("0");
    emit updateTodayTimeElapsedNeeded();
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
    QString filename = QFileDialog::getOpenFileName(this, tr("Open tasks"), QDir::currentPath());
    if(!filename.isNull()){
        this->filename = filename;
        load(filename);
    }
}

void MainWindow::on_actionSave_as_triggered()
{
    QString filename = QFileDialog::getSaveFileName(this, tr("Save tasks"), QDir::currentPath());
    if(!filename.isNull()){
        save(filename);
    }
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
        deleteAction->setEnabled(true);
    }
    else{
        deleteAction->setEnabled(false);
    }
    if (index.isValid() && !filterModel->data(index, Qt::DisplayRole).isNull()){
        copyAction->setEnabled(true);
    }
    else{
        copyAction->setEnabled(false);
    }
    contextMenu->popup(ui->tableView->viewport()->mapToGlobal(pos));
}

void MainWindow::on_actionPreferences_triggered()
{
    Preferences dlg(settings, this);
    dlg.setModal(true);
    dlg.setWindowFlags(dlg.windowFlags() | Qt::MSWindowsFixedSizeDialogHint);
    dlg.setWindowFlags(dlg.windowFlags() & ~Qt::WindowContextHelpButtonHint);
    dlg.exec();
}

void MainWindow::setTodayTimeElapsed(const QTime &time)
{
    ui->timeEditToday->setTime(time);
}
