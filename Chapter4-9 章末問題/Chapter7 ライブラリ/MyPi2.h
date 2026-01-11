// MyPi2.h

//LCD AQM1602用関数
extern int LcdSetup(int hndLcd);
extern int LcdWriteChar(int hndLcd, char c);
extern int LcdNewline(int hndLcd);
extern int LcdClear(int hndLcd);
extern int LcdWriteString(int hndLcd, char *s);
