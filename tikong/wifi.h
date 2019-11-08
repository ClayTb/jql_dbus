#ifndef WIFIH
#define WIFIH


int get_version(char *ver);
int connect_wifi(const char *iface, const char *ssid, const char *pw, char *ret);
int disconnect_wifi(const char *iface, char *err);
int check_connectivity(const char *iface, char * err);


#endif