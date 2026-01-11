// 6-Ref01.c
// gcc -Wall -o "%e" "%f" -llgpio -lpthread -g -O0
// Geanyのオプションの"%e"は実行ファイル名, "%f"はソースファイル名のこと

#include <stdio.h>      //printf
#include <stdlib.h>     //EXIT_SUCCESS
#include <unistd.h>     //usleep
#include <lgpio.h>      //lguTimestamp,etc
#define PI5     4       // /dev/gpiochip4
#define PI4B    0       // /dev/gpiochip0
#define LED0    23      //GPIO23をLED0と定義
#define SW0     4       //GPIO4をSW0と定義
#define DELAYTIME 20    //バウンシング対策時間(ms)　プログラムを実行してt0Difの値から調整する

/* グローバル変数 */
int g_hnd;                  //handle
static volatile int g_alt0; //0または1のオルタネート動作用

/* プロトタイプ宣言 */
void CbfSw0(int e, lgGpioAlert_p evt, void *userdata);  //SW0の割込み処理の関数

int main (void){
    int g_hnd;
    int lFlgOut=0;
    int lFlgIn=LG_SET_PULL_DOWN;    //プルダウン抵抗を有効
    int userdata;
    g_hnd = lgGpiochipOpen(PI5);
    //g_hnd = lgGpiochipOpen(PI4B);
    lgGpioClaimOutput(g_hnd,lFlgOut,LED0,LG_LOW);
    lgGpioSetAlertsFunc(g_hnd,SW0,CbfSw0, &userdata);   //SW0の割込み処理関数の設定
    lgGpioClaimAlert(g_hnd,lFlgIn,LG_BOTH_EDGES,SW0,-1); //SW0の両方エッジで割込み発生
    g_alt0=0;                           //オルタネート動作用変数
    
    while(1){
        lgGpioWrite(g_hnd,LED0,g_alt0); //g_alt0をLED0に出力
        usleep(1000);                   //CPU使用率の抑制のため
    }
    lgGpiochipClose(g_hnd);
    return EXIT_SUCCESS;
}

/*****************************************************
 関数名  void CbfSw0(int e, lgGpioAlert_p evt, void *userdata)
 引  数  e            使用せず
       evt          使用せず
       userdata     使用せず
 戻り値  なし
 概  要  SW0のバウンシングを除去し、オルタネート動作を実現する
 *****************************************************/
void CbfSw0(int e, lgGpioAlert_p evt, void *userdata)
{
    static uint64_t t0Now=0;    //現在のタイムスタンプ(ms)
    static uint64_t t0Pre=0;    //一つ前のタイムスタンプ(ms)
    uint64_t t0Dif;             //t0Nowとt0Preとの経過時間
    static int sw0A=0;          //バウンスを除去したSW0の信号

    t0Pre = t0Now;              //t0Preの更新
    t0Now = lguTimestamp()/1000000;  //タイムスタンプの取得、ナノ秒からミリ秒に変換
    t0Dif = t0Now - t0Pre;      //パルス幅の時間を計算
    //printf("t0Now=%ld, t0Pre=%ld, t0Dif=%ld\n",t0Now,t0Pre,t0Dif);  //デバック用
    if(t0Dif > DELAYTIME){      //DELAYTIMEより大きいときだけ、
        sw0A = !sw0A;           //sw0Aをオルタネート動作させる
        if(sw0A == LG_HIGH){    //sw0AがHのとき、g_alt0を反転させる
            g_alt0 = !g_alt0;   //g_alt0を論理否定
        }
        printf("sw0A=%d, g_alt0=%d \n\n",sw0A,g_alt0);    //デバック用
    }
}

