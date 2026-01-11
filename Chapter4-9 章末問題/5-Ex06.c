// 5-Ex06.c
// gcc -Wall -o "%e" "%f" -llgpio -lpthread -g -O0
// Geanyのオプションの"%e"は実行ファイル名, "%f"はソースファイル名のこと

#include <stdio.h>
#include <stdlib.h>     //EXIT_SUCCESS
#include <unistd.h>     //usleep
#include <lgpio.h>      //lgGpioSetAlertsFunc,lgGpioClaimAlert,etc
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
/* プロトタイプ宣言 */
void CbfSw0(int e, lgGpioAlert_p evt, void *userdata);  //SW0の割込み処理の関数
void CbfSw1(int e, lgGpioAlert_p evt, void *userdata);  //SW1の割込み処理の関数
void CbfSw2(int e, lgGpioAlert_p evt, void *userdata);  //SW2の割込み処理の関数
void CbfSw3(int e, lgGpioAlert_p evt, void *userdata);  //SW3の割込み処理の関数
void CbfSw4(int e, lgGpioAlert_p evt, void *userdata);  //SW4の割込み処理の関数
void CbfSw5(int e, lgGpioAlert_p evt, void *userdata);  //SW5の割込み処理の関数
void CbfSw6(int e, lgGpioAlert_p evt, void *userdata);  //SW6の割込み処理の関数
void CbfSw7(int e, lgGpioAlert_p evt, void *userdata);  //SW7の割込み処理の関数
/* グローバル変数 */
int gBits;

int main (void){
    int hnd;
    int userdata;
    int lFlgIn=LG_SET_PULL_DOWN;    //スイッチ用
    int lFlgOut=0;                  //LED用
    int leds[LNUM]={LED0,LED1,LED2,LED3};
    int levels[LNUM]={0,0,0,0};
    uint64_t gMsk=0b1111;
    
    hnd = lgGpiochipOpen(PI5);
    //hnd = lgGpiochipOpen(PI4B);
    lgGroupClaimOutput(hnd,lFlgOut,LNUM,leds,levels);

    lgGpioSetAlertsFunc(hnd,SW0,CbfSw0, &userdata);       //SW0の割込み処理関数の設定
    lgGpioSetAlertsFunc(hnd,SW1,CbfSw1, &userdata);       //SW1の割込み処理関数の設定
    lgGpioSetAlertsFunc(hnd,SW2,CbfSw2, &userdata);       //SW2の割込み処理関数の設定
    lgGpioSetAlertsFunc(hnd,SW3,CbfSw3, &userdata);       //SW3の割込み処理関数の設定
    lgGpioSetAlertsFunc(hnd,SW4,CbfSw4, &userdata);       //SW4の割込み処理関数の設定
    lgGpioSetAlertsFunc(hnd,SW5,CbfSw5, &userdata);       //SW5の割込み処理関数の設定
    lgGpioSetAlertsFunc(hnd,SW6,CbfSw6, &userdata);       //SW6の割込み処理関数の設定
    lgGpioSetAlertsFunc(hnd,SW7,CbfSw7, &userdata);       //SW7の割込み処理関数の設定

    lgGpioClaimAlert(hnd,lFlgIn,LG_RISING_EDGE,SW0,-1);   //SW0の立ち上がりエッジの設定
    lgGpioClaimAlert(hnd,lFlgIn,LG_RISING_EDGE,SW1,-1);   //SW1の立ち上がりエッジの設定
    lgGpioClaimAlert(hnd,lFlgIn,LG_RISING_EDGE,SW2,-1);   //SW2の立ち上がりエッジの設定
    lgGpioClaimAlert(hnd,lFlgIn,LG_RISING_EDGE,SW3,-1);   //SW3の立ち上がりエッジの設定
    lgGpioClaimAlert(hnd,lFlgIn,LG_RISING_EDGE,SW4,-1);   //SW4の立ち上がりエッジの設定
    lgGpioClaimAlert(hnd,lFlgIn,LG_RISING_EDGE,SW5,-1);   //SW5の立ち上がりエッジの設定
    lgGpioClaimAlert(hnd,lFlgIn,LG_RISING_EDGE,SW6,-1);   //SW6の立ち上がりエッジの設定
    lgGpioClaimAlert(hnd,lFlgIn,LG_RISING_EDGE,SW7,-1);   //SW7の立ち上がりエッジの設定

    while(1){
        lgGroupWrite(hnd,LED0,gBits,gMsk);  //値を出力
        usleep(1000);           //CPU使用率の抑制のため
    }
    lgGpiochipClose(hnd);
    return EXIT_SUCCESS;
}

void CbfSw0(int e, lgGpioAlert_p evt, void *userdata)   //SW0の割込み処理
{gBits = 0;}
void CbfSw1(int e, lgGpioAlert_p evt, void *userdata)   //SW1の割込み処理
{gBits = 1;}
void CbfSw2(int e, lgGpioAlert_p evt, void *userdata)   //SW2の割込み処理
{gBits = 2;}
void CbfSw3(int e, lgGpioAlert_p evt, void *userdata)   //SW3の割込み処理
{gBits = 3;}
void CbfSw4(int e, lgGpioAlert_p evt, void *userdata)   //SW4の割込み処理
{gBits = 4;}
void CbfSw5(int e, lgGpioAlert_p evt, void *userdata)   //SW5の割込み処理
{gBits = 5;}
void CbfSw6(int e, lgGpioAlert_p evt, void *userdata)   //SW6の割込み処理
{gBits = 6;}
void CbfSw7(int e, lgGpioAlert_p evt, void *userdata)   //SW7の割込み処理
{gBits = 7;}
