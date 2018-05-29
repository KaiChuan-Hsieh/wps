#ifndef WPAS_UTILS_H
#define WPAS_UTILS_H

#include <QDebug>

int wpa_command_resp(const char *ifname, const char *cmd, char *resp, size_t resp_size);

#endif // WPAS_UTILS_H
