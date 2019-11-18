
/*
author: matt ji
date: 2019-8-29
brief: 通过dbus接口来控制wifi
v0.9 2019-8-26
gcc -Wall tkwifi.c wifi.c wifi-fun.c -o tkwifi `pkg-config --cflags --libs libnm uuid`
v1.0 2019-11-5
1. 重新梳理流程
2. 也可以做为测试电梯wifi的程序

*/


/*
6个接口：

1. 寻找到wif硬件设备，比如/org/freedesktop/NetworkManager/Devices/0
2. 检查所有连接，看有没有特定连接(tikong-00013)的名字 check_exist,
    如果有，就记录下来/org/freedesktop/NetworkManager/Settings/10
3. 如果没有，创建连接函数 add_wifi_connection, CON_NAME 为tikong-00013与ssid一样
    不能设置成自动连接，因为如果有好几部电梯，就不知道连的哪一部电梯


一. 连接wifi connect_wifi(const char *ssid) return const char *;
    1. 寻找wifi硬件 find_hw()
        a. 成功继续进行第二步
        b. 不成功返回no wifi hardware字符串
    2. 检查之前是否已经连接过
        a. 第一次连接 创建连接，add_connect()
        b。 已经连接过，enable_connect()

connect_wifi()
1. 找到wifi接口设备path/DEVICES/1
2. 调用path/DEVICES/1使用Disconnect，可以断连wifi
3. 然后再连接特定的连接
4. 使用完之后需要再断连wifi，而不只是断连tikong的SSID

新流程：
1. 得到库版本
2. 连接wifi
    2.1 
3. 测试连通性，需要输入测试ip的网段
4. 断连wifi
*/

#include "wifi.h"
#include "stdio.h"
#include "unistd.h"
#include <string.h>
#include <sys/time.h>

//gcc tkwifi.c libtkwifi.a -o tkwifi -lpthread -lnm -lgio-2.0 -lgobject-2.0 -lglib-2.0 -luuid

//===========测试程序开始=========
int
main (int argc, char *argv[])
{

//将要连接的ssid作为参数传入
    int status;
    char ret[500]="";
    status = get_version(ret);
    printf("tk wifi version %s\n", ret);
    if(argc != 4)
    {
        printf("input wlp2s0 tikong tikong-4g \n");
        return 1;
    }
    memset(ret, 0, sizeof ret);
    status = disconnect_wifi(argv[1], ret);
    if(status == 0)
    {
        printf("disconnet wifi\n");
    }
    else
    {
        printf("%s\n", ret);
    }
    memset(ret, 0, sizeof ret);
    //disable 
    //status = disable_auto(ret);
    //status = list_connections(argv[2], ret);
    //printf("%s\n", ret);
    //memset(ret, 0, sizeof ret);    
    //status = update_property("/org/freedesktop/NetworkManager/Settings/9", property, "false", ret);

    struct timeval start, end;

    //计算时间开始
    gettimeofday(&start, 0);
    //connect_wifi(argv[1], ret);
    //void connect_wifi(const char *iface, const char *ssid, const char *pw, char *ret);
    status = connect_wifi(argv[1], argv[2], argv[3], ret);
    if(status != 0)
    {
        printf("%s\n", ret);  //各种错误，包括添加连接和使能连接这两个过程中出现的各种错误
        return 1;
    }
    //这里如果一连上就去ping会出现connect: Network is unreachable,实际上在命令行是可以ping通的
        memset(ret, 0, sizeof ret);

    int try = 0;
    while(try < 20)
    {
        status = check_connectivity(argv[1], "192.168.1", ret);
        if(status == 0)
        {
            printf("got ip address\n");
            break;
        }
        else
        {
            try++;
            sleep(1.5);
        }
    }
    printf("%s\n", ret);
    //计算时间结束
    gettimeofday(&end, 0);
    double timeuse = 1000000*(end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    timeuse /= 1000; // 1000 to ms, 1000000 to seconds
    printf("timeuse %fms\n", timeuse);
    sleep(10);
    //wifi断连
    
    /*status = remove_conn(argv[2], ret);
    if(status != 0)
    {
        printf("%s\n", ret);
    }*/
    return 0;
}
//===========测试程序结束=========
