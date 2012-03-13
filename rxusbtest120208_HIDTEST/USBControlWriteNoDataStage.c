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


void SetConfiguration(void)
{
	DEBUGFIFO_OutLine("SetConfiguration");
	
	/*Reset the data toggles on the endpoints*/

	/***BULK IN***/	
	/*PID must be set to NAK before configuring PIPECFG*/
	USB0.PIPE1CTR.BIT.PID = 0;
	
	/*Wait for pipe to be not busy*/
	while(USB0.PIPE1CTR.BIT.PBUSY == 1);
	
	/*Reset Data toggle*/
	USB0.PIPE1CTR.BIT.SQCLR = 1;	
	
	/*Clear Buffer*/
	USB0.PIPE1CTR.BIT.ACLRM = 1;
	USB0.PIPE1CTR.BIT.ACLRM = 0;
	

	/***BULK OUT***/	
	/*PID must be set to NAK before configuring PIPECFG*/
	USB0.PIPE2CTR.BIT.PID = 0;

	/*Wait for pipe to be not busy*/
	while(USB0.PIPE2CTR.BIT.PBUSY == 1);
	
	/*Reset Data toggle*/
	USB0.PIPE2CTR.BIT.SQCLR = 1;	
	
	/*Clear Buffer*/
	USB0.PIPE2CTR.BIT.ACLRM = 1;
	USB0.PIPE2CTR.BIT.ACLRM = 0;


}


// Control read status stage. 
/*Setup packet has been received.*/
/*This is a Request that has a data write stage*/
void ControlWriteNoDataStage(void)
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
	case 0x0000:
		switch(bRequest)
		{
		case 0x0009:
			// SetConfiguration.
			SetConfiguration();
			
			// PID = BUF.
			USB0.DCPCTR.BIT.PID = 1;	
			/* Set CCPL bit */
			USB0.DCPCTR.BIT.CCPL = 1;
			break;
		
		default:
			// Stall.
			USB0.DCPCTR.BIT.PID = 2;	
			break;	
		}
		break;
		
		
	case 0x0021:
		switch(bRequest)
		{
		case 0x0022:
			// PID = BUF.
			USB0.DCPCTR.BIT.PID = 1;	
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
}
