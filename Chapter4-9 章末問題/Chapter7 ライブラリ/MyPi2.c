// MyPi2.c
// gcc -Wall -c "%f" -llgpio -lpthread -g -O0
// Geanyのオプションの"%f"はソースファイル名のこと

#include <stdio.h>     //printf
#include <stdlib.h>    //EXIT_SUCCESS
#include <string.h>    //strlen
#include <lgpio.h>     //lgI2cWriteByteData
#include "MyPi2.h"     //マイライブラリ

#define PI5    4       // /dev/gpiochip4
#define PI4B   0       // /dev/gpiochip0
//LCD  AQM1602A
#define I2C_BUS    1    // /dev/i2c-1
#define LCD_ADR    0x3e //LCD スレーブアドレス
#define LCD_IR     0x00 //インストラクションレジスタ
#define LCD_DR     0x40 //データレジスタ
/* グローバル変数  */
int g_position = 0;        //文字の位置
int g_line = 0;            //行の位置
int g_charsPerLine = 16;   //1行の最大文字数
int g_dispLines = 2;       //LCDの行数

//関数名   int LcdSetup(int hndLcd)
//引  数   hndLcd lgI2cOpen関数で取得したハンドル
//戻り値   lgI2cWriteByteData関数の戻り値
//概  要   LCDの初期化
int LcdSetup(int hndLcd){
    int i,err;
    char lcdCmd1[6]={0x38,0x39,0x14,0x73,0x56,0x6c}; //データシートより
    char lcdCmd2[3]={0x38,0x01,0x0c};                //データシートより

    for (i=0;i<6;i++){
        err = lgI2cWriteByteData(hndLcd,LCD_IR,lcdCmd1[i]);
        lguSleep(0.02);     //20ms時間待ち
    }
    lguSleep(0.2);          //200ms時間待ち

    for (i=0;i<3;i++){
        err = lgI2cWriteByteData(hndLcd,LCD_IR,lcdCmd2[i]);
        lguSleep(0.02);     //20ms時間待ち
    }
    lguSleep(0.2);  //安定用
    return err;
}

//関数名   int LcdWriteChar(int hndLcd, char c)
//引  数   hndLcd lgI2cOpen関数で取得したハンドル
//引  数   c 1文字
//戻り値   lgI2cWriteByteData関数の戻り値
//概  要   1文字の表示
int LcdWriteChar(int hndLcd, char c){
    int err;
    if( c < 0x06) return EXIT_FAILURE;  //LCDの非表示文字コードの排除
    //行の最大文字数を超えたかの判定
    if( g_position==(g_charsPerLine*(g_line+1))){
        LcdNewline(hndLcd);        //改行処理
    }
    err = lgI2cWriteByteData(hndLcd,LCD_DR,c);
//  printf("%c",c);fflush;  //debug用
    g_position +=1;
    lguSleep(0.001);        //1ms時間待ち
    return err;
}

//関数名   int LcdNewline(int hndLcd)
//引  数   hndLcd lgI2cOpen関数で取得したハンドル
//戻り値   lgI2cWriteByteData関数の戻り値
//概  要   LCD改行処理
int LcdNewline(int hndLcd){
    int err;
    if (g_line == (g_dispLines-1)){
        LcdClear(hndLcd);
    }else{
        g_line +=1;
        err = lgI2cWriteByteData(hndLcd,LCD_IR,0xc0);
        lguSleep(0.01);     //10ms時間待ち
    }
    return err;
}

//関数名   int LcdClear(int hndLcd)
//引  数   hndLcd lgI2cOpen関数で取得したハンドル
//戻り値   lgI2cWriteByteData関数の戻り値
//概  要   LCDのディスプレイクリア
int LcdClear(int hndLcd){
    int err;
    err = lgI2cWriteByteData(hndLcd,LCD_IR,0x01);
    g_position =0;
    g_line=0;
    lguSleep(0.01);     //10ms時間待ち
    return err;
}

//関数名   int LcdWriteString(int hndLcd, char *s)
//引  数   hndLcd lgI2cOpen関数で取得したハンドル
//引  数   s 文字列へのポインタ
//戻り値   lgI2cWriteByteData関数の戻り値
//概  要   LCDに文字列を表示
int LcdWriteString(int hndLcd, char *s){
    int i,err;
    for(i=0;i<strlen(s);i++){
        err = LcdWriteChar(hndLcd, s[i]);
//      putchar(s[i]);  //debug用
    lguSleep(0.001);        //1ms間待ち
    }
    return err;
}
