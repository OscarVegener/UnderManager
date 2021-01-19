#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <QDialog>
#include <QMap>

#include <QtDebug>
#include <QSettings>

namespace Ui {
class Preferences;
}

class Preferences : public QDialog
{
    Q_OBJECT

public:
    explicit Preferences(QSettings *settings, QWidget *parent = nullptr);
    ~Preferences();

private slots:
    void on_pushButton_clicked();

    void on_checkBox_stateChanged(int arg1);

private:
    QSettings *settings;

private:
    Ui::Preferences *ui;
};

#endif // PREFERENCES_H
