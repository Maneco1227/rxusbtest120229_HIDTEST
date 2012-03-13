extern void DEBUGFIFO_Init(void);
extern void DEBUGFIFO_OutChar(unsigned char c);
extern void DEBUGFIFO_OutString(unsigned char *s);
extern void DEBUGFIFO_OutLine(unsigned char *s);
extern void DEBUGFIFO_OutByte(unsigned char c);
extern void DEBUGFIFO_OutWord(unsigned short c);
extern void DEBUGFIFO_OutByteValue(unsigned char *s, unsigned char c);
extern void DEBUGFIFO_OutWordValue(unsigned char *s, unsigned short c);

