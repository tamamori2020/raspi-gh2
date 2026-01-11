// 4-Ex05.c
// gcc -Wall -o "%e" "%f" -llgpio -lpthread -g -O0
// Geanyのオプションの"%e"は実行ファイル名, "%f"はソースファイル名のこと

#include <stdio.h>      //printf,fflush
#include <stdlib.h>     //EXIT_SUCCESS
#include <lgpio.h>      //lgGpiochipOpen,lgGpioWrite,etc
#define PI5     4       // /dev/gpiochip4
#define PI4B    0       // /dev/gpiochip0
#define LED0    23      //LED0からLED3までのGPIO番号の定義
#define LED1    22
#define LED2    25
#define LED3    24
#define LNUM    4       //LEDの数

int main (void){
    int i,hnd;
    int lFlgOut=0;
    int leds[LNUM]={LED0,LED1,LED2,LED3};
    int levels[LNUM]={0,0,0,0};
    uint64_t bits;
    uint64_t gMsk=0b1111;
    hnd = lgGpiochipOpen(PI5);
    //hnd = lgGpiochipOpen(PI4B);
    lgGroupClaimOutput(hnd,lFlgOut,LNUM,leds,levels);
    
    while(1){                                   //永久ループ
        for(i = 0; i < 4; i++){                 //4回
            if(i == 0){bits = 0b0001;}          //i=0のとき、bitsを1に初期化
            lgGroupWrite(hnd,LED0,bits,gMsk);   //値を出力
            lguSleep(0.5);                      //0.5秒時間待ち
            bits <<= 1;                         //左シフト
        }
        for(i = 0; i < 2; i++){                 //2回
            if(i == 0){bits = 0b0100;}          //i=0のとき、bitsを0b0100に初期化
            lgGroupWrite(hnd,LED0,bits,gMsk);   //値を出力
            lguSleep(0.5);                      //0.5秒時間待ち
            bits >>= 1;                         //右シフト
        }
    }
    lgGpiochipClose(hnd);
    return EXIT_SUCCESS;
}
