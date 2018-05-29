#include "stddef.h"
#include "wpa_ctrl.h"
#include "string.h"
#include "wpas_utils.h"

#define CTRL_IFACE_DIR "/var/run/wpa_supplicant"

int wpa_command_resp(const char *ifname, const char *cmd, char *resp, size_t resp_size)
{
    struct wpa_ctrl *ctrl;
    char buf[128];
    size_t len;

    sprintf(buf, "%s/%s", CTRL_IFACE_DIR, ifname);

    ctrl = wpa_ctrl_open(buf);
    if (ctrl == NULL) {
        qDebug() << "wpa_command: wpa_ctrl_open(" << buf << ") failed" << endl;
        return -1;
    }
    len = resp_size;
    if (wpa_ctrl_request(ctrl, cmd, strlen(cmd), resp, &len, NULL) < 0) {
        qDebug("wpa_command: wpa_ctrl_request failed");
        wpa_ctrl_close(ctrl);
        return -1;
    }
    wpa_ctrl_close(ctrl);
    resp[len] = '\0';
    return 0;
}
