#include <gio/gio.h>
#include <uuid/uuid.h>
#include <string.h>
//调试wifi的时候用到下面两个
//#include <nm-dbus-interface.h>
//#include <NetworkManager.h>
#include <stdlib.h>
#include <stdio.h>

#include <glib.h>
#include <gio/gio.h>


int bluez_adapter_set_property(const char *prop, GVariant *value);
extern GDBusConnection *con;
GVariant* bluez_adapter_get_property(const gchar *path, const char *name);

