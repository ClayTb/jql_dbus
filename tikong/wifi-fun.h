#include <gio/gio.h>
#include <uuid/uuid.h>
#include <string.h>
#include <nm-dbus-interface.h>
#include <NetworkManager.h>
#include <stdlib.h>
#include <stdio.h>

extern char WIFIDEVICE[];
char *
nm_utils_uuid_generate (void);

 void
add_connection (GDBusProxy *proxy, const char *con_name);

 gboolean
get_active_connection_details (const char *obj_path, const char *ssid);

 gboolean
find_tk_wifi (GDBusProxy *proxy, const char *ssid);

 gboolean
remove(const char *obj_path);

 gboolean
find_tk_conn(GDBusProxy *proxy);

 gboolean
enable_conn(const char *device_path);

 gboolean 
is_wifi(const char *obj_path);