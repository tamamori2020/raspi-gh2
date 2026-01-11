// 7-Ex01.c
// gcc -Wall -o "%e" "%f" -llgpio -lpthread -g -O0 libMyPi2.a
// Geanyのオプションの"%e"は実行ファイル名, "%f"はソースファイル名のこと

#include <stdio.h>      //printf,sprintf,etc
#include <stdlib.h>     //EXIT_SUCCESS
#include <lgpio.h>      //lgI2cReadWordData,etc
#include "MyPi2.h"      //マイライブラリ

#define I2C_BUS     1       // /dev/i2c-1
#define LCD_ADR     0x3e    //LCD スレーブアドレス
#define ADT7410_ADR 0x48    //温度センサのスレーブアドレス
#define ADT7410_REG 0       //温度レジスタ

int main(void){
    int hndLcd,hndTmp,err; 
    double temp;                        //温度 実数値℃
    char s1[17];                        //LCD16文字+ヌル文字
    hndLcd = lgI2cOpen(I2C_BUS,LCD_ADR,0);     //LCDのハンドルを取得
    err=LcdSetup(hndLcd);                      //LCDの初期化
    if(err<0){
        fprintf(stderr,"%s(%d).\n",lguErrorText(err),err);   //エラーメッセージの表示
        lgI2cClose(hndLcd);
        return EXIT_FAILURE;
    }

    hndTmp = lgI2cOpen(I2C_BUS,ADT7410_ADR,0);  //ADT7410のハンドルを取得

    while(1){
        temp = Adt7410Read(hndTmp);     //温度測定関数
        printf("%4.1f C\n",temp);       //ターミナルに温度を表示
        sprintf(s1,"%4.1f C",temp);     //実数を文字列に変換
        LcdClear(hndLcd);               //LCDクリア
        LcdWriteString(hndLcd, s1);     //LCDに温度を表示
        lguSleep(0.5);
    }
    lgI2cClose(hndLcd);
    lgI2cClose(hndTmp);
    return EXIT_SUCCESS;
}
