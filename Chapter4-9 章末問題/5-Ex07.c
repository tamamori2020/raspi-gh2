// 5-Ex07.c
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
#define SW2     6       //GPIO6をSW3と定義

int main (void){
    int f,sw[3];
    int hnd;
    int lFlgOut=0;
    int lFlgIn = LG_SET_PULL_DOWN;      //プルダウン抵抗を有効
    hnd = lgGpiochipOpen(PI5);
    //hnd = lgGpiochipOpen(PI4B);
    lgGpioClaimOutput(hnd,lFlgOut,LED0,LG_LOW);
    lgGpioClaimInput(hnd,lFlgIn,SW0);           //SW0を入力に設定
    lgGpioClaimInput(hnd,lFlgIn,SW1);           //SW1を入力に設定
    lgGpioClaimInput(hnd,lFlgIn,SW2);           //SW2を入力に設定

    while(1){
        sw[0]=lgGpioRead(hnd, SW0);
        sw[1]=lgGpioRead(hnd, SW1);
        sw[2]=lgGpioRead(hnd, SW2);

        f = (sw[0]&sw[1])|(sw[1]&sw[2])|(sw[2]&sw[0]);
        lgGpioWrite(hnd,LED0,f);

        //別解
        //f = sw[0]+sw[1]+sw[2];
        //if(1<f){
            //lgGpioWrite(hnd,LED0,LG_HIGH);
        //}else{
            //lgGpioWrite(hnd,LED0,LG_LOW);
        //}

        usleep(1000);               //CPU使用率の抑制のため
    }
    lgGpiochipClose(hnd);
    return EXIT_SUCCESS;
}
