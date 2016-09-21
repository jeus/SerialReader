/****************************************************************************
** *********************
** JEUS
** alkhandani@gmail.com
** *********************
****************************************************************************/

#ifndef DIALOG_H
#define DIALOG_H
#include <QtSql/QtSql>
#include <QDialog>

#include "slavethread.h"

QT_BEGIN_NAMESPACE

class QLabel;
class QLineEdit;
class QComboBox;
class QSpinBox;
class QPushButton;

QT_END_NAMESPACE

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = 0);

private slots:
    void startSlave();
    void showRequest(const QString &s);
    void processError(const QString &s);
    void processTimeout(const QString &s);
    void activateRunButton();
    void insertDatabase(const QString &json);

private:
    int transactionCount;
    QLabel *serialPortLabel;
    QComboBox *serialPortComboBox;
    QLabel *waitRequestLabel;
    QSpinBox *waitRequestSpinBox;
    QLabel *responseLabel;
    QLabel *hostLabel;
    QLabel *dataBaseLabel;
    QLabel *userLabel;
    QLabel *passLabel;
    QLineEdit *responseLineEdit;
    QLineEdit *hostLineEdit;
    QLineEdit *databaseLineEdit;
    QLineEdit *userLineEdit;
    QLineEdit *passwordLineEdit;
    QLabel *trafficLabel;
    QLabel *statusLabel;
    QPushButton *runButton;

    SlaveThread thread;
};

#endif // DIALOG_H
