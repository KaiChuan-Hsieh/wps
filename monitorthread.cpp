#include "monitorthread.h"
#include "wpa_ctrl.h"

#define CTRL_IFACE_DIR "/var/run/wpa_supplicant"

MonitorThread::MonitorThread(QObject *parent)
    : QThread(parent)
{
    qDebug() << "Monitor Thread created" << endl;
}

void MonitorThread::setInterface(const char *ifname)
{
    mInterface = ifname;
}

void MonitorThread::run()
{
    char buf[4096];
    fd_set rfd;
    size_t buf_size = sizeof(buf);
    struct wpa_ctrl *mon;
    int fd, ret;
    char *pos;

    qDebug("Monitor Thread starts on interface %s.", mInterface);

    sprintf(buf, "%s/%s", CTRL_IFACE_DIR, mInterface);

    mon = wpa_ctrl_open(buf);
    if (mon == NULL) {
        qDebug("wpa_ctrl_open(%s) failed", buf);
        goto out;
    }

    if (wpa_ctrl_attach(mon) < 0) {
        qDebug() << "wpa_ctrl_attach failed" << endl;
        goto close1;
    }

    fd = wpa_ctrl_get_fd(mon);
    if (fd < 0) {
        qDebug() << "wpa_ctrl_get_fd failed" << endl;
        goto close2;
    }

    qDebug() << "Start revceiving event" << endl;

    while (1) {
        size_t len;

        FD_ZERO(&rfd);
        FD_SET(fd, &rfd);

        ret =  select(fd + 1, &rfd, NULL, NULL, NULL);
        if (ret < 0) {
            qDebug()<< "select: %s" << strerror(errno) << endl;
            goto close2;
        }

        len = buf_size;
        if (wpa_ctrl_recv(mon, buf, &len) < 0) {
            qDebug() << "wpa_ctrl_recv failed" << endl;
            goto close2;
        }

        if (len == buf_size)
            len--;
        buf[len] = '\0';

        if (strstr(buf, "<")) {
            pos = strstr(buf, ">");
            pos += 1;
        }

        qDebug() << "Event: " << pos << endl;

        if (!strstr(pos, WPA_EVENT_BSS_ADDED) &&
                !strstr(pos, WPA_EVENT_BSS_REMOVED)) {
            qDebug() << "Event: " << pos << endl;
            emit updateMessage(pos);
        }

        if (strstr(pos, WPA_EVENT_TERMINATING)) {
            qDebug() << "Terminating thread" << endl;
            break;
        }
    }

close2:
    wpa_ctrl_detach(mon);
close1:
    wpa_ctrl_close(mon);
out:
    qDebug() << "Monitor loop stop" << endl;
}

MonitorThread::~MonitorThread()
{
    qDebug() << "Monitor Thread deleted" << endl;
}
