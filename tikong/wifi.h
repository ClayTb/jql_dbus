#ifndef WIFIH
#define WIFIH


int get_version(char *ver);
int connect_wifi(const char *iface, const char *ssid, const char *pw, char *ret);
//如果当前iface有活动的wifi，断掉，
int disconnect_wifi(const char *iface, char *ret);
int check_connectivity(const char *iface, char *ip, char * ret);
//删除这个ssid上所有的连接
int remove_conn(const char *ssid, char *ret);
//int disable_auto(const char *iface, char *ret);
//需要更新的连接的路径 属性名 属性的值
int update_property(const char *path, const char* property, const char* value, char * ret);
//列出这个ssid上所有的连接的路径，ret默认500字节
//如果没有就返回no path，如果有很多，字符串放不下
//就返回need 1000 bytes这样就可以继续来调用
//正确就返回0，没有路径或者缓冲区太少就返回1
int list_connections(const char *ssid, char *ret);

#endif