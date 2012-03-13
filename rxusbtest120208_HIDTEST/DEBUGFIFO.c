#include "iodefine.h"

//
// DEBUG FIFO:
//
// PD[7:0]: Data
// PE0: TXE#
// PE1: WR
//

void DEBUGFIFO_Init(void)
{
	PORTD.DDR.BYTE = 0xFF;
	PORTE.DDR.BIT.B0 = 1;
	PORTE.DDR.BIT.B1 = 1;

}

void DEBUGFIFO_OutChar(unsigned char c)
{
	PORTE.DR.BIT.B0 = 0;
	PORTE.DR.BIT.B1 = 0;
	
	PORTE.DR.BIT.B1 = 1;
	
	PORTD.DR.BYTE = c;
	
	PORTE.DR.BIT.B1 = 0;
}

void DEBUGFIFO_OutString(unsigned char *s)
{
	while(*s != '\0')
	{
		DEBUGFIFO_OutChar(*s++);
	}
}

void DEBUGFIFO_OutLine(unsigned char *s)
{
	while(*s != '\0')
	{
		DEBUGFIFO_OutChar(*s++);
	}
	
	DEBUGFIFO_OutChar(0x0d);
	DEBUGFIFO_OutChar(0x0a);
}

void DEBUGFIFO_OutNibble(unsigned char c)
{
	switch(c)
	{
	case 0:
		DEBUGFIFO_OutChar('0');
		break;

	case 1:
		DEBUGFIFO_OutChar('1');
		break;

	case 2:
		DEBUGFIFO_OutChar('2');
		break;
		
	case 3:
		DEBUGFIFO_OutChar('3');
		break;
		
	case 4:
		DEBUGFIFO_OutChar('4');
		break;
		
	case 5:
		DEBUGFIFO_OutChar('5');
		break;
		
	case 6:
		DEBUGFIFO_OutChar('6');
		break;
		
	case 7:
		DEBUGFIFO_OutChar('7');
		break;
		
	case 8:
		DEBUGFIFO_OutChar('8');
		break;
		
	case 9:
		DEBUGFIFO_OutChar('9');
		break;
		
	case 10:
		DEBUGFIFO_OutChar('A');
		break;
		
	case 11:
		DEBUGFIFO_OutChar('B');
		break;
		
	case 12:
		DEBUGFIFO_OutChar('C');
		break;
		
	case 13:
		DEBUGFIFO_OutChar('D');
		break;
		
	case 14:
		DEBUGFIFO_OutChar('E');
		break;
		
	case 15:
		DEBUGFIFO_OutChar('F');
		break;

	default:
		break;
	}
}

void DEBUGFIFO_OutByte(unsigned char c)
{
	unsigned char ch;
	unsigned char cl;
	
	ch = c / 16;
	cl = c % 16;
	
	DEBUGFIFO_OutNibble(ch);
	DEBUGFIFO_OutNibble(cl);
}

void DEBUGFIFO_OutWord(unsigned short c)
{
	unsigned char ch;
	unsigned char cl;
	
	ch = c / 256;
	cl = c % 256;
	
	DEBUGFIFO_OutByte(ch);
	DEBUGFIFO_OutByte(cl);
}

void DEBUGFIFO_OutByteValue(unsigned char *s, unsigned char c)
{
	DEBUGFIFO_OutString(s);
	DEBUGFIFO_OutByte(c);
	DEBUGFIFO_OutLine("");
}

void DEBUGFIFO_OutWordValue(unsigned char *s, unsigned short c)
{
	DEBUGFIFO_OutString(s);
	DEBUGFIFO_OutWord(c);
	DEBUGFIFO_OutLine("");
}


