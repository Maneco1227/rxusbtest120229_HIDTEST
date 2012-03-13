#include "iodefine.h"

#include "DEBUGFIFO.h"

//
// USB Standard Device Request Variables.
//
unsigned short bmRequestType;
unsigned short bRequest;
unsigned short wValue;
unsigned short wIndex;
unsigned short wLength;

// Control read data stage. 
/*Setup packet has been received.*/
/*This is a Request that has a data write stage*/
void ControlReadDataStage(void)
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
	
	//g_Control.m_etState = STATE_CONTROL_SETUP;

	/*NOTE: Expect to continue with a Data Stage
	 - (either Control IN or Control OUT) or straight to a Status Stage ,
	Users of this HAL must set this up when handling the Setup callback.*/
	
	switch(bRequest)
	{
	case 0:
		// GET_STATUS.
		GetStatus();
		break;
				
	case 1:
		// CLEAR_FEATURE.
		ClearFeature();
		break;
	
	case 3:
		// SET_FEATURE.
		SetFeature();
		break;
	
	case 5:
		// SET_ADDRESS.
		SetAddress();
		break;
	
	case 6:
		// GET_DESCRIPTOR.
		GetDescriptor();
		break;
	
	case 7:
		// SET_DESCRIPTOR.
		SetDescriptor();
		break;
	
	case 8:
		// GET_CONFIGURATION.
		GetConfiguration();
		break;
	
	case 9:
		// SET_CONFIGURATION.
		SetConfiguration();
		break;
		
	case 10:
		// GET_INTERFACE.
		GetInterface();
		break;
	
	case 11:
		// SET_INTERFACE.
		SetInterface();
		break;
	
	case 12:
		// SYNCH_FRAME.
		SynchFrame();
		break;
	
	default:
		// STALL.
		USB0.DCPCTR.BIT.PID = 2; 
		break;
	}
}
