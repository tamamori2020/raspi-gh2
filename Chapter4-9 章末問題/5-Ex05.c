// 5-Ex05.c
// gcc -Wall -o "%e" "%f" -llgpio -lpthread -g -O0
// Geanyのオプションの"%e"は実行ファイル名, "%f"はソースファイル名のこと

#include <stdio.h>
#include <stdlib.h>     //EXIT_SUCCESS
#include <unistd.h>     //usleep
#include <lgpio.h>      //lgGpioSetAlertsFunc,lgGpioClaimAlert,etc
#define PI5     4       // /dev/gpiochip4
#define PI4B    0       // /dev/gpiochip0
#define LED0    23      //GPIO23をLED0と定義
#define SW0     4       //GPIO4をSW0と定義
#define SW1     5       //GPIO5をSW1と定義
/* プロトタイプ宣言 */
void CbfSw0(int e, lgGpioAlert_p evt, void *userdata);  //SW0の割込み処理の関数
void CbfSw1(int e, lgGpioAlert_p evt, void *userdata);  //SW1の割込み処理の関数
/* グローバル変数 */
int g_state;
int main (void){
    int hnd;
    int lFlgOut=0;
    int lFlgIn = LG_SET_PULL_DOWN;      //プルダウン抵抗を有効
    int userdata;
    hnd = lgGpiochipOpen(PI5);
    //hnd = lgGpiochipOpen(PI4B);
    lgGpioClaimOutput(hnd,lFlgOut,LED0,LG_LOW);
    lgGpioSetAlertsFunc(hnd,SW0,CbfSw0, &userdata);       //SW0の割込み処理関数の設定
    lgGpioSetAlertsFunc(hnd,SW1,CbfSw1, &userdata);       //SW1の割込み処理関数の設定
    lgGpioClaimAlert(hnd,lFlgIn,LG_RISING_EDGE,SW0,-1);   //SW0の立ち上がりエッジの設定
    lgGpioClaimAlert(hnd,lFlgIn,LG_RISING_EDGE,SW1,-1);   //SW1の立ち上がりエッジの設定
    while(1){
        if(g_state){
            lgGpioWrite(hnd,LED0,LG_HIGH);
            lguSleep(1);        //1秒時間待ち
            lgGpioWrite(hnd,LED0,LG_LOW);
            lguSleep(1);        //1秒時間待ち
        }
        usleep(1000);           //CPU使用率の抑制のため
    }
    lgGpiochipClose(hnd);
    return EXIT_SUCCESS;
}

void CbfSw0(int e, lgGpioAlert_p evt, void *userdata)   //SW0の割込み処理
{
    g_state = LG_HIGH;
}

void CbfSw1(int e, lgGpioAlert_p evt, void *userdata)   //SW1の割込み処理
{
    g_state = LG_LOW;
}

