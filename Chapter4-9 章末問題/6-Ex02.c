// 6-Ex02.c
// gcc -Wall -o "%e" "%f" -llgpio -lpthread -g -O0
// Geanyのオプションの"%e"は実行ファイル名, "%f"はソースファイル名のこと

#include <stdio.h>
#include <stdlib.h>     //EXIT_SUCCESS
#include <lgpio.h>      //lgGpiochipOpen,lgTxPulse,etc
#define PI5     4       // /dev/gpiochip4
#define PI4B    0       // /dev/gpiochip0
#define LED0    23      //LED0からLED2までのGPIO番号の定義
#define LED1    22
#define LED2    25
#define LNUM    3       //LEDの数

int main (void){
    int hnd;
    int lFlgOut=0;
    int leds[LNUM]={LED0,LED1,LED2};
    int levels[LNUM]={0,0,0};
    uint64_t gMsk=0b111;
    hnd = lgGpiochipOpen(PI5);
    //hnd = lgGpiochipOpen(PI4B);
    lgGroupClaimOutput(hnd,lFlgOut,LNUM,leds,levels);
    for(;;){
        lgTxPulse(hnd,LED0,500000,500000,0,0);  //1Hzのパルス信号出力
        lgTxPulse(hnd,LED1,250000,250000,0,0);  //2Hzのパルス信号出力
        lgTxPulse(hnd,LED2,166667,166667,0,0);  //3Hzのパルス信号出力
    }
    lgGroupWrite(hnd,LED0,0,gMsk);  //全LEDを消灯
    lgGpiochipClose(hnd);
    return EXIT_SUCCESS;
}
