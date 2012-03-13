#include "iodefine.h"

#include "DEBUGFIFO.h"

//
// USB Standard Device Request Variables.
//
extern unsigned short bmRequestType;
extern unsigned short bRequest;
extern unsigned short wValue;
extern unsigned short wIndex;
extern unsigned short wLength;

extern unsigned char *p_dcpBuf;
extern unsigned short dcpBufCnt;


extern void WriteControlINPacket();

unsigned char g_LineCoding[7] = 
{
	0x00, 0xc2, 0x01, 0x00,		/* 115200 baud = h'0001c200 - send little endian */
	0x00,						/* 1 stop bit */
	0x00,						/* Parity = None */
	0x08						/* 8 Data Bits */
};

void GetLineCoding(void)
{
	DEBUGFIFO_OutLine("GET_LINE_CODING");
	
	switch(bmRequestType)
	{
	case 0x00A1:
		switch(bRequest)
		{
		case 0x0021:
			p_dcpBuf = &g_LineCoding[0];
			dcpBufCnt = 7;
			
			WriteControlINPacket();
			break;

		default:
			// Stall.
			USB0.DCPCTR.BIT.PID = 2;					
			break;
		}
		break;
	
	default:
		// Stall.
		USB0.DCPCTR.BIT.PID = 2;					
		break;
	}
}