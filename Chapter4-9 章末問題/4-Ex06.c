// 4-Ex06.c
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
    int hnd;
    int lFlgOut=0;
    int leds[LNUM]={LED0,LED1,LED2,LED3};
    int levels[LNUM]={0,0,0,0};
    uint64_t bits;
    uint64_t gMsk=0b1111;
    hnd = lgGpiochipOpen(PI5);
    //hnd = lgGpiochipOpen(PI4B);
    lgGroupClaimOutput(hnd,lFlgOut,LNUM,leds,levels);
    
    while(1){                   //永久ループ
        printf("0から15までの値を入力してください>>");
        scanf("%lu",&bits);     //%lu:unsigned long
        if(0 <= bits && bits <=15){   //入力データが0から15までの範囲かどうか判断
            lgGroupWrite(hnd,LED0,bits,gMsk);   //値を出力
        }else{
            printf("入力された値が範囲外です\n\n");
            bits = 0;
            lgGroupWrite(hnd,LED0,bits,gMsk);   //消灯
        }
    }
    lgGpiochipClose(hnd);
    return EXIT_SUCCESS;
}
