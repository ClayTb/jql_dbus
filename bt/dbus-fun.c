#include "dbus-fun.h"
GDBusConnection *conn;

GVariant* bluez_adapter_get_property(const gchar *path, const char *name)
{
	GVariant *prop;
	GDBusProxy *proxy;
	GError *error = NULL;
	GVariant *result;

	proxy = g_dbus_proxy_new_sync(conn,
				      G_DBUS_PROXY_FLAGS_NONE,
				      NULL,
				      "org.bluez",
				      path,
				      "org.freedesktop.DBus.Properties",
				      NULL,
				      &error);
	if(error != NULL)
		return NULL;

	error = NULL;
	result = g_dbus_proxy_call_sync(proxy,
					"Get",
					g_variant_new("(ss)", "org.bluez.Adapter1", name),
					G_DBUS_CALL_FLAGS_NONE,
					-1,
					NULL,
					&error);
	if(error != NULL)
		return NULL;

	g_variant_get(result, "(v)", &prop);
	g_variant_unref(result);
    //return g_variant_get_string(prop, NULL);
    return prop;

}

int bluez_adapter_set_property(const char *prop, GVariant *value)
{
	GVariant *result;
	GError *error = NULL;

	result = g_dbus_connection_call_sync(conn,
					     "org.bluez",
					     "/org/bluez/hci0",
					     "org.freedesktop.DBus.Properties",
					     "Set",
					     g_variant_new("(ssv)", "org.bluez.Adapter1", prop, value),
					     NULL,
					     G_DBUS_CALL_FLAGS_NONE,
					     -1,
					     NULL,
					     &error);
	if(error != NULL)
		return 1;

	g_variant_unref(result);
	return 0;
}

int bluez_adapter_call_method(const char *method)
{
	GVariant *result;
	GError *error = NULL;

	result = g_dbus_connection_call_sync(con,
					     "org.bluez",
					/* TODO Find the adapter path runtime */
					     "/org/bluez/hci0",
					     "org.bluez.Adapter1",
					     method,
					     NULL,
					     NULL,
					     G_DBUS_CALL_FLAGS_NONE,
					     -1,
					     NULL,
					     &error);
	if(error != NULL)
		return 1;

	g_variant_unref(result);
	return 0;
}