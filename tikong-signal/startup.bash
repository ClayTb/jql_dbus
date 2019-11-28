#author: matt ji
#copyright: candela
#date: 2019.7.4
#purpose: 守护进程，检测到梯控程序不在运行，就启动梯控程序
#!/usr/bin/env bash
sleep 20
echo v0.1
while true; do
    output="$(ps aux | grep cti-elevator | grep -v grep)"
    if [[ -n $output ]]; then
        #echo $output
        :
    else
        date >> /home/tikong/log/start.log
        echo "not start, start then" >> /home/tikong/log/start.log
        /home/tikong/code/tikong-ziyan/cti-elevator &
    fi

    output="$(ps aux | grep tksignal | grep -v grep)"
    if [[ -n $output ]]; then
        #echo $output
        :
    else
        date >> /home/tikong/log/start.log
        echo "tk signal not start, start then" >> /home/tikong/log/start.log
        /home/tikong/code/signal/tksignal 5 &
    fi

    output="$(ps aux | grep candela_cloud_elevator | grep -v grep)"
    if [[ -n $output ]]; then
        #echo $output
        :
    else
        date >> /home/tikong/log/start.log
        echo "daemon not start, start then" >> /home/tikong/log/start.log
        service candela-elevator-daemon restart &
    fi
    sleep 3
done
    
