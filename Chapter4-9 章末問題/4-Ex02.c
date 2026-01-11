// 4-Ex02.c
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
    int lFlgOut=0;      //出力のオプション
    int leds[LNUM]={LED0,LED1,LED2,LED3};   //LEDのグループ
    int levels[LNUM]={0,0,0,0};             //初期値のグループ
    uint64_t bits;                          //グループデータ
    uint64_t gMsk=0b1111;                   //ビットマスク
    hnd = lgGpiochipOpen(PI5);
    //hnd = lgGpiochipOpen(PI4B);
    lgGroupClaimOutput(hnd,lFlgOut,LNUM,leds,levels);   //グループ出力
    
    for(i = 0; i < 10; i++){
        bits = 0b0101;
        lgGroupWrite(hnd,LED0,bits,gMsk);   //グループライト
        lguSleep(0.5);                      //0.5秒時間待ち
        bits = 0b1010;
        lgGroupWrite(hnd,LED0,bits,gMsk);   //グループライト
        lguSleep(0.5);                      //0.5秒時間待ち
    }
    bits = 0;
    lgGroupWrite(hnd,LED0,bits,gMsk);       //全LEDを消灯
    lgGpiochipClose(hnd);
    return EXIT_SUCCESS;
}
