/*
author: matt ji
date: 2019-8-29
brief: 关于wifi控制流程的函数接口
test on ubuntu 16.04
*/
/*1. 检查所有连接，看有没有特定连接(tikong-wifi)的名字 check_exist*/

#include "wifi-fun.h"
#include "wifi.h"
#include <gio/gio.h>
#include <uuid/uuid.h>
#include <string.h>
#include <nm-dbus-interface.h>
#include <NetworkManager.h>
#include <stdlib.h>
#include <stdio.h>


int disable_auto(const char *iface, char *err)
{
	gboolean status = FALSE;
	status = find_ap(iface, err);

	return ((status == TRUE)?0:1);
}
/*3. 删除连接 remove_conn()*/
int
remove_conn(const char *ssid, char *err)
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
	//这里把所有的连接都会列出来
	ret = g_dbus_proxy_call_sync (proxy,
	                              "ListConnections",
	                              NULL,
	                              G_DBUS_CALL_FLAGS_NONE, -1,
	                              NULL, &error);
	if (!ret) {
		g_dbus_error_strip_remote_error (error);
		g_print ("ListConnections failed: %s\n", error->message);
		strcpy(err, "ListConnections: ");
		strcat(err, error->message);
		g_error_free (error);
		return 1;
	}

	g_variant_get (ret, "(^ao)", &paths);
	g_variant_unref (ret);

//这里删除所有这个ssid上的连接
	//int match = 0;
	gboolean status = TRUE;
	for (i = 0; paths[i]; i++)
    {        
        found = get_active_connection_details (paths[i], ssid);
        if(found == TRUE)
        {
            status = remove_fun(paths[i], err);
            if(status == FALSE)
            {
            	break;
            }
        }       
        
    }
	g_strfreev (paths);
    
	g_object_unref (proxy);

	return ((status == TRUE) ? 0: 2);
}


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
        //g_print("ok, find it\n");
        found = TRUE;
    }
    else
    {
        //创建连接
        //g_print("well, not found, create it then\n");
        found = FALSE;
        
    }
    
	g_object_unref (proxy);

	return found;
}



/*5. 断开wifi函数 disconn_wifi()*/
 int
disconnect_wifi(const char *iface, char *err)
{
    //g_print("first diconn wifi\n");
    gboolean found = find_hw(iface);
    gboolean status = FALSE;
    if(found == TRUE)
    {
    	status = disc_wifi_fun(err);
    	return ((status == TRUE) ? 0: 2);
    }
    else
    {
    	return 1;
    }
	
}
int list_connections(const char *ssid, char *err)
{
	//取得这个连接的所有信息
	GDBusProxy *proxy;
    gboolean found = FALSE;

	proxy = g_dbus_proxy_new_for_bus_sync (G_BUS_TYPE_SYSTEM,
	                                       G_DBUS_PROXY_FLAGS_NONE,
	                                       NULL,
	                                       NM_DBUS_SERVICE,
	                                       NM_DBUS_PATH_SETTINGS,
	                                       NM_DBUS_INTERFACE_SETTINGS,
	                                       NULL, NULL);
	g_assert (proxy != NULL);
    
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
		strcpy(err, "ListConnections: ");
		strcat(err, error->message);
		g_error_free (error);
		return 1;
	}

	g_variant_get (ret, "(^ao)", &paths);
	g_variant_unref (ret);

	gboolean status = TRUE;
	int length = 0;
	for (i = 0; paths[i]; i++)
    {        
    	//如果这个路径的ssid是要找的
        found = get_active_connection_details (paths[i], ssid);
        if(found == TRUE)
        {
        	length +=strlen(paths[i]);
        }       
        
    }
    if(length < 200)
	{
		for (i = 0; paths[i]; i++)
    	{        
	    	//如果这个路径的ssid是要找的
	        found = get_active_connection_details (paths[i], ssid);
	        if(found == TRUE)
	        {
	        	strcat(err, paths[i]);
	        	strcat(err,",");
	        }        
    	}
	}
	else
	{
		strcpy(err, "too many connections on this ssid, need to remove all");
		status = FALSE;
	}
	g_strfreev (paths);
    
	g_object_unref (proxy);

	return ((status == TRUE) ? 0: 2);
}
//修改某一个属性的值
#if 0
int update_property(const char *path, const char* property, const char* value, char * err)
{
	GDBusProxy *props_proxy;
	GVariant *ret = NULL, *setting = NULL;
	//const char *path = NULL;
	GError *error = NULL;
	gboolean status = TRUE;


	/* Create a D-Bus object proxy for the active connection object's properties */
	props_proxy = g_dbus_proxy_new_for_bus_sync (G_BUS_TYPE_SYSTEM,
                                 G_DBUS_PROXY_FLAGS_NONE,
                                 NULL,
                                 //"org.freedesktop.NetworkManager"
                                 NM_DBUS_SERVICE,
                                 path,
	 "org.freedesktop.NetworkManager.Settings.Connection",
	                                             NULL, NULL);
	g_assert (props_proxy);

	/* Get the object path of the Connection details */
	ret = g_dbus_proxy_call_sync (props_proxy,
	                              "GetSettings",
	                              NULL,
	                              G_DBUS_CALL_FLAGS_NONE, -1,
	                              NULL, &error);
	if (!ret) {
		g_dbus_error_strip_remote_error (error);
		g_warning ("Failed to get active connection Connection property: %s\n",
		           error->message);
		g_error_free (error);
		status = FALSE;
		goto out;
	}

	g_variant_get (ret, "(@a{sa{sv}})", &setting);

	GVariant *connection = NULL;
	gboolean found;
	const char  *auto, *id;
	//const gchar *ssid;
	gchar ssid[100];
	GVariant *a;
	connection = g_variant_lookup_value (setting, NM_SETTING_CONNECTION_SETTING_NAME, NULL);
	found = g_variant_lookup (connection, "autoconnect", "&s", &auto);
	if(found == NULL)
	{
		//增加auto = false选项
	}
	//g_assert (found);
	else
	{
		if(strcmp(auto, "true") == 0)
		{
			//修改为false
		}
		else if(strcmp(auto, "false") == 0)
		{
			//不需要做什么动作
		}
	}

out:
	if (setting)
		g_variant_unref (setting);
	if (ret)
		g_variant_unref (ret);
	g_object_unref (props_proxy);
    return ((status == TRUE)?0:1);
	//修改值
	//写回
}
#endif
/*6. 测试网络连通性函数 check_connectivity()*/

