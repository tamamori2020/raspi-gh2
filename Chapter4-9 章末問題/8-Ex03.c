// 8-Ex03.c
// gcc -Wall -o "%e" "%f" -llgpio -lpthread -g -O0 libMyPi2.a
// Geanyのオプションの"%e"は実行ファイル名, "%f"はソースファイル名のこと

#include <stdio.h>      //printf,scanf
#include <stdlib.h>     //EXIT_SUCCESS
#include <lgpio.h>      //lgSpiOpen,lgSpiWrite,etc
#include "MyPi2.h"      //マイライブラリ

//SPIバスインタフェース
#define SPI_BUS     0       // /dev/spidev0
#define SPI_SS0     0       //ADCのスレーブセレクト番号
#define SPI_SS1     1       //DACのスレーブセレクト番号
#define SPI_SPEED   500000  //クロック信号の周波数
#define SPI_MODE    0       //SPIモード0

//D/Aコンバータ  MCP4922
#define MCP4922_DACA    0           //DACAチャンネル
#define MCP4922_DACB    0b10000000  //DACBチャンネル

//A/Dコンバータ MCP3208
#define MCP3208_CH0     0   // CH0入力
#define MCP3208_CH1     1   // CH1入力
#define MCP3208_CH2     2   // CH2入力
#define MCP3208_CH3     3   // CH3入力
#define MCP3208_CH4     4   // CH4入力
#define MCP3208_CH5     5   // CH5入力
#define MCP3208_CH6     6   // CH6入力
#define MCP3208_CH7     7   // CH7入力

int main (void){
    int i;
    int hndDac;
    unsigned int dacCode;
    int hndAdc;
    unsigned short adcCode = 0;
    hndDac = lgSpiOpen(SPI_BUS, SPI_SS1, SPI_SPEED, SPI_MODE); //DACのハンドルを取得
    hndAdc = lgSpiOpen(SPI_BUS, SPI_SS0, SPI_SPEED, SPI_MODE); //ADCのハンドルを取得

    while(1){
        for(i=0;i<=0x1000;i+=0x100){
            dacCode=i;
            if(dacCode != 0){
                dacCode-=1;
            }
            Mcp4922Write(hndDac,MCP4922_DACB,dacCode);          //D/A変換 VOUTBの出力
            printf("DA-VoutB=%3XH  ",dacCode);
            adcCode = Mcp3208RW(hndAdc, MCP3208_CH1);           //A/D変換 CH1の入力
            printf("AD-CH1=%3XH %5.3fV\n", adcCode,adcCode*(3.3/4096));
            lguSleep(0.5);
        }
    }
    lgSpiClose(hndDac);
    lgSpiClose(hndAdc);
    return EXIT_SUCCESS;
}
