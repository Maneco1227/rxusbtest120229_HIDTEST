#include "iodefine.h"
#include "DEBUGFIFO.h"

extern unsigned char *p_pipe2Buf;
extern unsigned short pipe2BufCnt;

extern void WriteBulkInPacket(void);

/**********************************************************************
* Outline 		: HandleVBus
* Description 	: VBUS interrupt has occoured.
*				  USB cable has been connected or disconnected.
*				  If Disconnected then use registered callback to inform user.
*				  Note: User is not informed of connection event until SetAddress
*                 USB request is received.	
* Argument  	: none
* Return value  : none
**********************************************************************/
static void HandleVBus(void)
{
	/*Check VBSTS to see the VBUS value.*/
	unsigned char vbsts_value;
	unsigned char count;

	#define CHATTER_COUNT 5
	
	/*Keep reading VBSTS until it is stable*/
	do
	{
		count = CHATTER_COUNT;

		/*Read VBSTS then confirm it's value does not change for CHATTER_COUNT times */
		vbsts_value = (unsigned char)USB0.INTSTS0.BIT.VBSTS;

		while(count != 0)
		{
			if((unsigned char)USB0.INTSTS0.BIT.VBSTS != vbsts_value)
			{
				/*Chatter - signal is not stable*/
				break;
			}
			else
			{
				count--;
			}
		}
	} while(count != 0);
	
	if(vbsts_value == 1)
	{
		/* USB Bus Connected */
		//g_Control.m_etState = STATE_READY;
		
		/*Reset HAL*/
		//USBHAL_Reset();
		
		/*Call Registered Callback to tell that cable is connected*/
		/*NOTE: Moved until Set Address has been recieved as more important for
		user to know when enumerated that just vbus change.*/
		/*g_CBs.fpCable(true);*/
		
		USB0.SYSCFG.BIT.DPRPU = 1;
	}
	else
	{	
		/* USB Bus Disconnected */
		//g_Control.m_etState = STATE_DISCONNECTED;
		
		/*Cancel any pending operations - 
		including calling any 'done' callbacks */
		//USBHAL_Cancel(USB_ERR_NOT_CONNECTED);
		
		/*Call Registered Callback*/
		//g_CBs.fpCable(false);

		USB0.SYSCFG.BIT.DPRPU = 0;
	}
}
/**********************************************************************
End HandleVBus function
**********************************************************************/

/**********************************************************************
* Outline 		: HandleDVST
* Description 	: Device State Transition Interrupt.
*                 Used to detect a USB bus reset.
* Argument  	: none
* Return value  : none
**********************************************************************/
static void HandleDVST(void)
{	
	DEBUGFIFO_OutLine("HandleDVST");
	DEBUGFIFO_OutWordValue("USB0.INTSTS0.BIT.DVSQ = ", USB0.INTSTS0.BIT.DVSQ);
	
	/*Read new device state in DVSQ bits*/
	switch(USB0.INTSTS0.BIT.DVSQ)
	{
	case 0:
		// Powered state.
		break;
		
	case 1:
		// Default state.
		/*Has a bus reset just occured*/
		if(USB0.DVSTCTR0.BIT.RHST == 4)
		{
			/*Reset endpoints - this also ensures BULK out is still able
			to receive data.*/
			//USBHAL_ResetEndpoints();
		}	//	
		break;
		
	case 2:
		// Address state.
		/*This is a conveniant point to tell user that USB is connected. */
		//g_CBs.fpCable(true);
		break;
		
	case 3:
		// Configured state.
		/*Nothing todo*/
		break;
		
	case 4:
	default:
		// Suspended state.
		/*Nothing todo*/
		break;
	};
}
/**********************************************************************
End HandleDVST function
**********************************************************************/

/**********************************************************************
* Outline 		: HandleCTRT
* Description 	: Control Transfer Stage Transition Interrupt
*				  Used to detect the receipt of a Setup command.
* Argument  	: none
* Return value  : none
**********************************************************************/
extern void ControlReadDataStage(void);
extern void ControlReadStatusStage(void);
extern void ControlWriteNoDataStage(void);

static void HandleCTRT(void)
{
	DEBUGFIFO_OutLine("ENTER: HandleCTRT");
	DEBUGFIFO_OutWordValue("USB0.INTSTS0.BIT.CTSQ = ", USB0.INTSTS0.BIT.CTSQ); 
	
	switch(USB0.INTSTS0.BIT.CTSQ)
	{
	case 0: 
		// Idle, Setup stage.
		DEBUGFIFO_OutLine("// Idle, Setup stage.");
		break;
		
	case 1: 
		// Control read data stage. 
		/*Setup packet has been received.*/
		/*This is a Request that has a data write stage*/
		DEBUGFIFO_OutLine("// Control read data stage.");
		
		ControlReadDataStage();
		break;
		
	case 2: 
	 	// Control read status stage.
		DEBUGFIFO_OutLine("// Control read status stage.");
		
		ControlReadStatusStage();
		break;
		
	case 3:
		// Control write data stage.
		/*Setup packet has been received.*/
		/*This is a Request that has a data write stage*/
		DEBUGFIFO_OutLine("// Control write data stage.");
		
		ControlWriteDataStage();			
		break;
		
	case 4:
		// Control write status stage.
		DEBUGFIFO_OutLine("// Control write status stage.");
		
		ControlWriteStatusStage();
		break;
		
	case 5:
		// Control write No Data stage.
		/*Setup packet has been received.*/
		/*This is a Request that doesn't have a data stage*/
	
		DEBUGFIFO_OutLine("// Control write No Data stage.");
		
		ControlWriteNoDataStage();
		break;
		
	case 6:
		// Control transfer sequence error.
		DEBUGFIFO_OutLine("// Control transfer sequence error.");

		break;
		
	default:
		break;
	}
}
/**********************************************************************
End HandleCTRT function
**********************************************************************/

