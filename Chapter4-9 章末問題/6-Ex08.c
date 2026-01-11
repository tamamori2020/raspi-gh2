// 6-Ex08.c
// gcc -Wall -o "%e" "%f" -llgpio -lpthread -g -O0 -lm
// Geanyのオプションの"%e"は実行ファイル名, "%f"はソースファイル名のこと

#include <stdio.h>      //printf,scanf
#include <stdlib.h>     //EXIT_SUCCESS
#include <unistd.h>     //usleep
#include <math.h>       //pow
#include <pthread.h>    //pthread_exit
#include <lgpio.h>      //lgThreadStart,etc
#define PI5     4       // /dev/gpiochip4
#define PI4B    0       // /dev/gpiochip0
#define LED0    23      //LED0からLED3までのGPIO番号の定義
#define BUZZER  18
/*  オクターブ(oct)  */
#define OL      3   //1オクターブ下
#define OC      4   //中央のオクターブ
#define OH      5   //1オクターブ上
/*   階名(solfa)   */
#define REST    0   //休符
#define DO      1   //ド
#define DOS     2   //ド#
#define RE      3   //レ
#define RES     4   //レ#
#define MI      5   //ミ
#define FA      6   //ファ
#define FAS     7   //ファ#
#define SO      8   //ソ
#define SOS     9   //ソ#
#define RA      10  //ラ
#define RAS     11  //ラ#
#define SI      12  //シ
/* グローバル変数 */
int g_hnd;          //LED0とBUZZERのハンドル
/* プロトタイプ宣言 */
void g(int hnd, int oct, int solfa, int length); //1音を発生する関数
void *Led1Hz(void *userdata);   //Lチカスレッド関数
void *Orgel(void *userdata);    //オルゴールスレッド関数

int main (void){
    int lFlgOut=0;
    int userdata;                       //スレッド関数への引数
    pthread_t   *pLed, *pOrgel;         //スレッドID　
    int i;
    int qN = 1000;                      //音の長さ(ms)
    g_hnd = lgGpiochipOpen(PI5);
    //g_hnd = lgGpiochipOpen(PI4B);
    lgGpioClaimOutput(g_hnd,lFlgOut,LED0,LG_LOW);
    lgGpioClaimOutput(g_hnd,lFlgOut,BUZZER,LG_LOW);

    while(1){
        printf("1:LED ON,2:LED OFF,3:Orgel ON, 4:Orgel OFF >>>");
        scanf("%d",&i);
        switch(i){
            case 1:      //Lチカスレッドスタート
                userdata=LED0;pLed=lgThreadStart(Led1Hz,&userdata);break;
            case 2:      //Lチカスレッドストップ
                lgThreadStop(pLed);lgGpioWrite(g_hnd,LED0,LG_LOW);break;
            case 3:      //オルゴールスレッドスタート
                pOrgel=lgThreadStart(Orgel,&userdata);break;
            case 4:      //オルゴールスレッドストップ
                lgThreadStop(pOrgel);g(g_hnd,OC,REST,qN);break;
            default:
                printf("入力の値が範囲外です\n\n");break;
        }
    }
    lgGpiochipClose(g_hnd);
    return EXIT_SUCCESS;
}

/*****************************************************
 関数名  void *Led1Hz(void *userdata)
 引  数  userdata GPIO番号
 戻り値  なし
 概  要  引数で指定されたGPIO番号に1Hzを永久ループで出力する。
 *****************************************************/
void *Led1Hz(void *userdata){   //Lチカのスレッド
    int led;
    led = *(int*)userdata;
    while(1){
        lgGpioWrite(g_hnd,led,LG_HIGH);
        lguSleep(0.5);
        lgGpioWrite(g_hnd,led,LG_LOW);
        lguSleep(0.5);
    }
    pthread_exit(NULL);
}

/*****************************************************
 関数名  void *Orgel(void *userdata)
 引  数  userdata 未使用
 戻り値  なし
 概  要  オルゴールを永久ループで再生する。
 *****************************************************/
void *Orgel(void *userdata){    //オルゴールのスレッド
    int i;
    int oct,solfa;
    int qN = 500; //4分音符(quarter note) の長さ(ミリ秒)
    int musicNo;
    int music[][3] = {  //ドレミ
        {OC,DO,qN}, //中央のオクターブ
        {OC,RE,qN},
        {OC,MI,qN},
        {OC,FA,qN},
        {OC,SO,qN},
        {OC,RA,qN},
        {OC,SI,qN},
        {OH,DO,qN}, //1オクターブ上
        {OH,REST,qN} //曲の最後は休止符を挿入して音を止める
    };
    musicNo = sizeof music/sizeof music[0];    //配列の要素数(音符)の計算

    while(1){
        for(i=0; i<musicNo; i++){   //音符の最初から最後まで再生
                oct = music[i][0];      //オクターブの取得
                solfa =  music[i][1];   //階名の取得
                qN = music[i][2];       //音の長さの取得
                g(g_hnd, oct,solfa,qN);   //1音を再生
        }
    }
    pthread_exit(NULL);
}

/*****************************************************
 関数名  void g(int g_hnd, int oct, int solfa, int length)
 引  数  g_hnd   lgTxPulse用ハンドル
       oct     3:1オクターブ下,4:中央,5:1オクターブ上
       solfa   0:休符,1:ド,2:ド#,3:レ,4:レ#,5:ミ,6:ファ,
               7:ファ#,8:ソ,9:ソ#,10:ラ,11:ラ#,12:シ
       length　音の長さ 単位ms
 戻り値  なし
 概  要  引数で指定された音名または休符を発生する。
 *****************************************************/
void g(int g_hnd, int oct, int solfa, int length){
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
    lgTxPulse(g_hnd,BUZZER,hlfT,hlfT,0,0);
    usleep(length*1000);        //マイクロ秒に変換
}
