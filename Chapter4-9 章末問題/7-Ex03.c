// 7-Ex03.c
// gcc -Wall -o "%e" "%f" -llgpio -lpthread -g -O0 libMyPi2.a
// Geanyのオプションの"%e"は実行ファイル名, "%f"はソースファイル名のこと

#include <stdio.h>          //printf,etc
#include <stdlib.h>         //EXIT_SUCCESS
#include <unistd.h>         //usleep
#include <time.h>           //time,strftime,localtime
#include <lgpio.h>          //lgI2cReadWordData,etc
#include "MyPi2.h"          //マイライブラリ
#define PI5         4       // /dev/gpiochip4
#define PI4B        0       // /dev/gpiochip0
#define SW0         4       //GPIO4をSW0と定義
#define SW1         5       //GPIO5をSW1と定義
#define LED0        23      //GPIO23をLED0と定義

//I2Cバスインタフェース
#define I2C_BUS     1       // /dev/i2c-1
#define LCD_ADR     0x3e    //LCD スレーブアドレス
#define ADT7410_ADR 0x48    //温度センサのスレーブアドレス
#define ADT7410_REG 0       //温度レジスタ

#define STOP        0       //測定停止
#define START       1       //測定開始
#define STANDBY     2       //測定の待機
#define DEBUG               //#ifdef DEBUGから#endifの有効
/* グローバル変数　*/
static volatile int g_state; //SW操作の変数
/* プロトタイプ宣言 */
void CbfSw0(int e, lgGpioAlert_p evt, void *userdata);  //SW0の割込み処理の関数
void CbfSw1(int e, lgGpioAlert_p evt, void *userdata);  //SW1の割込み処理の関数

int main(void){
    int i=0;
    FILE *fp;                   //ファイルポインタ
    int hnd,hndLcd,hndTmp,err;
    char s1[17];                //LCD16文字+ヌル文字
    int lFlgOut=0;
    int lFlgIn=LG_SET_PULL_DOWN;//プルダウン抵抗を有効
    int userdata;
    double temp;                //温度 実数値℃

    time_t now;
    char fileName[32];
    now = time(NULL);
    strftime(fileName, sizeof(fileName), "%Y%m%d%H%M%S.csv",localtime(&now));
    if((fp = fopen (fileName,"w"))==NULL){
        fprintf(stderr,"file open error.\n");
        return EXIT_FAILURE;
    }

    hnd = lgGpiochipOpen(PI5);
    //hnd = lgGpiochipOpen(PI4B);
    lgGpioSetAlertsFunc(hnd,SW0,CbfSw0, &userdata);     //SW0の割込み処理関数の設定
    lgGpioSetAlertsFunc(hnd,SW1,CbfSw1, &userdata);     //SW1の割込み処理関数の設定
    lgGpioClaimAlert(hnd,lFlgIn,LG_RISING_EDGE,SW0,-1); //SW0の立ち上がりエッジの設定
    lgGpioClaimAlert(hnd,lFlgIn,LG_RISING_EDGE,SW1,-1); //SW1の立ち上がりエッジの設定

    lgGpioClaimOutput(hnd,lFlgOut,LED0,LG_LOW);         //LED0の設定

    hndLcd = lgI2cOpen(I2C_BUS,LCD_ADR,0);     //LCDのハンドルを取得
    err=LcdSetup(hndLcd);                      //LCDの初期化
    if(err<0){
        fprintf(stderr,"%s(%d).\n",lguErrorText(err),err);   //エラーメッセージの表示
        lgI2cClose(hndLcd);
        return EXIT_FAILURE;
    }

    hndTmp = lgI2cOpen(I2C_BUS,ADT7410_ADR,0);  //ADT7410のハンドルを取得
    g_state = STANDBY;
    LcdWriteString(hndLcd,"Push SW0");      //LCDにスタートのメッセージを表示

    while(1){
        if(g_state == START){
            i++;
            lgGpioWrite(hnd,LED0,LG_HIGH);
            temp=Adt7410Read(hndTmp);       //温度データの取得
            sprintf(s1,"%d,%4.1f",i,temp);  //摂氏度の値を文字列化
            LcdClear(hndLcd);               //画面クリア
            LcdWriteString(hndLcd, s1);     //LCDに温度を表示
            LcdWriteString(hndLcd, " C");   //単位の表示
            fprintf(fp,"%s\n",s1);          //ファイルに記録
            #ifdef DEBUG
                printf("%s\n",s1);          //ターミナルにデバック表示
            #endif
            lguSleep(0.5);                  //0.5秒待ち時間
        }else if (g_state == STOP){
            lgGpioWrite(hnd,LED0,LG_LOW);
            fclose(fp);                     //ファイルのクローズ
            break;                          //whileを抜ける
        }
        usleep(1000);       //CPU使用率の抑制のため
    }
    lgGpiochipClose(hnd);
    lgI2cClose(hndLcd);
    lgI2cClose(hndTmp);
    return EXIT_SUCCESS;
}

void CbfSw0(int e, lgGpioAlert_p evt, void *userdata)   //SW0の割込み処理
{
    g_state = START;
}

void CbfSw1(int e, lgGpioAlert_p evt, void *userdata)   //SW1の割込み処理
{
    g_state = STOP;
}
