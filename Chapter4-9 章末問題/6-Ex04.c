// 6_Ex04.c 
// gcc -Wall -o "%e" "%f" -llgpio -lpthread -g -O0
// Geanyのオプションの"%e"は実行ファイル名, "%f"はソースファイル名のこと

#include <stdio.h>      //printf
#include <stdlib.h>     //EXIT_SUCCESS
#include <unistd.h>     //usleep
#include <lgpio.h>      //lgTxPulse,etc

#define PI5     4       // /dev/gpiochip4
#define PI4B    0       // /dev/gpiochip0
#define BUZZER  18      //圧電サウンダ
#define SW0     4       //SW0からSW7までのGPIO番号の定義
#define SW1     5
#define SW2     6
#define SW3     26
#define SW4     17
#define SW5     27
#define SW6     20
#define SW7     21
#define SNUM    8       //SWの数

//半周期(us)
#define DO1     1911    //低いド SW7
#define RE      1703    //レ	    SW6
#define MI      1517    //ミ	    SW5
#define FA      1432    //ファ   SW4
#define SO      1276    //ソ	    SW3
#define RA      1136    //ラ	    SW2
#define SI      1012    //シ	    SW1
#define DO2      952    //高いド SW0

int main (void){
    int hnd;
    int lFlgOut=0;                  //出力のオプション
    int lFlgIn=LG_SET_PULL_DOWN;    //プルダウン抵抗を有効
    int sw70[SNUM] = {SW0,SW1,SW2,SW3,SW4,SW5,SW6,SW7}; //スイッチのグループ
    int doremi = 0;                 //音名
    uint64_t bits;                  //グループデータ
    hnd = lgGpiochipOpen(PI5);
    //hnd = lgGpiochipOpen(PI4B);
    lgGpioClaimOutput(hnd,lFlgOut,BUZZER,LG_LOW);   //BUZZERを出力に設定
    lgGroupClaimInput(hnd, lFlgIn, SNUM, sw70);     //SW0-SW7を入力に設定

    while(1){
        lgGroupRead(hnd, SW0, &bits);
        //printf("bits = %ld\n",bits);    //デバック用
        switch(bits){
            case 128:           //SW7
                doremi = DO1;   //ド
                break;
            case 64:            //SW6
                doremi = RE;    //レ
                break;
            case 32:            //SW5
                doremi = MI;    //ミ
                break;
            case 16:            //SW4
                doremi = FA;    //ファ
                break;
            case 8:             //SW3
                doremi = SO;    //ソ
                break;
            case 4:             //SW2
                doremi = RA;    //ラ
                break;
            case 2:             //SW1
                doremi = SI;    //シ
                break;
            case 1:             //SW0
                doremi = DO2;   //ド
                break;
            default:            //スイッチが押されていない時
                doremi = 0;     //パルス信号の停止、音を鳴らさない
                break;
        }
        lgTxPulse(hnd, BUZZER, doremi, doremi,0,0);
        usleep(1000);           //CPU使用率の抑制のため
    }
    lgGpiochipClose(hnd);
    return EXIT_SUCCESS;
}
