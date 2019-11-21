# dbus
tikong-cmd: 里面是命令行的代码
tikong：里面是使用dbus的代码
网卡名 ssid 密码 buf(抛异常c++) 
ar -rcs libtkwifi.a wifi.o wifi-fun.o 
gcc -shared wifi.o wifi-fun.o -o libtkwifi.so

### 接口说明
下面所有的api的：
ret返回字符串请设置大于500字节
返回值0：正确 其他值：错误
1. 获得库版本
	int get_version(char *ver);
2. 连接wifi，参数: 网口名 ssid 密码 dft_route没有特殊要求下设置为0 
	int connect_wifi(const char *iface, const char *ssid, const char *pw, const int dft_route, char *ret);
3. 断开当前iface上的wifi连接
	int disconnect_wifi(const char *iface, char *ret);
4. 测试是否连接上，ssid 是要测试的ssid，ip为需要测试的网段，比如192.168.4.
	int check_connectivity(const char *iface, const char *ssid, const char *ip, char * ret);
5. (option)删除所有这个ssid的连接
	int remove_conn(const char *ssid, char *ret);  
6. 获取当前某个接口的网络状态
    int check_signal(const char * iface, char *ret)
    iface分为 有线网络 wifi 4g网络等等，需要明确地给出接口名
    比如有线网络eth0 wifi是wlp3s0，4g的接口wwp0s20f0u6c2 诸如此类
    返回值：1. 有线网络 返回ping baidu的时间
        2. wifi 返回 wifi 信号，以及 ping baidu的时间
        3. 4g 返回 4g网络的信号，以及 ping baidu的时间
    如果当前这个接口有任何问题，会通过ret返回错误
    不断监听网络状态，需要建立一个线程了

### 版本说明
v1.2 2019-11-18
1. connect_wifi接口增加设置default route选项
2. check_connectivity增加ssid参数
2. 所有接口增加参数无效检测
3. 内部接口重组

v1.1 2019-11-12
1. 内部增加使能wifi功能 Done
2. 内部增加检查wifi iface为指定wlp3s0功能 Done
3. 对外增加删除wifi连接的接口remove_conn(const char * ssid, char *ret),
    删除所有这个ssid上的连接 Done
4. 更新disconnect_wifi逻辑，变为当前iface如果有活动的wifi，断掉 Done
disconn之后，即使有wifi是auto的，也不会自动连上
5. option，disable_auto(),对外增加修改其他ap的auto connect，
    针对目前小车会建立一个ap的情况，
    小车会自动连接上这个ap
    或者直接删除这个ap
    这个一旦连接上，nmcli conn up Candela可以连接上，
    但是nmcli dev wifi就不能搜索热点了，因为现在本身是热点模式
    建议每次开机调用一次，待实现
5. 对外增加测试信号强度的接口get_signal(const char * ssid, char * signal);

### 安装说明
sudo apt-get install libnm-dev uuid-dev

### 编译
gcc -Wall tkwifi.c wifi.c wifi-fun.c -o tkwifi `pkg-config --cflags --libs libnm uuid`
