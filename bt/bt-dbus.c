
/*gcc `pkg-config --cflags glib-2.0 gio-2.0` -o bt bt-dbus.c `pkg-config --libs glib-2.0 gio-2.0`*/
#include "dbus-fun.h"

int get_bt(char *buf)
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
    {
        strcpy(buf, error->message);
		return 1;
    }

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
    {
        strcpy(buf, error->message);
		return 2;
    }

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
    {
        strcpy(buf, error->message);
		return 3;
    }

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
                {
                    g_print("%s\n", object_path);
                    //name 返回 raspberrypi address返回B8:27:EB:9F:30:A9
                    GVariant *ret;
                    ret = bluez_adapter_get_property(object_path, "Name");
                    g_print("HCI Name: %s\n",g_variant_get_string(ret, NULL));
                    ret = bluez_adapter_get_property(object_path, "Address");
                    g_print("Address: %s\n",g_variant_get_string(ret, NULL));
                    ret = bluez_adapter_get_property(object_path, "Discoverable");
                    g_print("Discoverable: %s\n",(g_variant_get_boolean (ret)?"True":"False"));
                    if(g_variant_get_boolean (ret) == FALSE)
                    {
                        //g_print("not discoverable, set it\n");
                        //bluez_adapter_set_property("Discoverable", g_variant_new("b", TRUE));
                    }
                    ret = bluez_adapter_get_property(object_path, "DiscoverableTimeout");
                    g_print("DiscoverableTimeout: %d\n",g_variant_get_uint32(ret));
				}
                g_variant_unref(properties);
			}
			g_variant_unref(ifaces_and_properties);
		}
		g_variant_unref(result);
	}

//fail:
//	rc = 1;
done:
	if(proxy)
		g_object_unref(proxy);
	if(conn)
		g_object_unref(conn);
	if(error)
		g_error_free(error);

	//return rc;
    return 0;
}

int start_bt(void)
{
    //1. set discoverable
    bluez_adapter_set_property("Discoverable", g_variant_new("b", TRUE));
    //2. powered
    bluez_adapter_set_property("Powered", g_variant_new("b", TRUE));
    //3. 不需要开始查找蓝牙，而是让其他设备来发现树莓派
    
}

int get_version(char *ver)
{
    if(ver == NULL)
    {
        return 1;
    }
    if(sizeof(ver) < 5)
    {
        //strcpy(ret,"返回buf太小，需要大于500字节"); //错误1
        return 2;
    }
    strcpy(ver, "1.2");
    return 0;
}