// MyPi2.h

//LCD AQM1602用関数
extern int LcdSetup(int hndLcd);
extern int LcdWriteChar(int hndLcd, char c);
extern int LcdNewline(int hndLcd);
extern int LcdClear(int hndLcd);
extern int LcdWriteString(int hndLcd, char *s);

//温度センサ ADT7410用関数
extern double Adt7410Read(int hndTmp);

//D/AコンバータMCP4922用関数
extern int Mcp4922Write(int hndDac,unsigned char dacCh, unsigned short dacCode);

//A/DコンバータMCP3802用関数
extern unsigned short Mcp3208RW(int hndAdc, unsigned char adcCh);
