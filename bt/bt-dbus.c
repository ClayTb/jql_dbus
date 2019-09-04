
#include "bt-dbus.h"


GDBusConnection *conn;
static const gchar* bluez_adapter_get_property(const gchar *path, const char *name)
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
	return g_variant_get_string(prop, NULL);
}

int main(void)
{
	int rc = 0;
	GDBusProxy *proxy;
	GError *error = NULL;
	GVariant *result;
	const gchar *object_path;
	GVariant *ifaces_and_properties;
	GVariantIter i;

	conn = g_bus_get_sync(G_BUS_TYPE_SYSTEM, NULL, &error);
	if(error != NULL)
		return 1;

	proxy = g_dbus_proxy_new_sync(conn,
				      G_DBUS_PROXY_FLAGS_NONE,
				      NULL,
                      //蓝牙程序里这个参数应该全都是这个
				      "org.bluez",
                      //这里会有不一样
				      "/",
				      "org.freedesktop.DBus.ObjectManager",
				      NULL,
				      &error);
	if(error != NULL)
		return 1;

	error = NULL;
	result = g_dbus_proxy_call_sync(proxy,
    //这里是去得到所有蓝牙设备的属性
					"GetManagedObjects",
					NULL,
					G_DBUS_CALL_FLAGS_NONE,
					-1,
					NULL,
					&error);
	if(error != NULL)
		return 1;

	/* Parse the result */
    /*在wifi的程序里直接使用的是g_variant_get(device_value, "ao", &iter);
	const char *path;
	//然後從數組裏拿
	while(g_variant_iter_loop(iter, "o", &path))
    这里不行，这里返回的就是dict of{object path, dict of{string, dict{string, variant}}}    */
	if(result) {
		result = g_variant_get_child_value(result, 0);
		g_variant_iter_init(&i, result);
		while(g_variant_iter_next(&i, "{&o@a{sa{sv}}}", &object_path, &ifaces_and_properties)) {
			const gchar *interface_name;
			GVariant *properties;
			GVariantIter ii;
			g_variant_iter_init(&ii, ifaces_and_properties);
			while(g_variant_iter_next(&ii, "{&s@a{sv}}", &interface_name, &properties)) {
                //g_strstr_len 查找字符串，g_ascii_strdown 将大写转换成小写
				if(g_strstr_len(g_ascii_strdown(interface_name, -1), -1, "adapter"))
                    //name 返回 raspberrypi address返回B8:27:EB:9F:30:A9
					g_print("HCI Name: %s Address: %s\n", bluez_adapter_get_property(object_path, "Name"), bluez_adapter_get_property(object_path, "Address"));
				g_variant_unref(properties);
			}
			g_variant_unref(ifaces_and_properties);
		}
		g_variant_unref(result);
	}

fail:
	rc = 1;
done:
	if(proxy)
		g_object_unref(proxy);
	if(conn)
		g_object_unref(conn);
	if(error)
		g_error_free(error);

	return rc;
}