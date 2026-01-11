// 5-Ex02.c
// gcc -Wall -o "%e" "%f" -llgpio -lpthread -g -O0
// Geanyのオプションの"%e"は実行ファイル名, "%f"はソースファイル名のこと

#include <stdio.h>      //printf
#include <stdlib.h>     //EXIT_SUCCESS
#include <unistd.h>     //usleep
#include <lgpio.h>      //lgGpiochipOpen,lgGroupRead,etc
#define PI5     4       // /dev/gpiochip4
#define PI4B    0       // /dev/gpiochip0
#define SW0     4       //SW0からSW3までのGPIO番号の定義
#define SW1     5
#define SW2     6
#define SW3     26
#define SNUM    4       //SWの数
#define LED0    23      //LED0からLED3までのGPIO番号の定義
#define LED1    22
#define LED2    25
#define LED3    24
#define LNUM    4       //LEDの数

int main (void){
    int hnd;
    int lFlgIn=LG_SET_PULL_DOWN;            //プルダウン抵抗の有効
    int sw30[SNUM]={SW0,SW1,SW2,SW3};       //スイッチのグループ
    uint64_t bits;                          //グループデータ

    int lFlgOut=0;                          //出力のオプション
    int leds[LNUM]={LED0,LED1,LED2,LED3};   //LEDのグループ
    int levels[LNUM]={0,0,0,0};             //初期値のグループ
    uint64_t gMsk=0b1111;                   //ビットマスク

    hnd = lgGpiochipOpen(PI5);
    //hnd = lgGpiochipOpen(PI4B);
    lgGroupClaimInput(hnd, lFlgIn, SNUM, sw30);         //グループ入力
    lgGroupClaimOutput(hnd,lFlgOut,LNUM,leds,levels);   //グループ出力

    while(1){
        lgGroupRead(hnd, SW0, &bits);       //グループリード
        lgGroupWrite(hnd,LED0,bits,gMsk);   //グループライト
        usleep(1000);                       //CPU使用率の抑制のため
    }
    lgGpiochipClose(hnd);
    return EXIT_SUCCESS;
}
