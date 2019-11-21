
/*
author: matt ji
date: 2019-11-21
在梯控设备上测试信号


*/


#include "wifi.h"
#include "stdio.h"
#include "unistd.h"
#include <stdbool.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <sys/time.h>



//===========测试程序开始=========
int
main (int argc, char *argv[])
{

    int status;
    //缓冲区大于500字节
    char ret[501]={};
    struct timeval start, end;
    status = get_version(ret);
    if(status == 0)
    {
        printf("tk wifi version %s, %ld\n", ret, sizeof(ret));
    }
    char buffer[33]={};
    struct timeval tv;
    time_t curtime;
    while(1)
    {
        status = check_signal("wlp2s0", ret);
        if(status != 70)
        {
            printf("err code: %d, err msg: %s\n", status, ret);
            //写入文件
            FILE *fp;

           fp = fopen("/tmp/start.txt", "a");
           gettimeofday(&tv, NULL);
           curtime = tv.tv_sec;
           strftime(buffer, 30, "%Y-%m-%d-%T: ", localtime(&curtime));
           fputs(buffer, fp);
           fprintf(fp, "%s\n", ret);
           //fputs("This is testing for fputs...\n", fp);
           fclose(fp);
        }
        sleep(10);
    }

    /*status = remove_conn(argv[2], ret);
    if(status != 0)
    {
        printf("%s\n", ret);
    }*/
    return 0;
}
//===========测试程序结束=========
