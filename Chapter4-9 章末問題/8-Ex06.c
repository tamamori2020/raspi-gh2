// 8-Ex06.c
// gcc -Wall -o "%e" "%f" -llgpio -lpthread -g -O0 libMyPi2.a
// Geanyのオプションの"%e"は実行ファイル名, "%f"はソースファイル名のこと

#include <stdio.h>      //printf
#include <stdlib.h>     //EXIT_SUCCESS
#include <lgpio.h>      //lgSpiOpen,lgSpiXfer,etc
#include "MyPi2.h"      //マイライブラリ
#define PI5         4       // /dev/gpiochip4
#define PI4B        0       // /dev/gpiochip0
#define LED0        23      //GPIO23をLED0と定義

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

//#define DEBUG               //#ifdef DEBUGから#endifの有効

/* プロトタイプ宣言 */
long map(long value, long fromLow, long fromHigh, long toLow, long toHigh);

int main(void)
{
    int hnd,hndAdc;
    int lFlgOut=0;
    int duty;
    unsigned short adcCode;
    hnd = lgGpiochipOpen(PI5);
    //hnd = lgGpiochipOpen(PI4B);
    lgGpioClaimOutput(hnd,lFlgOut,LED0,LG_LOW);
    hndAdc = lgSpiOpen(SPI_BUS, SPI_SS0, SPI_SPEED, SPI_MODE); //ADCのハンドルを取得

    while(1){
        adcCode = Mcp3208RW(hndAdc, MCP3208_CH7);   //A/D変換 CH7の入力
        duty = map(adcCode,0,0xfff,0,100);          //ADCの値を0~100に変換
        #ifdef DEBUG
             printf("%3XH　%d\n",adcCode, duty);     //デバックによりLEDの点灯がチラつく場合がある
        #endif
        lgTxPwm(hnd,LED0,100.0,duty,0,0);           //デューティ比の設定
        lguSleep(0.1);                              //待ち時間100ms
    }
    lgGpiochipClose(hnd);
    lgSpiClose(hndAdc);
    return EXIT_SUCCESS;
}

/*****************************************************
関数名  long map(long value, long fromLow, long fromHigh, long toLow, long toHigh)
引  数  元の値value、元の範囲の最小値fromLowと最大値fromHigh、新しい範囲の最小値toLowと最大値toHigh
戻り値  新しい範囲に変換された値
概  要  元の範囲の値を新しい範囲の値に変換する。
******************************************************/
long map(long value, long fromLow, long fromHigh, long toLow, long toHigh)
{
    return(value -fromLow)*(toHigh-toLow)/(fromHigh-fromLow)+toLow;
}
