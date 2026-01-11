// 8-Ex01.c
// gcc -Wall -o "%e" "%f" -llgpio -lpthread -g -O0 libMyPi2.a
// Geanyのオプションの"%e"は実行ファイル名, "%f"はソースファイル名のこと

#include <stdio.h>      //printf,scanf
#include <stdlib.h>     //EXIT_SUCCESS
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

int main (void){
    int hndDac;
    unsigned int dacCode;
    hndDac = lgSpiOpen(SPI_BUS, SPI_SS1, SPI_SPEED, SPI_MODE); //DACのハンドルを取得

    while(1){
        printf("16進数で0からfffまでの値を入力してください >>>");
        scanf("%x",&dacCode);
        if(0<=dacCode && dacCode <=0xfff){
            printf("VoutA,VoutB = %5.3f V\n\n", ((3.3/4096) * dacCode));    //電圧の表示
            Mcp4922Write(hndDac,MCP4922_DACA,dacCode);
            Mcp4922Write(hndDac,MCP4922_DACB,dacCode);
        }else{
            printf("値が範囲外です\n\n");
            Mcp4922Write(hndDac,MCP4922_DACA,0);
            Mcp4922Write(hndDac,MCP4922_DACB,0);
        }
    }
    lgSpiClose(hndDac);
    return EXIT_SUCCESS;
}
