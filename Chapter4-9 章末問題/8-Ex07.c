// 8-Ex07.c
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
    FILE *fp;                   //ファイルポインタ
    int hnd,hndLcd,hndAdc,err;
    char s1[17];                //LCD16文字+ヌル文字
    int lFlgOut=0;
    int lFlgIn=LG_SET_PULL_DOWN;//プルダウン抵抗を有効
    int userdata;
    unsigned short adcCode;     //ADCコード

    time_t now;
    char fileName[32];
    char timeStamp[16];         //時刻
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

    hndLcd = lgI2cOpen(I2C_BUS,LCD_ADR,0);              //LCDのハンドルを取得
    err=LcdSetup(hndLcd);                               //LCDの初期化
    if(err<0){
        fprintf(stderr,"%s(%d).\n",lguErrorText(err),err);   //エラーメッセージの表示
        lgI2cClose(hndLcd);
        return EXIT_FAILURE;
    }

    hndAdc = lgSpiOpen(SPI_BUS, SPI_SS0, SPI_SPEED, SPI_MODE); //ADCのハンドルを取得

    g_state = STANDBY;
    LcdWriteString(hndLcd,"Push SW0");

    while(1){
        if(g_state == START){
            lgGpioWrite(hnd,LED0,LG_HIGH);
            now = time(NULL);
            strftime(timeStamp,sizeof(timeStamp),"%T",localtime(&now));  //"%T"="%H:%M:%S"
            adcCode = Mcp3208RW(hndAdc, MCP3208_CH7);
            sprintf(s1,"%5.3f",((3.3/4096) * adcCode)); //実数を文字列に変換
            LcdClear(hndLcd);                           //画面クリア
            LcdWriteString(hndLcd,timeStamp);           //LCDの１行目に時刻を表示
            LcdNewline(hndLcd);                         //LCDの改行
            LcdWriteString(hndLcd,s1);                  //LCDの２行目に電圧を表示
            LcdWriteString(hndLcd," V");                //単位の表示
            fprintf(fp,"%s,%s\n",timeStamp,s1);         //ファイルに記録
            #ifdef DEBUG
                    printf("%s, %s V\n",timeStamp,s1);  //ターミナルにデバック表示
            #endif
            lguSleep(1);;                   //待ち時間1秒
        }else if (g_state == STOP){
            lgGpioWrite(hnd,LED0,LG_LOW);
            fclose(fp);                     //ファイルのクローズ
            break;                          //whileを抜ける
        }
    }

    lgI2cClose(hndLcd);
    lgSpiClose(hndAdc);
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
