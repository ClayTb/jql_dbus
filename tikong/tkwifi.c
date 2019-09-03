
/*
author: matt ji
date: 2019-8-29
brief: 通过dbus接口来控制wifi
v1.0 2019-8-26
gcc -Wall tkwifi.c wifi.c wifi-fun.c -o tkwifi `pkg-config --cflags --libs libnm uuid`
*/


/*
6个接口：
1. 检查所有连接，看有没有特定连接(tikong-wifi)的名字 check_exist
2. 创建连接函数 add_wifi_connection
3. 删除连接 remove_connection()
4. 连接wifi函数 connect_wifi()
5. 断开wifi函数 disconn_wifi()
6. 测试网络连通性函数 check_conn()

connect_wifi()
1. 找到wifi接口设备path/DEVICES/1
2. 调用path/DEVICES/1使用Disconnect，可以断连wifi
3. 然后再连接特定的连接
4. 使用完之后需要再断连wifi，而不只是断连tikong的SSID
*/

#include "wifi.h"
//===========测试程序开始=========
int
main (int argc, char *argv[])
{
    gboolean found = FALSE;
    found = check_exist(SSID);
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
