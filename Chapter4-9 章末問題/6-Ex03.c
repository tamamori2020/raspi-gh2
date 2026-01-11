// 6-Ex03.c
// gcc -Wall -o "%e" "%f" -llgpio -lpthread -g -O0
// Geanyのオプションの"%e"は実行ファイル名, "%f"はソースファイル名のこと

#include <stdio.h>
#include <stdlib.h>     //EXIT_SUCCESS
#include <unistd.h>     //usleep
#include <lgpio.h>      //lgTxPulse,etc
#define PI5     4       // /dev/gpiochip4
#define PI4B    0       // /dev/gpiochip0
#define BUZZER  18      //圧電サウンダ
#define SW0     4       //GPIO4をSW0と定義

int main(void) {
    int hnd;
    int lFlgOut=0;
    int lFlgIn=LG_SET_PULL_DOWN;    //プルダウン抵抗を有効
    hnd = lgGpiochipOpen(PI5);
    //hnd = lgGpiochipOpen(PI4B);
    lgGpioClaimOutput(hnd,lFlgOut,BUZZER,LG_LOW);   //BUZZERを出力に設定
    lgGpioClaimInput(hnd,lFlgIn,SW0);               //SW0を入力に設定

    while(1){
        if(lgGpioRead(hnd, SW0)==LG_HIGH){
            lgTxPulse(hnd,BUZZER,1136,1136,0,0);    //440Hzのパルス信号出力
        }else{                                      //1136usは440Hzの半周期
            lgTxPulse(hnd,BUZZER,0,0,0,0);
            lgGpioWrite(hnd,BUZZER,LG_LOW);
        }
        usleep(1000);               //CPU使用率の抑制のため
    }
    lgGpiochipClose(hnd);
    return EXIT_SUCCESS;
}
