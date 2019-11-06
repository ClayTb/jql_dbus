#include <gio/gio.h>
#include <uuid/uuid.h>
#include <string.h>
#include <nm-dbus-interface.h>
#include <NetworkManager.h>
#include <stdlib.h>

#define TKIP "192.168.1.150"
#define SSID "shanzhu_5G"

#include <iostream>
using namespace std;

enum STATE
{
    DISCONN = 0;
    CONNCTING;
    CONNED;
}
class Wifi{
    STATE conn;
public:
    Wifi(STATE);
    Wifi();
    gboolean check_exist (const char *ssid);
    gboolean add_wifi_connection();
    gboolean connect_wifi();
    gboolean remove_connection();
};

Wifi::Wifi(STATE c)
{
    conn = c;
}
Wifi::Wifi()
{
    conn = DISCONN;
}

