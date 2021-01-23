#include "datedialog.h"
#include "ui_datedialog.h"

DateDialog::DateDialog(const QDate &selectedDate, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DateDialog)
{
    ui->setupUi(this);
    ui->calendarWidget->setSelectedDate(selectedDate);
    ui->calendarWidget->setMaximumDate(QDate::currentDate());
    ui->calendarWidget->setMinimumDate(QDate(1980, 1, 1));
}

DateDialog::~DateDialog()
{
    delete ui;
}

void DateDialog::on_buttonBox_accepted()
{
    emit dateChanged(ui->calendarWidget->selectedDate());
}