//这里如果一连上就去ping会出现connect: Network is unreachable

int
check_connectivity(const char *iface, char *ip, char * err)
{
/*	int ret = -1;
	ret = system("ping 192.168.1.1");
//	ret = system("ping 192.168.1.1 -c 1");
	if(ret == 0)
	{
		strcpy(err, "network ok");
		return TRUE;
	}
	else
	{
		strcpy(err, "network not working");
		return FALSE;
	}
	//增加ping外网的需求，ping150主控
	*/
	//一种方法，一直去查询dbus的某一个值
	/*
	string cmd = "nmcli c add type wifi con-name tikong-wifi ifname ";
    	cmd.append(WLAN);
    	cmd.append(" ssid tikong");
        //system("nmcli c add type wifi con-name tikong-wifi ifname wlp3s0 ssid tikong");
        system(cmd.c_str());
	*/

	gboolean status;
	char cmd[200]="";
	strcpy(cmd, "ifconfig | grep ");
	strcat(cmd, iface);
	strcat(cmd, " -A1 | grep -v Link");
	//exec("ifconfig | grep wlp2s0 -A1 | grep -v Link", err);
	exec(cmd, err);
	//exec("ifconfig | grep wlp3s0 -A1 | grep -v Link");
	printf("%s\n", err);
	if(strstr(err, ip) != NULL)
	{
		memset(cmd, 0, sizeof cmd);
		strcpy(cmd, "ping ");
		strcat(cmd, ip);
		strcat(cmd, ".1 -c 1");
		int ret = -1;
		ret = system(cmd);
		if(ret == 0)
		{
			strcpy(err, "network ok");
			return 0;
		}
		else
		{
			strcpy(err, "network not working");
			return 1;
		}
		//return 0;
	}
	return 1;
	//这里应该提供信号强度
} 



//void connect_wifi(const char *ssid, char *ret)
int connect_wifi(const char *iface, const char *ssid, const char *pw, char *ret)
{ 
	
    gboolean status = FALSE;

    status = find_hw(iface);
    if(status == TRUE)
    {
        //printf("found wifi hardware\n");  
        //strcpy(ret,"found wifi hardware"); 
    }
    else if(status == FALSE)
    {
        printf("no wifi hardware, return\n"); 
        strcpy(ret,"no wifi hardware"); //错误1
        return 1;
    }
    //check_exist函数里会把连接的PATH找到
    status = check_exist(ssid);
    if(status == TRUE)
    {
        printf("found wifi ssid\n");  
    }
    else if(status == FALSE)
    {
    	printf("need establish %s connection\n", ssid);
        status = add_connection (iface, ssid, pw,ret);
        if(status == FALSE)
	    {
	    	printf("%s\n", ret); //错误2 已经拷贝进去
	        return 2;
	    }
	    else if(status == TRUE)
	    {
	    	printf("add connection ok\n"); 
    	}
    }
    //enable wifi connection
    status = active_conn(ret); //错误3
    if(status == TRUE)
    {
    	strcpy(ret,"connection enabled"); //正确1
    }
    else
    {
    	//错误信息已经在enable_conn函数里拷贝到ret里了  //错误3
    	return 3;
    }
    //sleep(8);
    return 0;

}

int get_version(char *ver)
{
	strcpy(ver, "1.1");
	return 0;
}