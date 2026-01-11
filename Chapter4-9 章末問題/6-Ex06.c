// 6-Ex06.c
// gcc -Wall -o "%e" "%f" -llgpio -lpthread -g -O0 -lm
// Geanyのオプションの"%e"は実行ファイル名, "%f"はソースファイル名のこと

#include <stdio.h>      //printf
#include <stdlib.h>     //EXIT_SUCCESS
#include <unistd.h>     //usleep
#include <math.h>       //pow
#include <lgpio.h>      //lgTxPulse,etc
#include "doremi.h"     //楽譜
//#include "sakura.h"     //楽譜
#define PI5     4       // /dev/gpiochip4
#define PI4B    0       // /dev/gpiochip0
#define BUZZER  18
#define SW0     4
#define SW1     5

//#define  DEBUG    // マクロ定義により#ifdef #endifで挟まれた命令が有効になります。
                    // 命令を無効にする場合は、「#define  DEBUG」をコメント文にしてビルドします。
/* グローバル変数　*/
int g_sw1;          //SW1操作の変数

/* プロトタイプの定義 */
void g(int hnd, int oct, int solfa, int length); //1音を発生する関数
void CbfSw1(int e, lgGpioAlert_p evt, void *userdata);  //SW1の割込み関数

int main (void){
    int i;
    int oct,solfa;
    int qN = 1000; //4分音符(quarter note) の長さ(ミリ秒)
    int musicNo;
    int hnd;
    int lFlgOut=0;
    int lFlgIn = LG_SET_PULL_DOWN;
    int userdata;
    hnd = lgGpiochipOpen(PI5);
    //hnd = lgGpiochipOpen(PI4B);
    lgGpioClaimOutput(hnd,lFlgOut,BUZZER,LG_LOW);       //BUZZERを出力に設定
    lgGpioClaimInput(hnd,lFlgIn,SW0);                   //SW0を入力に設定
    lgGpioSetAlertsFunc(hnd,SW1,CbfSw1, &userdata);     //SW1の割り込み処理関数CbfSw1を設定
    lgGpioClaimAlert(hnd,lFlgIn,LG_RISING_EDGE,SW1,-1); //立ち上がりエッジで割り込み発生

    musicNo = sizeof music/sizeof music[0];    //配列の要素数(音符)の計算
    #ifdef DEBUG
        printf("musicNo = %d\n",musicNo);
    #endif

    while(1){
        if(lgGpioRead(hnd, SW0) == LG_HIGH){ //オルゴールスタート
            g_sw1 = 0;
            for(i=0; i<musicNo; i++){   //音符の最初から最後まで再生
    #ifdef DEBUG
                printf("%d  ",i+1);
    #endif
                if(g_sw1 == 1){         //SW1が押されたら再生停止
                    g(hnd,OC,REST,qN);  //音の出力を停止
                    break;
                }
                oct = music[i][0];      //オクターブの取得
                solfa =  music[i][1];   //階名の取得
                qN = music[i][2];       //音の長さの取得
                g(hnd, oct,solfa,qN);   //1音を再生
            }
        }
        usleep(1);              //CPU使用率の抑制のため
    }
    return EXIT_SUCCESS;
}

/*****************************************************
 関数名  void g(int hnd, int oct, int solfa, int length)
 引  数  hnd     lgTxPulse用ハンドル
       oct     3:1オクターブ下,4:中央,5:1オクターブ上
       solfa   0:休符,1:ド,2:ド#,3:レ,4:レ#,5:ミ,6:ファ,
               7:ファ#,8:ソ,9:ソ#,10:ラ,11:ラ#,12:シ
       length　音の長さ 単位ms
 戻り値  なし
 概  要  引数で指定された音名または休符を発生する。
 *****************************************************/
void g(int hnd, int oct, int solfa, int length){
    int d = 0;      //指数 式6-1
    int f = 0;      //周波数(Hz)
    int hlfT;       //半周期(us)
    if(solfa != REST){          //休符以外の場合、音名の周波数を計算
        d = solfa - 10;         //引数solfaから式6-1の変数dに変換する
        if (oct == OL){ d -= 12;}   //1オクターブ下のとき
        if (oct == OH){ d += 12;}   //1オクターブ上のとき
        f = 440 * pow(2,(double)d/12)+0.5;  //式6-1から周波数fを計算
        hlfT = 1.0/f*pow(10,6)/2.0;        //周波数fから半周期(us)を計算
    }else{
        hlfT = 0;          //休符、valueを0にして、音を止めます
    }
    #ifdef DEBUG
        printf("d = %d, f = %d, hlfT = %d \n",d,f,hlfT);
    #endif
    lgTxPulse(hnd,BUZZER,hlfT,hlfT,0,0);
    usleep(length*1000);        //マイクロ秒に変換
}

/*****************************************************
 関数名  void CbfSw1(int e, lgGpioAlert_p evt, void *userdata)
 引  数  e            使用せず
       evt          使用せず
       userdata     使用せず
 戻り値  なし
 概  要  SW1の割込み処理関数で、グローバル変数g_sw1に1を代入する
 *****************************************************/
void CbfSw1(int e, lgGpioAlert_p evt, void *userdata)
{g_sw1 = 1;}

