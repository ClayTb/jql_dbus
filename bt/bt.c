/*
author: matt ji
date: 2019-9-4
brief: 通过dbus接口来控制蓝牙
v1.0 2019-8-26
gcc -Wall bt.c wifi.c wifi-fun.c -o bt `pkg-config --cflags --libs libnm uuid`
*/
/*
1. 安装networkmanager
2. 蓝牙设备的路径是/org/bluez/hci0
*/
#include "bt.h"
//===========测试程序开始=========
int
main (int argc, char *argv[])
{
    gboolean found = FALSE;
    found = check_exist();
    if(found == TRUE)
    {
        //remove_wifi_connection(SSID);        
    }
    else if(found == FALSE)
    {
        add_wifi_connection(CON_NAME);
    }
    //enable wifi connection
    connect_wifi();
    sleep(5);
    check_connectivity();
    sleep(3);
    //使用完之后，断连wifi
    disconn_wifi(WIFIDEVICE);
        
    return 0;
}
//===========测试程序结束=========