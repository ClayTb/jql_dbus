#ifndef WIFIH
#define WIFIH


int get_version(char *ver);
int connect_wifi(const char *iface, const char *ssid, const char *pw, char *ret);
//如果当前iface有活动的wifi，断掉，
int disconnect_wifi(const char *iface, char *ret);
int check_connectivity(const char *iface, char *ip, char * ret);
int remove_conn(const char *ssid, char *ret);
int disable_auto(const char *iface, char *ret);


#endif