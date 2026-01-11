#define OL      3   //1 オクターブ下
#define OC      4   //中央のオクターブ
#define OH      5   //1 オクターブ上

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

#define QN      1000    //4分音符(quarter note)

int music[][3] = {  //ドレミ
    {OL,DO,QN}, //1オクターブ下
    {OL,RE,QN},
    {OL,MI,QN},
    {OL,FA,QN},
    {OL,SO,QN},
    {OL,RA,QN},
    {OL,SI,QN},
    {OC,DO,QN}, //中央のオクターブ
    {OC,RE,QN},
    {OC,MI,QN},
    {OC,FA,QN},
    {OC,SO,QN},
    {OC,RA,QN},
    {OC,SI,QN},
    {OH,DO,QN}, //1オクターブ上
    {OH,RE,QN},
    {OH,MI,QN},
    {OH,FA,QN},
    {OH,SO,QN},
    {OH,RA,QN},
    {OH,SI,QN},
    {OH,REST,QN}}; //曲の最後は休止符を挿入して音を止める

