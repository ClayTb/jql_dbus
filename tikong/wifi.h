#ifndef WIFIH
#define WIFIH

#define TKIP "192.168.1.150"
//某一个tikong的ssid
#define SSID "tikong"
#define CON_NAME "tikong"
#include <gio/gio.h>
#include <uuid/uuid.h>
#include <string.h>
#include <nm-dbus-interface.h>
#include <NetworkManager.h>
#include <stdlib.h>
#include <stdio.h>



gboolean check_exist (const char *ssid);
int add_wifi_connection(const char *con_name);
gboolean disconnect_wifi(char *err);
gboolean check_connectivity(const char *iface, char * err);
gboolean remove_wifi_connection(const char *ssid);
gboolean find_hw();
void connect_wifi(const char *iface, const char *ssid, const char *pw, char *ret);


#endif