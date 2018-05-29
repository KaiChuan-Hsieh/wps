#ifndef MONITORTHREAD_H
#define MONITORTHREAD_H
#include <QThread>
#include <QDebug>

class MonitorThread : public QThread
{
    Q_OBJECT

public:
    explicit MonitorThread(QObject *parent = 0);
    ~MonitorThread();
    void setInterface(const char *ifname);
signals:
    void updateMessage(QString);
protected:
    void run();
private:
    const char *mInterface;
};

#endif // MONITORTHREAD_H
