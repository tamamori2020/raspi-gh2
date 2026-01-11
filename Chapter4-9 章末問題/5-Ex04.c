// 5-Ex04.c
// gcc -Wall -o "%e" "%f" -llgpio -lpthread -g -O0
// Geanyのオプションの"%e"は実行ファイル名, "%f"はソースファイル名のこと

#include <stdio.h>      //printf
#include <stdlib.h>     //EXIT_SUCCESS
#include <unistd.h>     //usleep
#include <lgpio.h>      //lgGpiochipOpen,lgGroupRead,etc
#define PI5     4       // /dev/gpiochip4
#define PI4B    0       // /dev/gpiochip0
#define LED0    23      //GPIO23をLED0と定義
#define SW0     4       //GPIO4をSW0と定義
#define SW1     5       //GPIO5をSW1と定義

int main (void){
    int hnd;
    int lFlgOut=0;
    int lFlgIn = LG_SET_PULL_DOWN;      //プルダウン抵抗を有効
    hnd = lgGpiochipOpen(PI5);
    //hnd = lgGpiochipOpen(PI4B);
    lgGpioClaimOutput(hnd,lFlgOut,LED0,LG_LOW);
    lgGpioClaimInput(hnd,lFlgIn,SW0);           //SW0を入力に設定
    lgGpioClaimInput(hnd,lFlgIn,SW1);           //SW1を入力に設定

    while(1){
        if(lgGpioRead(hnd, SW0)==LG_HIGH){         //SW0のチェック
            while(1){
                lgGpioWrite(hnd,LED0,LG_HIGH);
                lguSleep(1);        //1秒時間待ち
                lgGpioWrite(hnd,LED0,LG_LOW);
                lguSleep(1);        //1秒時間待ち
                if(lgGpioRead(hnd, SW1)==LG_HIGH){ //SW1のチェック
                    break;
                }
            }
        }
        usleep(1000);               //CPU使用率の抑制のため
    }
    lgGpiochipClose(hnd);
    return EXIT_SUCCESS;
}
