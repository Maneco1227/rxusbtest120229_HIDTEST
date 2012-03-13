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

// Control read status stage. 
/*Setup packet has been received.*/
/*This is a Request that has a data write stage*/
void ControlReadStatusStage(void)
{
	/*Clear VALID flag*/
	USB0.INTSTS0.BIT.VALID = 0;
	
	/*Read the 8 bytes setup command into temporary buffer*/
	bmRequestType = USB0.USBREQ.BIT.BMREQUESTTYPE;
	bRequest      = USB0.USBREQ.BIT.BREQUEST;
	wValue        = USB0.USBVAL;
	wIndex        = USB0.USBINDX;
	wLength       = USB0.USBLENG;
	
	DEBUGFIFO_OutWordValue("bmRequestType = ", bmRequestType);
	DEBUGFIFO_OutWordValue("bRequest      = ", bRequest);
	DEBUGFIFO_OutWordValue("wValue        = ", wValue);
	DEBUGFIFO_OutWordValue("wIndex        = ", wIndex);
	DEBUGFIFO_OutWordValue("wLength       = ", wLength);


	switch(bmRequestType)
	{
	case 0x0080:
		switch(bRequest)
		{
		case 0x0006:
			switch(wValue)
			{
				case 0x0100:
				
				case 0x0200:
				
				case 0x0300:
				case 0x0301:
				case 0x0302:
				case 0x0303:
				case 0x0304:
				case 0x0305:
				case 0x0306:
				case 0x0307:
					/* Set CCPL bit */
					USB0.DCPCTR.BIT.CCPL = 1;
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
		break;
		
	default:
		// Stall.
		USB0.DCPCTR.BIT.PID = 2;					
		break;
	}
}
