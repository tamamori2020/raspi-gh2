// 8-Ex02.c
// gcc -Wall -o "%e" "%f" -llgpio -lpthread -g -O0 libMyPi2.a
// Geanyのオプションの"%e"は実行ファイル名, "%f"はソースファイル名のこと

#include <stdio.h>      //printf
#include <stdlib.h>     //EXIT_SUCCESS
#include <lgpio.h>      //lgSpiOpen,lgSpiXfer,etc
#include "MyPi2.h"      //マイライブラリ

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
    unsigned short adcCode = 0;
    hndAdc = lgSpiOpen(SPI_BUS, SPI_SS0, SPI_SPEED, SPI_MODE); //ADCのハンドルを取得

    while(1){
        adcCode = Mcp3208RW(hndAdc, MCP3208_CH7);
        printf("CH7 = %3XH", adcCode);
        printf("\t電圧 = %5.3f V\n", ((3.3/4096) * adcCode));
        lguSleep(0.5);
    }
    lgSpiClose(hndAdc);
    return EXIT_SUCCESS;
}
