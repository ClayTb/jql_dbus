
#include "wifi-fun.h"

#define TKIP "192.168.1.150"
#define SSID "shanzhu_5G"
#define CON_NAME "tikong_conn"


 gboolean check_exist (const char *ssid);
int add_wifi_connection(const char *con_name);
 gboolean connect_wifi();
 gboolean disconn_wifi(const char *device_path);
 gboolean check_connectivity();
