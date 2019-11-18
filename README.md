# dbus
tikong-cmd: 里面是命令行的代码
tikong：里面是使用dbus的代码
网卡名 ssid 密码 buf(抛异常c++) 
ar -rcs libtkwifi.a wifi.o wifi-fun.o 

### 接口说明
下面所有的api的：
返回字符串请设置大小为500字节
返回值0：正确 其他值：错误
1. 获得库版本
int get_version(char *ver);
2. 连接wifi，参数 网口名 ssid 密码 返回具体错误字符串
int connect_wifi(const char *iface, const char *ssid, const char *pw, char *ret);
3. 断开当前iface上的wifi连接
int disconnect_wifi(const char *iface, char *err);
4. 测试是否连接上，ip为需要测试的网段，比如192.168.4
int check_connectivity(const char *iface, char * ip, char *err);
5. (option)删除所有这个ssid的连接
int remove_conn(const char *ssid, char *ret);  

### 版本说明
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