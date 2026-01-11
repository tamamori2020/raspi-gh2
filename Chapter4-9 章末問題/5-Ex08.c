// 5-Ex08.c
// gcc -Wall -o "%e" "%f" -llgpio -lpthread -g -O0
// Geanyのオプションの"%e"は実行ファイル名, "%f"はソースファイル名のこと

#include <stdio.h>      //printf
#include <stdlib.h>     //EXIT_SUCCESS
#include <time.h>       //time
#include <lgpio.h>      //lgGpiochipOpen,lgGroupRead,etc
#define PI5     4       // /dev/gpiochip4
#define PI4B    0       // /dev/gpiochip0
#define SW0     4       //SW0からSW3までのGPIO番号の定義
#define SW1     5
#define SW2     6
#define SW3     26
#define SNUM    4       //SWの数
#define LED0    23      //LED0からLED3までのGPIO番号の定義
#define LED1    22
#define LED2    25
#define LED3    24
#define LNUM    4       //LEDの数

int main (void){
    int i,no;
    int dice=0;         //0〜3までのサイコロの目
    int scr=0;          //得点
    int err=0;          //失点
    int speed=1000;     //１打あたりの制限時間(ms)
    int ledPtn[4]={0b0001,0b0010,0b0100,0b1000};    //LED点灯パターン
    int hnd;
    int lFlgIn=LG_SET_PULL_DOWN;    //スイッチ用
    int sw30[SNUM]={SW0,SW1,SW2,SW3};
    uint64_t bits;
    int lFlgOut=0;                  //LED用
    int leds[LNUM]={LED0,LED1,LED2,LED3};
    int levels[LNUM]={0,0,0,0};
    uint64_t gMsk=0b1111;
    hnd = lgGpiochipOpen(PI5);
    //hnd = lgGpiochipOpen(PI4B);
    lgGroupClaimInput(hnd, lFlgIn, SNUM, sw30);
    lgGroupClaimOutput(hnd,lFlgOut,LNUM,leds,levels);
    for(i=0;i<2;i++){               //ゲーム開始の合図
        lgGroupWrite(hnd,LED0,0b1111,gMsk);
        lguSleep(0.2);
        lgGroupWrite(hnd,LED0,0,gMsk);
        lguSleep(0.2);
    }
    srand((unsigned)time(NULL));    //rand関数で発生させる乱数の初期化

    for(no=1;no<11;no++){           //10回プレイをする
        dice = rand()%4;            //0〜3の乱数を発生
        lgGroupWrite(hnd,LED0,ledPtn[dice],gMsk);   //LEDの点灯
        for(i=0;i<speed;i++){       //1打あたりの制限時間
            lgGroupRead(hnd,SW0,&bits);
            if(bits==ledPtn[dice]){ //当たりの場合は得点を加算してfor文をブレイクして次へ
                scr++;
                break;
            }
            lguSleep(0.001);        //1ミリ秒時間待ち
        }
        if(bits!=ledPtn[dice]){     //外した場合は失点を加算する
            err++;
        }
        printf("No.%d  Score %d  Lost %d\n",no,scr,err);    //得点と失点の表示
        lgGroupWrite(hnd,LED0,0b0000,gMsk);         //ゲームの区切りのため0.3秒全LEDを消灯
        lguSleep(0.3);
    }

    if(scr>err){                   //結果の判定と表示
        printf("You win\n");
    }else if(scr<err){
        printf("You lose\n");
    }else{
        printf("Draw\n");
    }

    lgGpiochipClose(hnd);
    return EXIT_SUCCESS;
}
