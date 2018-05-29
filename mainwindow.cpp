#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "wpas_utils.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->pinButton->setText("WPS PIN");
    ui->pushButton->setText("WPS Push");
    ui->pinText->setText("PIN:");
    ui->statusMessage->setReadOnly(true);
    connect(ui->pinButton, SIGNAL(released()), this, SLOT(handlePinButton()));
    connect(ui->pushButton, SIGNAL(released()), this, SLOT(handlePushButton()));

    monThread = new MonitorThread(this);
    connect(monThread, SIGNAL(updateMessage(QString)), this, SLOT(handleMessage(QString)));
}

void MainWindow::handlePinButton()
{
    QString pin = ui->pinTextEdit->text();
    QString cmd = "WPS_PIN any";
    int ret;
    char resp[1024];
    QByteArray ba;

    if (pin != "")
        cmd = cmd + " " + pin;

    ba = cmd.toLatin1();

    ui->statusMessage->insertPlainText(cmd);
    ui->statusMessage->insertPlainText("\n");

    ret = wpa_command_resp(mInterface, ba.data(), resp, sizeof(resp));
    if (ret < 0) {
       qDebug() << "wpa_command_resp failed" << ": " << cmd << endl;
       return;
    }

    ui->statusMessage->insertPlainText(resp);
    ui->statusMessage->insertPlainText("\n");
}

void MainWindow::handlePushButton()
{
    QString cmd = "WPS_PBC";
    int ret;
    char resp[1024];
    QByteArray ba;

    ba = cmd.toLatin1();

    ui->statusMessage->insertPlainText(cmd);
    ui->statusMessage->insertPlainText("\n");

    ret = wpa_command_resp(mInterface, ba.data(), resp, sizeof(resp));
    if (ret < 0) {
       qDebug() << "wpa_command_resp failed" << ": " << cmd << endl;
       return;
    }

    ui->statusMessage->insertPlainText(resp);
    ui->statusMessage->insertPlainText("\n");
}

void MainWindow::handleMessage(QString text)
{
    ui->statusMessage->insertPlainText(text);
    ui->statusMessage->insertPlainText("\n");
}

void MainWindow::setInterface(const char *ifname)
{
    mInterface = ifname;
    monThread->setInterface(mInterface);
    monThread->start();
}

MainWindow::~MainWindow()
{
    monThread->terminate();
    monThread->wait();
    delete monThread;
    delete ui;
}
