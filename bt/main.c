/*
author: matt ji
date: 2019-9-4
brief: 通过dbus接口来控制蓝牙
v1.0 2019-9-4
1. wifi的编译命令
gcc -Wall bt.c wifi.c wifi-fun.c -o bt `pkg-config --cflags --libs libnm uuid`
2. 蓝牙的编译命令
gcc `pkg-config --cflags glib-2.0 gio-2.0` -o bt bt.c bt-dbus.c dbus-fun.c `pkg-config --libs glib-2.0 gio-2.0`

*/
/*
1. 安装networkmanager
2. 蓝牙设备的路径是/org/bluez/hci0 
3. 蓝牙名称 蓝牙mac地址 HCI Name: raspberrypi Address: B8:27:EB:9F:30:A9
*/
#include "bt-dbus.h"
//===========测试程序开始=========
int
main (int argc, char *argv[])
{
    int status = -1;
    //缓冲区大于500字节
    char ret[501]={};
    status = get_version(ret);
    if(status == 0)
    {
        printf("bluetooth tool version %s, %ld\n", ret, sizeof(ret));
    }
    int found = -1;
    found = get_bt(ret);
    if(found != 0)
    {
        g_print("%s\n", ret);
        return 1;
    }

    g_print("found bluetooth\n");
    status = start_bt(ret);
    
        
    return 0;
}
//===========测试程序结束=========