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


void SetConfiguration(void)
{
	switch(bmRequestType)
	{
	case 0x00:
		break;
	default:
		break;
	}		
}