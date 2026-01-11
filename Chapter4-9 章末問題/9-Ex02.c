// 9-Ex02.c
// gcc -Wall -o "%e" "%f" -llgpio -lpthread -g -O0 libMyPi2.a
// Geanyのオプションの"%e"は実行ファイル名, "%f"はソースファイル名のこと

#include <stdio.h>
#include <stdlib.h>     //system,EXIT_SUCCESS
#include <lgpio.h>      //lguSleep

int main(void){
    int kyb;
    while(1){
        printf("\nホワイトバンスの選択\n");
        printf("1:自動,2:電球,3:タングステン,4:蛍光灯,5:室内,6:太陽光,7:曇天, 0:終了 >>>");
        scanf("%d",&kyb);
            switch(kyb){
                case 1:
                    printf("自動\n");
                    system("rpicam-still --awb auto");          //自動
                    break;
                case 2:
                    printf("電球\n");
                    system("rpicam-still --awb incandescent");  //電球
                    break;
                case 3:
                    printf("タングステン\n");
                    system("rpicam-still --awb tungsten");      //タングステン
                    break;
                case 4:
                    printf("蛍光灯\n");
                    system("rpicam-still --awb fluorescent");   //蛍光灯
                    break;
                case 5:
                    printf("室内\n");
                    system("rpicam-still --awb indoor");        //室内
                    break;
                case 6:
                    printf("太陽光\n");
                    system("rpicam-still --awb daylight");      //太陽光
                    break;
                case 7:
                    printf("曇天\n");
                    system("rpicam-still --awb cloudy");        //曇天
                    break;
                case 0:
                    printf("終了\n");                            //終了
                    return EXIT_SUCCESS;
                default:
                    printf("入力の値が範囲外です\n\n");
                    break;
        }
        lguSleep(0.001);                          //CPU使用率の抑制のため
    }
    return EXIT_SUCCESS;
}
