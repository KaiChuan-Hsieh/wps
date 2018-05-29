#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include "monitorthread.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void setInterface(const char *ifname);

private slots:
    void handlePinButton();
    void handlePushButton();
    void handleMessage(QString);
private:
    Ui::MainWindow *ui;
    MonitorThread *monThread;
    const char *mInterface;
};

#endif // MAINWINDOW_H
