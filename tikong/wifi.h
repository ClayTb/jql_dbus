
#include "wifi-fun.h"

#define TKIP "192.168.1.150"
//某一个tikong的ssid
#define SSID "tikong"
#define CON_NAME "tikong"
#include <sys/time.h>



 gboolean check_exist (const char *ssid);
int add_wifi_connection(const char *con_name);
 //gboolean connect_wifi();
 gboolean disconnect_wifi(char *err);
 gboolean check_connectivity(char * err);
gboolean remove_wifi_connection(const char *ssid);
gboolean find_hw();
void connect_wifi(const char *ssid, char *ret);
