// 9-Ex01.c
// gcc -Wall -o "%e" "%f" -llgpio -lpthread -g -O0 libMyPi2.a
// Geanyのオプションの"%e"は実行ファイル名, "%f"はソースファイル名のこと

#include <stdio.h>
#include <stdlib.h>     //system,EXIT_SUCCESS
#include <lgpio.h>      //lgGpiochipOpen,lgGpioRead,etc
#define PI5     4       // /dev/gpiochip4
#define PI4B    0       // /dev/gpiochip0
#define SW0     4       //GPIO4をSW0と定義

int main(void){
    int hnd;
    int inFlg = LG_SET_PULL_DOWN;               //プルダウン抵抗を有効
    hnd = lgGpiochipOpen(PI5);
    //hnd = lgGpiochipOpen(PI4B);
    lgGpioClaimInput(hnd,inFlg,SW0);            //SW0を入力に設定

    while(1){
        if(lgGpioRead(hnd, SW0)==LG_HIGH){      //SW0が押されたら撮影する
            system("rpicam-still -t 500 -o $(date +'%Y%m%d-%H%M%S').jpg");
            lguSleep(1);                        //1秒時間待ち
        }
        lguSleep(0.001);                        //CPU使用率の抑制のため
    }
    lgGpiochipClose(hnd);
    return EXIT_SUCCESS;
}
