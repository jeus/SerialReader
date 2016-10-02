/****************************************************************************
** *********************
** JEUS
** alkhandani@gmail.com
** *********************
****************************************************************************/


#include "dialog.h"

#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>
#include <QPushButton>
#include <QGridLayout>
#include <QtSerialPort/QSerialPortInfo>
#include <QtSql>
#include <QSqlDatabase>
#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDialog>

QT_USE_NAMESPACE

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , transactionCount(0)
    , serialPortLabel(new QLabel(tr("Serial port:")))
    , serialPortComboBox(new QComboBox())
    , waitRequestLabel(new QLabel(tr("Wait request, msec:")))
    , waitRequestSpinBox(new QSpinBox())
    , responseLabel(new QLabel(tr("Response:")))
    , hostLabel(new QLabel(tr("Host:")))
    , dataBaseLabel(new QLabel(tr("Database:")))
    , userLabel(new QLabel(tr("User:")))
    , passLabel(new QLabel(tr("Password:")))
    , responseLineEdit(new QLineEdit(tr("OK")))
    , hostLineEdit(new QLineEdit(tr("localhost")))
    , databaseLineEdit(new QLineEdit(tr("radon")))
    , userLineEdit(new QLineEdit(tr("root")))
    , passwordLineEdit(new QLineEdit(tr("6111191111")))
    , trafficLabel(new QLabel(tr("No traffic.")))
    , statusLabel(new QLabel(tr("Status: Not running.")))
    , runButton(new QPushButton(tr("Start")))
{
    passwordLineEdit->setEchoMode(QLineEdit::Password);
    waitRequestSpinBox->setRange(0, 10000);
    waitRequestSpinBox->setValue(10000);

    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
        serialPortComboBox->addItem(info.portName());

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(serialPortLabel, 0, 0);
    mainLayout->addWidget(serialPortComboBox, 0, 1);
    mainLayout->addWidget(waitRequestLabel, 1, 0);
    mainLayout->addWidget(waitRequestSpinBox, 1, 1);
    mainLayout->addWidget(runButton, 0, 2, 2, 1);
    mainLayout->addWidget(responseLabel, 2, 0);
    mainLayout->addWidget(responseLineEdit, 2, 1, 1, 3);
    mainLayout->addWidget(hostLabel,3,0);
    mainLayout->addWidget(hostLineEdit,3,1,1,3);
    mainLayout->addWidget(dataBaseLabel,4,0);
    mainLayout->addWidget(databaseLineEdit,4,1,1,3);
    mainLayout->addWidget(userLabel,5,0);
    mainLayout->addWidget(userLineEdit,5,1,1,3);
    mainLayout->addWidget(passLabel,6,0);
    mainLayout->addWidget(passwordLineEdit,6,1,1,3);
    mainLayout->addWidget(trafficLabel, 8, 0, 1, 4);
    mainLayout->addWidget(statusLabel, 9, 0, 1, 5);
    setLayout(mainLayout);

    setWindowTitle(tr("Blocking Slave"));
    serialPortComboBox->setFocus();

    connect(runButton, SIGNAL(clicked()),this, SLOT(startSlave()));
    connect(&thread, SIGNAL(request(QString)),this, SLOT(showRequest(QString)));
    connect(&thread, SIGNAL(error(QString)),this, SLOT(processError(QString)));
    connect(&thread, SIGNAL(timeout(QString)),this, SLOT(processTimeout(QString)));

    connect(serialPortComboBox, SIGNAL(currentIndexChanged(QString)),this, SLOT(activateRunButton()));
    connect(waitRequestSpinBox, SIGNAL(valueChanged(int)),this, SLOT(activateRunButton()));
    connect(responseLineEdit, SIGNAL(textChanged(QString)),this, SLOT(activateRunButton()));
    startSlave();
}


void Dialog::startSlave()
{
    runButton->setEnabled(false);
    statusLabel->setText(tr("Status: Running, connected to port %1.").arg(serialPortComboBox->currentText()));
    thread.startSlave(serialPortComboBox->currentText(),waitRequestSpinBox->value(),responseLineEdit->text());
    //thread dige bayad ezafe shavad.SQL
}

void Dialog::showRequest(const QString &s)
{
    trafficLabel->setText(tr("Traffic, transaction #%1:" "\n\r-request: %2" "\n\r-response: %3")
                          .arg(++transactionCount).arg(s).arg(responseLineEdit->text()));
    qInfo() << tr("%1").arg(s);
    insertDatabase(s);
}

void Dialog::processError(const QString &s)
{
    activateRunButton();
    statusLabel->setText(tr("Status: Not running, %1.").arg(s));
    trafficLabel->setText(tr("No traffic."));
}

void Dialog::insertDatabase(const QString &json)
{
    /**
     * Sample Format
     * "{\"Radon\":[{\"radon\": 15445,\"temp\": 73,\"hum\": 47,\"pres\": 99}]}";
     */

    QJsonDocument jsonResponse = QJsonDocument::fromJson(json.toUtf8());
    QJsonObject jsonObject = jsonResponse.object();
    QJsonArray jsonArray = jsonObject["Radon"].toArray();
    QString sql = "INSERT INTO `radon`.`logs` (`radon`,`hum`,`temp`,`pres`) VALUES (%1,%2,%3,%4);";
    foreach (const QJsonValue & value, jsonArray) {
        QJsonObject obj = value.toObject();
        sql = QString(sql).arg(obj["radon"].toInt()).arg(obj["hum"].toDouble()).arg(obj["temp"].toDouble()).arg(obj["pres"].toInt());

    }
         QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
         db.setHostName(hostLineEdit->text());
         db.setDatabaseName(databaseLineEdit->text());
         db.setUserName(userLineEdit->text());
         db.setPassword(passwordLineEdit->text());

         db.open();
         QSqlQuery query;
         query.exec(sql);
         db.close();
}

void Dialog::processTimeout(const QString &s)
{
    statusLabel->setText(tr("Status: Running, %1.").arg(s));
    trafficLabel->setText(tr("No traffic."));
}
void Dialog::activateRunButton()
{
    runButton->setEnabled(true);
}
