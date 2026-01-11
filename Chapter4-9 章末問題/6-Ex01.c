// 6-Ex01.c
// gcc -Wall -o "%e" "%f" -llgpio -lpthread -g -O0
// Geanyのオプションの"%e"は実行ファイル名, "%f"はソースファイル名のこと

#include <stdio.h>
#include <stdlib.h>     //EXIT_SUCCESS
#include <lgpio.h>      //lgGpiochipOpen,lgTxPulse,etc
#define PI5     4       // /dev/gpiochip4
#define PI4B    0       // /dev/gpiochip0
#define LED0    23      //GPIO23をLED0と定義

int main (void){
    int hnd;
    int lFlgOut=0;
    hnd = lgGpiochipOpen(PI5);
    //hnd = lgGpiochipOpen(PI4B);
    lgGpioClaimOutput(hnd,lFlgOut,LED0,LG_LOW); //LED0を出力に設定
    for(;;){
        lgTxPulse(hnd,LED0,500000,500000,0,0);  //1Hzのパルス信号出力
    }
    lgGpioWrite(hnd,LED0,LG_LOW);   //LED0を消灯
    lgGpiochipClose(hnd);
    return EXIT_SUCCESS;
}
