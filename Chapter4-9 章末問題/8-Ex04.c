// 8-Ex04.c
// gcc -Wall -o "%e" "%f" -llgpio -lpthread -g -O0 libMyPi2.a
// Geanyのオプションの"%e"は実行ファイル名, "%f"はソースファイル名のこと

#include <stdio.h>      //printf
#include <stdlib.h>     //EXIT_SUCCESS
#include <lgpio.h>      //lgSpiOpen,lgSpiXfer,etc
#include "MyPi2.h"      //マイライブラリ

//I2Cバスインタフェース
#define I2C_BUS    1        // /dev/i2c-1
#define LCD_ADR    0x3e     //LCD スレーブアドレス

//SPIインタフェース
#define SPI_BUS     0       // /dev/spidev0
#define SPI_SS0     0       //ADCのスレーブセレクト番号
#define SPI_SPEED   500000  //クロック信号の周波数
#define SPI_MODE    0       //SPIモード0

//A/Dコンバータ MCP3208
#define MCP3208_CH0 0       // CH0入力
#define MCP3208_CH1 1       // CH1入力
#define MCP3208_CH2 2       // CH2入力
#define MCP3208_CH3 3       // CH3入力
#define MCP3208_CH4 4       // CH4入力
#define MCP3208_CH5 5       // CH5入力
#define MCP3208_CH6 6       // CH6入力
#define MCP3208_CH7 7       // CH7入力

int main(void)
{
    int hndAdc;
    unsigned short adcCode;
    int hndLcd,err;
    char s1[17];                //LCD16文字+ヌル文字
    hndLcd = lgI2cOpen(I2C_BUS,LCD_ADR,0);     //LCDのハンドルを取得
    err=LcdSetup(hndLcd);                      //LCDの初期化
    if(err<0){
        fprintf(stderr,"%s(%d).\n",lguErrorText(err),err);      //エラーメッセージの表示
        lgI2cClose(hndLcd);
        return EXIT_FAILURE;
    }
    hndAdc = lgSpiOpen(SPI_BUS, SPI_SS0, SPI_SPEED, SPI_MODE);  //ADCのハンドルを取得

    while(1){
        adcCode = Mcp3208RW(hndAdc, MCP3208_CH7);               //A/D変換 CH7の入力
        sprintf(s1,"CH7=%3XH %5.3fV",adcCode, ((3.3/4096) * adcCode));  //実数を文字列に変換
        LcdClear(hndLcd);               //画面クリア
        LcdWriteString(hndLcd, s1);     //LCDに電圧を表示
        printf("%s\n",s1);              //ターミナルにデバック表示
        lguSleep(0.5);
    }
    lgI2cClose(hndLcd);
    lgSpiClose(hndAdc);
    return EXIT_SUCCESS;
}
