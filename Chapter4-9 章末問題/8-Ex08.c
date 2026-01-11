// 8-Ex08.c
// gcc -Wall -o "%e" "%f" -llgpio -lpthread -g -O0 libMyPi2.a
// Geanyのオプションの"%e"は実行ファイル名, "%f"はソースファイル名のこと

#include <stdio.h>      //printf,scanf
#include <stdlib.h>     //EXIT_SUCCESS
#include <math.h>       //sin
#include <lgpio.h>      //lgSpiOpen,lgSpiWrite,etc
#include "MyPi2.h"      //マイライブラリ

//SPIバスインタフェース
#define SPI_BUS     0       // /dev/spidev0
#define SPI_SS1     1       //DACのスレーブセレクト番号
#define SPI_SPEED   500000  //クロック信号の周波数
#define SPI_MODE    0       //SPIモード0

//D/Aコンバータ  MCP4922
#define MCP4922_DACA    0           //DACAチャンネル
#define MCP4922_DACB    0b10000000  //DACBチャンネル

//円周率
#define PI      3.141593F   //円周率(単精度)
#define DIV     360         //正弦波信号の360等分

int main (void){
    int i,hndDac;
    unsigned int dacCode;
    const short daDelta=806;        //DACの最小電圧　806uV=3.3V÷4096
    long sinWave;                   //sin波電圧 uV 0Vから3300000uV
    hndDac = lgSpiOpen(SPI_BUS, SPI_SS1, SPI_SPEED, SPI_MODE); //DACのハンドルを取得

    while(1){
        for (i=0;i<DIV;i++){
            sinWave = 1650000*sin(2*PI*i/DIV)+1650000;  //uVの単位として、整数で計算
            dacCode = sinWave/daDelta;                  //12bit(4096)の刻み値からDA変換データを求める
            Mcp4922Write(hndDac,MCP4922_DACA,dacCode);
            #ifdef DEBUG
                printf("%ld %3x\n",sinWave,dacCode);
            #endif
        }
    }
    lgSpiClose(hndDac);
    return EXIT_SUCCESS;
}
