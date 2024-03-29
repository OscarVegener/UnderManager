#include "preferences.h"
#include "ui_preferences.h"

Preferences::Preferences(QSettings *settings, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Preferences)
{
    ui->setupUi(this);
    this->settings = settings;
    if(settings->value("General/autoFinishTask").toBool()){
        ui->checkBox->setCheckState(Qt::Checked);
    }
    else{
        ui->checkBox->setCheckState(Qt::Unchecked);
    }
    if(settings->value("General/closeToTray").toBool()){
        ui->checkBox_2->setCheckState(Qt::Checked);
    }
    else{
        ui->checkBox_2->setCheckState(Qt::Unchecked);
    }
}

Preferences::~Preferences()
{
    delete ui;
}

void Preferences::on_pushButton_clicked()
{
    this->close();
}

void Preferences::on_checkBox_stateChanged(int arg1)
{
    if(arg1 == Qt::Checked){
        settings->setValue("General/autoFinishTask", true);
    }
    else if(arg1 == Qt::Unchecked){
        settings->setValue("General/autoFinishTask", false);
    }
}

void Preferences::on_checkBox_2_stateChanged(int arg1)
{
    if(arg1 == Qt::Checked){
        settings->setValue("General/closeToTray", true);
    }
    else if(arg1 == Qt::Unchecked){
        settings->setValue("General/closeToTray",  false);
    }
}
