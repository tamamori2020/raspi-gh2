// 6-Ex07.c
// gcc -Wall -o "%e" "%f" -llgpio -lpthread -g -O0
// Geanyのオプションの"%e"は実行ファイル名, "%f"はソースファイル名のこと

#include <stdio.h>      //printf
#include <stdlib.h>     //EXIT_SUCCESS
#include <unistd.h>     //usleep
#include "lgpio.h"      //lgTxPwm,etc
#define PI5     4
#define PI4B    0
#define SW0     4       //SW0からSW7までのGPIO番号の定義
#define SW1     5
#define SW2     6
#define SW3     26
#define SW4     17
#define SW5     27
#define SW6     20
#define SW7     21
#define SNUM    8       //SWの数
#define LED0    23

int main (void){
    int hnd;
    int lFlgIn=LG_SET_PULL_DOWN;    //プルダウン抵抗を有効
    int sw70[SNUM] = {SW0,SW1,SW2,SW3,SW4,SW5,SW6,SW7};
    uint64_t bits;
    //hnd = lgGpiochipOpen(PI5);
    hnd = lgGpiochipOpen(PI4B);
    lgGroupClaimInput(hnd, lFlgIn, SNUM, sw70);
    int lFlgOut=0;
    lgGpioClaimOutput(hnd,lFlgOut,LED0,LG_LOW);
    int fpwm = 100;     //100HzのPWM信号
    float duty = 0;
    
    while(1){
        lgGroupRead(hnd, SW0, &bits);
        switch(bits){                       //デューティ比の設定
            case   1 : duty=12.5; break;
            case   2 : duty=25.0; break;
            case   4 : duty=37.5; break;
            case   8 : duty=50.0; break;
            case  16 : duty=62.5; break;
            case  32 : duty=75.0; break;
            case  64 : duty=87.5; break;
            case 128 : duty=100.0;break;
            default  : duty=0; break;
        }
        lgTxPwm(hnd, LED0, fpwm, duty,0,0); //100HzのPWM信号出力
        usleep(1000);                       //CPU使用率の抑制のため
        printf("bits= %ld  duty=%0.1f\n",bits,duty);    //デバック用
    }
    lgGpiochipClose(hnd);
    return EXIT_SUCCESS;
}
