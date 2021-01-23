#ifndef DATEDIALOG_H
#define DATEDIALOG_H

#include <QDialog>

namespace Ui {
class DateDialog;
}

class DateDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DateDialog(const QDate &selectedDate, QWidget *parent = nullptr);
    ~DateDialog();

private:
    Ui::DateDialog *ui;

signals:
    void dateChanged(const QDate& date);
private slots:
    void on_buttonBox_accepted();
};

#endif // DATEDIALOG_H