/**********************************************************************
* Outline 		: HandleBRDY
* Description 	: Buffer Ready Interrupt.
*                 Used to start/continue:
*					1. Interrupt IN packets.
*					2. Bulk IN packets.
*					3. Bulk IN packets.
*					4. Bulk OUT packets.
*					5. Control Out packets.
* Argument  	: none
* Return value  : none
**********************************************************************/
void HandleBRDY(void)
{
	unsigned short buf; // Received echo back buffer.

	DEBUGFIFO_OutWordValue("USB0.BRDYSTS.WORD = ",USB0.BRDYSTS.WORD);
	
	//
	/*Has Control OUT pipe caused this interrupt (pipe0)*/
	//
	if((USB0.BRDYSTS.BIT.PIPE0BRDY == 1) && (USB0.BRDYENB.BIT.PIPE0BRDYE == 1))
	{
		DEBUGFIFO_OutLine("Pipe0 Interrupt.");
		
		/*Clear this bit (write 1 to all other bits)*/
		USB0.BRDYSTS.WORD = ~0x0001;
		
		/*Read received packet*/
		//ReadControlOUTPacket();
		
		
	}	

}
/**********************************************************************
End HandleBRDY function
**********************************************************************/

/**********************************************************************
* Outline 		: HandleBEMP
* Description 	: Buffer Empty Interrupt
*				  Used for Control IN to write another packet or
*					move to status stage.
* Argument  	: none
* Return value  : none
**********************************************************************/
extern unsigned char *p_dcpBuf;
extern unsigned short dcpBufCnt;

extern void WriteControlINPacket(void);

void HandleBEMP(void)
{
	DEBUGFIFO_OutWordValue("USB0.BEMPSTS.WORD = ",USB0.BEMPSTS.WORD);
	
	/*Has control pipe caused this interrupt*/
	if(USB0.BEMPSTS.BIT.PIPE0BENP == 1)
	{
		/*Clear this bit (write 1 to all other bits)*/
		USB0.BEMPSTS.WORD = ~0x0001;
		
		/*If we are performing a CONTROL IN DATA*/
		//if(STATE_CONTROL_IN == g_Control.m_etState)
		//{
			/*If there is more data to send*/
			if(dcpBufCnt != 0)
			{
				WriteControlINPacket();
			}
			else
			{
				/*End of data transfer*/
				
				/*Clear CFIFO (seemed necessary to prevent duplicate transfers)*/
				USB0.CFIFOCTR.BIT.BCLR = 1;
				
				/*Status Stage*/
				/*Set CCPL to 1 - this will cause module to automatically handle the status stage.
				i.e Transmit zero length packet and recieve ACK from host.*/				
				/*Ensure PID is set to BUF before setting CCPL*/
				USB0.DCPCTR.BIT.PID = 1; // PID = BUF.
				USB0.DCPCTR.BIT.CCPL = 1;
			}
		//}
	}
		
	/*NOTE: for other pipes we are not using BEMP, only BRDY*/
}
/**********************************************************************
End HandleBEMP function
**********************************************************************/


/**********************************************************************
* Outline 		: USBHALInterruptHandler 
* Description 	: USB interrupt handler.
*				  User must ensure that this gets called when
*				  the USB interrupt occours.
*				  
* Argument  	: none
* Return value  : none
**********************************************************************/
void USBInterruptHandler(void)
{
	DEBUGFIFO_OutLine("ENTER: USBInterruptHandler");
	DEBUGFIFO_OutWordValue("USB0.INTSTS0.WORD: ", USB0.INTSTS0.WORD);
	
	/*The status flag tells us what caused the interrupt. */
	
	/*Is this a VBUS Interrupt?*/
	if(USB0.INTSTS0.BIT.VBINT == 1)
	{
		DEBUGFIFO_OutLine("VBINT == 1");
		
		/*Clear interrupt flag*/
		USB0.INTSTS0.BIT.VBINT = 0;
		
		/* VBUS State change i.e USB cable connected/disconnected */
		HandleVBus();	
	}
	
	/*Is this a DVST Interrupt?*/
	if(USB0.INTSTS0.BIT.DVST == 1)
	{
		DEBUGFIFO_OutLine("DVST == 1");
		
		/*Clear interrupt flag*/
		USB0.INTSTS0.BIT.DVST = 0;
		
		/* Device State Transition */
		HandleDVST();
	}
	
	/*Is this a CTRT interrupt?*/
	if(USB0.INTSTS0.BIT.CTRT == 1)
	{
		DEBUGFIFO_OutLine("CTRT == 1");

		/* Control Transfer Stage Transition */
		HandleCTRT();
		
		/*Clear interrupt flag*/
		/*Note: Had to do this after call to HandleCTRT to avoid getting
		two interrupts for same setup packet. */
		USB0.INTSTS0.BIT.CTRT = 0;
	}
	
	/*Is this a BEMP interrupt?*/
	if(USB0.INTSTS0.BIT.BEMP == 1)
	{
		DEBUGFIFO_OutLine("BEMP == 1");

		/*Note: Can't clear this flag by SW.*/
		
		/*Buffer Empty Interrupt*/
		HandleBEMP();
	}
	
	/*Is this a BRDY interrupt?*/
	if(USB0.INTSTS0.BIT.BRDY == 1)
	{
		DEBUGFIFO_OutLine("BRDY == 1");
		
		/*Note: Can't clear this flag by SW.*/
		
		/*Buffer Ready Interrupt*/
		HandleBRDY();
	}
}
//**********************************************************************
//End USBHALInterruptHandler function
//**********************************************************************/
