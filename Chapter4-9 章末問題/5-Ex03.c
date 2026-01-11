// 5-Ex03.c
// gcc -Wall -o "%e" "%f" -llgpio -lpthread -g -O0
// Geanyのオプションの"%e"は実行ファイル名, "%f"はソースファイル名のこと

#include <stdio.h>      //printf
#include <stdlib.h>     //EXIT_SUCCESS
#include <unistd.h>     //usleep
#include <lgpio.h>      //lgGpiochipOpen,lgGroupRead,etc
#define PI5     4       // /dev/gpiochip4
#define PI4B    0       // /dev/gpiochip0
#define SW0     4       //SW0からSW7までのGPIO番号の定義
#define SW1     5
#define SW2     6
#define SW3     26
#define SW4     17
#define SW5     27
#define SW6     20
#define SW7     21
#define SNUM    8       //SWの数
#define LED0    23      //LED0からLED3までのGPIO番号の定義
#define LED1    22
#define LED2    25
#define LED3    24
#define LNUM    4       //LEDの数

int main (void){
    int hnd;
    int lFlgIn=LG_SET_PULL_DOWN;    //スイッチ用
    int sw70[SNUM]={SW0,SW1,SW2,SW3,SW4,SW5,SW6,SW7};
    uint64_t bits;
    uint64_t sw74;

    int lFlgOut=0;                  //LED用
    int leds[LNUM]={LED0,LED1,LED2,LED3};
    int levels[LNUM]={0,0,0,0};
    uint64_t gMsk=0b1111;

    hnd = lgGpiochipOpen(PI5);
    //hnd = lgGpiochipOpen(PI4B);
    lgGroupClaimInput(hnd, lFlgIn, SNUM, sw70);
    lgGroupClaimOutput(hnd,lFlgOut,LNUM,leds,levels);

    while(1){
        lgGroupRead(hnd, SW0, &bits);
        if(bits>15){        //上位4bitの処理
            sw74 = bits;
            sw74 >>= 4;
            bits = (bits | sw74) & 0b01111; //　| ビット和、& ビット積
        }
        lgGroupWrite(hnd,LED0,bits,gMsk);
        usleep(1000);               //CPU使用率の抑制のため
    }
    lgGpiochipClose(hnd);
    return EXIT_SUCCESS;
}
