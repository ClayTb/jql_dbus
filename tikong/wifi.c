/*
author: matt ji
date: 2019-8-29
brief: 关于wifi控制流程的函数接口
*/
/*1. 检查所有连接，看有没有特定连接(tikong-wifi)的名字 check_exist*/

#include "wifi.h"
 gboolean
check_exist (const char *ssid)
{
	GDBusProxy *proxy;
    gboolean found = FALSE;

	/* Create a D-Bus proxy; NM_DBUS_* defined in nm-dbus-interface.h */
	proxy = g_dbus_proxy_new_for_bus_sync (G_BUS_TYPE_SYSTEM,
	                                       G_DBUS_PROXY_FLAGS_NONE,
	                                       NULL,
	                                       NM_DBUS_SERVICE,
	                                       NM_DBUS_PATH_SETTINGS,
	                                       NM_DBUS_INTERFACE_SETTINGS,
	                                       NULL, NULL);
	g_assert (proxy != NULL);

	/* List connections of system settings service */
    //check if exist    
	found = find_tk_wifi (proxy, ssid);
    if(found == TRUE)
    {
        g_print("ok, find it\n");
        found = TRUE;
    }
    else
    {
        //创建连接
        g_print("well, not found, create it then\n");
        found = FALSE;
        
    }
    
	g_object_unref (proxy);

	return found;
}

/*2. 创建连接函数 add_wifi_connection*/
int add_wifi_connection(const char *con_name)
{
    GDBusProxy *proxy;
	GError *error = NULL;

	/* Create a D-Bus proxy; NM_DBUS_* defined in nm-dbus-interface.h */
	proxy = g_dbus_proxy_new_for_bus_sync (G_BUS_TYPE_SYSTEM,
	                                       G_DBUS_PROXY_FLAGS_NONE,
	                                       NULL,
	                                       NM_DBUS_SERVICE,
                                           //"/org/freedesktop/NetworkManager/Settings"
	                                       NM_DBUS_PATH_SETTINGS,
                                           // "org.freedesktop.NetworkManager.Settings"
	                                       NM_DBUS_INTERFACE_SETTINGS,
	                                       NULL, &error);
	if (!proxy) {
		g_dbus_error_strip_remote_error (error);
		g_print ("Could not create NetworkManager D-Bus proxy: %s\n", error->message);
		g_error_free (error);
		return 1;
	}

	/* Add a connection */
	add_connection (proxy, con_name);

	g_object_unref (proxy);

	return 0;
}

/*3. 删除连接 remove_connection()*/
gboolean
remove_wifi_connection(const char *ssid)
{
    GDBusProxy *proxy;
    gboolean found = FALSE;

	/* Create a D-Bus proxy; NM_DBUS_* defined in nm-dbus-interface.h */
	proxy = g_dbus_proxy_new_for_bus_sync (G_BUS_TYPE_SYSTEM,
	                                       G_DBUS_PROXY_FLAGS_NONE,
	                                       NULL,
	                                       NM_DBUS_SERVICE,
	                                       NM_DBUS_PATH_SETTINGS,
	                                       NM_DBUS_INTERFACE_SETTINGS,
	                                       NULL, NULL);
	g_assert (proxy != NULL);
    
    //remove_tk_wifi (proxy);
    int i;
	GError *error = NULL;
	GVariant *ret;
	char **paths;

	/* Call ListConnections D-Bus method */
	ret = g_dbus_proxy_call_sync (proxy,
	                              "ListConnections",
	                              NULL,
	                              G_DBUS_CALL_FLAGS_NONE, -1,
	                              NULL, &error);
	if (!ret) {
		g_dbus_error_strip_remote_error (error);
		g_print ("ListConnections failed: %s\n", error->message);
		g_error_free (error);
		return FALSE;
	}

	g_variant_get (ret, "(^ao)", &paths);
	g_variant_unref (ret);

	for (i = 0; paths[i]; i++)
    {        
        found = get_active_connection_details (paths[i], ssid);
        if(found == TRUE)
        {
            remove(paths[i]);
        }       
        
    }
	g_strfreev (paths);
    
	g_object_unref (proxy);

	return found;
}

/*4. 连接wifi函数 connect_wifi()*/
 gboolean
connect_wifi()
{
    GDBusProxy *proxy;
    gboolean found = FALSE;

	/* Create a D-Bus proxy; NM_DBUS_* defined in nm-dbus-interface.h */
	proxy = g_dbus_proxy_new_for_bus_sync (G_BUS_TYPE_SYSTEM,
	                                       G_DBUS_PROXY_FLAGS_NONE,
	                                       NULL,
                                           // "org.freedesktop.NetworkManager"
	                                       NM_DBUS_SERVICE,
                                           //"/org/freedesktop/NetworkManager"
	                                       NM_DBUS_PATH,
                                           //"org.freedesktop.NetworkManager"
	                                       //NM_DBUS_INTERFACE,
	                                       "org.freedesktop.DBus.Properties",
	                                       NULL, NULL);
	g_assert (proxy != NULL);
    
	find_tk_conn(proxy);
    
	g_object_unref (proxy);

    return TRUE;
}

/*5. 断开wifi函数 disconn_wifi()*/
 gboolean
disconn_wifi(const char *device_path)
{

	GDBusProxy *proxy;
    //gboolean found = FALSE;
    GError *error = NULL;
    g_print("first diconn wifi\n");

	/* Create a D-Bus proxy; NM_DBUS_* defined in nm-dbus-interface.h */
	proxy = g_dbus_proxy_new_for_bus_sync (G_BUS_TYPE_SYSTEM,
	                                       G_DBUS_PROXY_FLAGS_NONE,
	                                       NULL,
                                           // "org.freedesktop.NetworkManager"
	                                       NM_DBUS_SERVICE,
	                                       device_path,                                          
	                                       "org.freedesktop.NetworkManager.Device",
	                                       NULL, NULL);
	g_assert (proxy != NULL);
	GVariant *ret = NULL;
    
	ret = g_dbus_proxy_call_sync (proxy,
                              "Disconnect",
                              NULL,
                              G_DBUS_CALL_FLAGS_NONE, -1,
                              NULL, &error);
    //const char *value = NULL; 
    //GVariant *value;
	//g_variant_get (ret, "(o)", &value);
	//g_print("return current %s\n", value);

	g_object_unref (proxy);
	//g_object_unref (ret);

    return TRUE;
}

/*6. 测试网络连通性函数 check_connectivity()*/
 gboolean
check_connectivity()
{
	int ret = -1;
	ret = system("ping 192.168.1.1 -c 1");
	if(ret == 0)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
} 



