/****************************************************************************
** *********************
** JEUS
** alkhandani@gmail.com
** *********************
****************************************************************************/


#ifndef SLAVETHREAD_H
#define SLAVETHREAD_H
#include <QtSql/QtSql>
#include <QThread>
#include <QMutex>
#include <QWaitCondition>

//! [0]
class SlaveThread : public QThread
{
    Q_OBJECT

public:
    SlaveThread(QObject *parent = 0);
    ~SlaveThread();

    void startSlave(const QString &portName, int waitTimeout, const QString &response);
    void run();

signals:
    void request(const QString &s);
    void error(const QString &s);
    void timeout(const QString &s);

private:
    QString portName;
    QString response;
    int waitTimeout;
    QMutex mutex;
    bool quit;
};
//! [0]

#endif // SLAVETHREAD_H
