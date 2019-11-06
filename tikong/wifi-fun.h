#ifndef WIFIFUNH
#define WIFIFUNH
#include <gio/gio.h>
#include <uuid/uuid.h>
#include <string.h>
#include <nm-dbus-interface.h>
#include <NetworkManager.h>
#include <stdlib.h>
#include <stdio.h>


//char   ret[100]=""; 
//#define IFACE "wlp3s0"

extern char WIFIDEVICE[];

#ifdef __cplusplus
extern "C"
{
	#endif
char *
nm_utils_uuid_generate (void);

 gboolean
//add_connection (GDBusProxy *proxy, const char *con_name);
add_connection (const char *iface, const char *con_name, const char *pw, char *err);



 gboolean
get_active_connection_details (const char *obj_path, const char *ssid);

 gboolean
find_tk_wifi (GDBusProxy *proxy, const char *ssid);

 gboolean
remove(const char *obj_path);

 gboolean
find_tk_conn(GDBusProxy *proxy);

 gboolean
enable_conn(char *err);

 gboolean 
is_wifi(const char *obj_path);

gboolean
find_hw_fun(GDBusProxy *proxy);

gboolean exec(const char* cmd, char *ret);
//gboolean exec(const char* cmd, char *ret);
#ifdef __cplusplus
}
#endif

#endif