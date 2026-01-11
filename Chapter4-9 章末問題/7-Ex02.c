// 7-Ex02.c
// gcc -Wall -o "%e" "%f" -llgpio -lpthread -g -O0 libMyPi2.a
// Geanyのオプションの"%e"は実行ファイル名, "%f"はソースファイル名のこと

#include <stdio.h>          //printf,sprintf,etc
#include <stdlib.h>         //EXIT_SUCCESS
#include <lgpio.h>          //lgI2cReadWordData,etc
#include "MyPi2.h"          //マイライブラリ
#define PI5         4       // /dev/gpiochip4
#define PI4B        0       // /dev/gpiochip0
#define I2C_BUS     1       // /dev/i2c-1
#define LCD_ADR     0x3e    //LCD スレーブアドレス
#define ADT7410_ADR 0x48    //温度センサのスレーブアドレス
#define ADT7410_REG 0       //温度レジスタ
#define SW0         4       //GPIO4をSW0と定義
#define DELAYTIME   20      //バウンシング対策時間(ms)
/* グローバル変数 */
static volatile int g_alt0; //0または1のオルタネート動作用

/* プロトタイプ宣言 */
void CbfSw0(int e, lgGpioAlert_p evt, void *userdata);  //SW0の割込み処理の関数

int main(void){
    int hnd,hndLcd,hndTmp,err; 
    double temp;                        //温度データ
    char s1[17];                        //LCD16文字+ヌル文字
    int lFlgIn=LG_SET_PULL_DOWN;        //プルダウン抵抗を有効
    int userdata;
    
    hnd = lgGpiochipOpen(PI5);
    //hnd = lgGpiochipOpen(PI4B);
    lgGpioSetAlertsFunc(hnd,SW0,CbfSw0, &userdata);   //SW0の割込み処理関数の設定
    lgGpioClaimAlert(hnd,lFlgIn,LG_BOTH_EDGES,SW0,-1); //SW0の両方エッジの設定
    
    hndLcd = lgI2cOpen(I2C_BUS,LCD_ADR,0);     //LCDのハンドルを取得
    err=LcdSetup(hndLcd);                      //LCDの初期化
    if(err<0){
        fprintf(stderr,"%s(%d).\n",lguErrorText(err),err);   //エラーメッセージの表示
        lgI2cClose(hndLcd);
        return EXIT_FAILURE;
    }

    hndTmp = lgI2cOpen(I2C_BUS,ADT7410_ADR,0);  //ADT7410のハンドルを取得
    g_alt0=LG_LOW;
    while(1){
        temp = Adt7410Read(hndTmp);     //温度データの取得
        if(g_alt0 == LG_LOW){
            sprintf(s1,"%4.1f C",temp); //摂氏度を文字列に変換
        }else{
            temp = temp*9.0/5.0+32.0;   //華氏度に変換
            sprintf(s1,"%4.1f F",temp); //華氏度を文字列に変換
        }
        LcdClear(hndLcd);               //LCDクリア
        LcdWriteString(hndLcd, s1);     //LCDに温度を表示
        printf("%s\n",s1);              //ターミナルに表示
        lguSleep(0.5);
    }
    lgI2cClose(hndLcd);
    lgI2cClose(hndTmp);
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
    uint64_t t0Dif;             //t0Nowとt0Preとの時間間隔
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
        //printf("sw0A=%d, g_alt0=%d \n",sw0A,g_alt0);    //デバック用
    }
}
