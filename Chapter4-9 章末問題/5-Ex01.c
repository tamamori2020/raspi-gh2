// 5-Ex01.c
// gcc -Wall -o "%e" "%f" -llgpio -lpthread -g -O0
// Geanyのオプションの"%e"は実行ファイル名, "%f"はソースファイル名のこと

#include <stdio.h>      //printf
#include <stdlib.h>     //EXIT_SUCCESS
#include <lgpio.h>      //lgGpiochipOpen,lgGpioRead,etc
#define PI5    4        // /dev/gpiochip4
#define PI4B   0        // /dev/gpiochip0
#define LED0   23       //GPIO23をLED0と定義
#define SW0    4        //GPIO4をSW0と定義
#define COUNT  3        //3秒

int main (void){
    int hnd;
    int lFlgOut=0;                  //出力のオプション
    int lFlgIn=LG_SET_PULL_DOWN;    //プルダウン抵抗の有効（入力のオプション）
    int sw0Stat;                    //SW0のステータス
    int sw0Cnt=0;                   //秒のカウンター

    hnd = lgGpiochipOpen(PI5);
    //hnd = lgGpiochipOpen(PI4B);
    lgGpioClaimOutput(hnd,lFlgOut,LED0,LG_LOW); //LED0を出力に設定
    lgGpioClaimInput(hnd,lFlgIn,SW0);           //SW0を入力に設定

    while(sw0Cnt<COUNT){    //3秒未満のときは1秒ごとにSW0の値をリード
        sw0Stat=lgGpioRead(hnd,SW0);
        if(sw0Stat==LG_HIGH){
            sw0Cnt++;       //長押しの検出
            printf("%d\n",sw0Cnt);  //デバッグ用
        }else{
            sw0Cnt=0;       //カウンターのクリア
        }
        lguSleep(1);        //1秒時間待ち
    }

    lgGpioWrite(hnd,LED0,LG_HIGH);    //LED0の3秒間の点灯
    lguSleep(COUNT);
    lgGpioWrite(hnd,LED0,LG_LOW);
    lgGpiochipClose(hnd);
    return EXIT_SUCCESS;
}
