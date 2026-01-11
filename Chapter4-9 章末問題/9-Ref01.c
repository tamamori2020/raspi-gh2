// 9-Ref01.c
// gcc -Wall -o "%e" "%f" -llgpio -lpthread -g -O0 libMyPi2.a
// Geanyのオプションの"%e"は実行ファイル名, "%f"はソースファイル名のこと

#include <stdio.h>
#include <stdlib.h>     //system,EXIT_SUCCESS
#include <lgpio.h>      //lgGpiochipOpen,lgGpioRead,etc
#define PI5     4       // /dev/gpiochip4
#define PI4B    0       // /dev/gpiochip0
#define SW0     4       //GPIO4をSW0と定義

int main(void){
    int hnd;
    int inFlg = LG_SET_PULL_DOWN;                 //プルダウン抵抗を有効
    hnd = lgGpiochipOpen(PI5);
    //hnd = lgGpiochipOpen(PI4B);
    lgGpioClaimInput(hnd,inFlg,SW0);

    while(1){
        if(lgGpioRead(hnd, SW0)==LG_HIGH){
            //写真を撮影
            system("rpicam-still -t 500 --width 640 --height 480 -o /home/my-pi/MyApp2/test.jpg");
            //ファイル名を日付にしてバックアップ
            system("sudo cp /home/my-pi/MyApp2/test.jpg   /home/my-pi/MyApp2/$(date +'%Y%m%d-%H%M%S').jpg");
            //電子メール送信  Subjectに表題を記入、foo@example.orgは送信先メールアドレスを指定
            system("echo '送信' | mail -A /home/my-pi/MyApp2/test.jpg -s 'From RasPi' foo@example.org");
        }
        lguSleep(0.001);                          //CPU使用率の抑制のため
    }
    lgGpiochipClose(hnd);
    return EXIT_SUCCESS;
}
