// 4-Ex01.c
// gcc -Wall -o "%e" "%f" -llgpio -lpthread -g -O0
// Geanyのオプションの"%e"は実行ファイル名, "%f"はソースファイル名のこと

#include <stdio.h>
#include <stdlib.h>     //EXIT_SUCCESS
#include <lgpio.h>      //lgGpiochipOpen,lgGpioWrite,etc
#define PI5     4       // /dev/gpiochip4
#define PI4B    0       // /dev/gpiochip0
#define LED0    23      //GPIO23をLED0と定義
#define LED1    22

int main(void) {
    int hnd;
    int lFlgOut=0;      //出力のオプション
    hnd = lgGpiochipOpen(PI5);
    //hnd = lgGpiochipOpen(PI4B);
    lgGpioClaimOutput(hnd,lFlgOut,LED0,LG_LOW);
    lgGpioClaimOutput(hnd,lFlgOut,LED1,LG_LOW);
    for(;;){
        lgGpioWrite(hnd,LED0,LG_HIGH);      //1
        lgGpioWrite(hnd,LED1,LG_HIGH);
        lguSleep(0.25);      //0.25秒時間待ち
        lgGpioWrite(hnd,LED0,LG_HIGH);      //2
        lgGpioWrite(hnd,LED1,LG_LOW);
        lguSleep(0.25);      //0.25秒時間待ち
        lgGpioWrite(hnd,LED0,LG_LOW);       //3
        lgGpioWrite(hnd,LED1,LG_HIGH);
        lguSleep(0.25);      //0.25秒時間待ち
        lgGpioWrite(hnd,LED0,LG_LOW);       //4
        lgGpioWrite(hnd,LED1,LG_LOW);
        lguSleep(0.25);      //0.25秒時間待ち
    }
    lgGpiochipClose(hnd);
    return EXIT_SUCCESS;
}
