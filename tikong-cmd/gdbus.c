
/*
v1.0 2019-8-26
gcc -Wall tkwifi-gdbus.c -o tkwifi-gdbus `pkg-config --cflags --libs libnm uuid`
*/
/*
std::string exec(const char* cmd) {
    char buffer[128];
    std::string result = "";
    FILE* pipe = popen(cmd, "r");
    if (!pipe) throw std::runtime_error("popen() failed!");
    try {
        while (fgets(buffer, sizeof buffer, pipe) != NULL) {
            result += buffer;
        }
    } catch (...) {
        pclose(pipe);
        throw;
    }
    pclose(pipe);
    return result;
}

int main(int argc, char *argv[])
{
	std::cout << exec("ping -c 1 www.baidu.com 2>&1");
	return 0;
}*/


//1. wifi控制
/*
1. 检查所有连接，看有没有特定连接(tikong-wifi)的名字 check_exist
2. 创建连接函数 add_wifi_connection
3. 删除连接 remove_connection()
4. 连接wifi函数 connect_wifi()
5. 断开wifi函数 disconn_wifi()
6. 测试网络连通性函数 check_conn()

1. 找到wifi接口设备path/DEVICES/1
2. 调用path/DEVICES/1使用Disconnect，可以断连wifi
3. 然后再连接特定的连接
4. 使用完之后需要再断连wifi，而不只是断连tikong的SSID
ping可以使用之前写过的ping代码
*/

//2. 蓝牙控制
/*
2.1 树莓派实现主设备，手机作为从设备可以连接上树莓派
2.2 后期使用蓝牙模块连接一个继电器控制梯控电源，使用手机连接蓝牙模块，
    发送控制指令
*/
//3. 4g控制    
/*
1. 连接
2. 断连
*/
#include "wifi.h"
//===========测试程序开始=========
int
main (int argc, char *argv[])
{
    //wifi part
    
    gboolean found = FALSE;
    found = check_exist();
    if(found == TRUE)
    {
        //remove_connection();        
    }
    else if(found == FALSE)
    {
        add_wifi_connection();
    }
    //enable wifi connection
    connect_wifi();
    //cpp
    Wifi wifi{DISCONN};
    
    //bluetooth part
    
    //wifi part
    
    
    return 0;
}
//===========测试程序结束=========
