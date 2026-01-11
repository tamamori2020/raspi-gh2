// MyPi2.c
// gcc -Wall -c "%f" -llgpio -lpthread -g -O0
// Geanyのオプションの"%f"はソースファイル名のこと

#include <stdio.h>      //printf
#include <stdlib.h>     //EXIT_SUCCESS
#include <string.h>     //strlen
#include <lgpio.h>      //lgI2cWriteByteData
#include "MyPi2.h"      //マイライブラリ

#define PI5     4       // /dev/gpiochip4
#define PI4B    0       // /dev/gpiochip0

//LCD  AQM1602A
#define I2C_BUS    1    // /dev/i2c-1
#define LCD_ADR    0x3e //LCD スレーブアドレス
#define LCD_IR     0x00 //インストラクションレジスタ
#define LCD_DR     0x40 //データレジスタ

//温度センサ  ADT7410
#define ADT7410_ADR 0x48 //温度センサのスレーブアドレス
#define ADT7410_REG 0    //温度レジスタ

//D/Aコンバータ  MCP4922
#define MCP4922_DACA    0           //DACAチャンネル
#define MCP4922_DACB    0b10000000  //DACBチャンネル
#define MCP4922_GAx1    0b00100000  //ゲイン1倍
#define MCP4922_SHDN    0b00010000  //VOUTの出力を有効

//A/Dコンバータ MCP3802
//マクロ定義なし

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

//関数名   double Adt7410Read(int hndTmp)
//引  数   hndTmp lgI2cOpen関数で取得したハンドル
//戻り値   double型温度データ（単位℃）
//概  要   ADT7410温度センサによる温度データの取得
double Adt7410Read(int hndTmp){
    short wdat;                         //温度データ
    double temp;                        //温度 実数値℃
    const float tempDelta = 0.0625F;    //温度の分解能 0.0625℃
    wdat = lgI2cReadWordData(hndTmp, ADT7410_REG);  //温度センサよりデータを取得
    wdat = (wdat&0xff00)>>8 | (wdat&0xff)<<8;       //上位と下位のバイトを交換する
    wdat = wdat >>3;                                //13bitフォーマット形式
    if ((wdat&0x1000)==0){          //温度の正負を判断
        temp = wdat*tempDelta;     //プラス温度の計算
    }else{
        temp = ((~wdat&0x1fff)+1)*(-tempDelta); //マイナス温度の計算
    }
    return temp;
}

//関数名  int Mcp4922Write(int hndDac,unsigned char dacCh, unsigned short dacCode)
//引  数  hndDac lgSpiOpen関数で取得したハンドル
//引  数  dacCh DACチャンネル DACA=0, DACB=0b10000000
//引  数  dacCode 12bit DACコード 0000-0fff
//戻り値  lgSpiWrite関数の戻り値
//概  要  0Vから約3.3Vの範囲で、12bit分解能でアナログ電圧を出力する
int Mcp4922Write(int hndDac,unsigned char dacCh, unsigned short dacCode)
{
    int ret;
    char txBuf[2]={0,0};
    if(dacCh == MCP4922_DACB){    //DACBチャンネルを指定
        txBuf[0] |= MCP4922_DACB;
    }
    txBuf[0] = txBuf[0]|MCP4922_GAx1|MCP4922_SHDN; //config bitをセット
    txBuf[0]|= dacCode>>8;        //12bit DACコードの上位4ビットをセット
    txBuf[1] = dacCode&0x00ff;    //12bit DACコードの下位8ビットをセット
    ret = lgSpiWrite(hndDac, txBuf, sizeof(txBuf)); //SPIバスへ出力
    return ret;
}

//関数名  unsigned short Mcp3208RW(int hndAdc, unsigned char adcCh)
//引  数  hndAdc lgSpiOpen関数で取得したハンドル
//引  数  adcCh ADCチャンネル 0から7
//戻り値  12bit ADCコード 0000-0fff
//概  要  0Vから約3.3Vの範囲のアナログ電圧を12bit分解能でディジタル値に変換
unsigned short Mcp3208RW(int hndAdc, unsigned char adcCh)
{
    unsigned short adcCode = 0;
    char txBuf[3]={0,0,0};
    char rxBuf[3];
    txBuf[0] =0b00000110|(adcCh>>2);    // 0,0,0,0,0,ST,SGL,D2
    txBuf[1]=adcCh<<6;                  // D1,D0,0,0,0,0,0,0
    lgSpiXfer(hndAdc, txBuf, rxBuf, sizeof(rxBuf));
    adcCode =(((rxBuf[1]&0x0f) << 8) | rxBuf[2]); // 0000+MSB----------LSB
    return adcCode;
}

