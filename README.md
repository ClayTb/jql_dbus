# dbus
tikong-cmd: 里面是命令行的代码
tikong：里面是使用dbus的代码
网卡名 ssid 密码 buf(抛异常c++) 
ar -rcs libtkwifi.a wifi.o wifi-fun.o 
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