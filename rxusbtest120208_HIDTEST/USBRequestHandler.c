#include "iodefine.h"




/**********************************************************************
* Outline 		: CBSetup
* Description 	: Callback from HAL when it has received a setup packet.
				  Responds with either an CONTROL IN, OUT, ACK or STALL.
				  If this layer can not deal with the setup packet
				  on its own it will call an upper layer to see if it
				  knows how to deal with it.
* Argument  	: _pSetupPacket - buffer containing the setup packet.
* Return value  : none
**********************************************************************/
static void CBSetup(const uint8_t(*_pSetupPacket)[USB_SETUP_PACKET_SIZE])
{
	USB_ERR err;
	uint16_t NumBytes;
	uint8_t* pBuffer;
	
	/*Populate the Setup Packet structure g_oSetupPacket */
	PopulateSetupPacket(_pSetupPacket);
	
	/*Process this setup packet*/
	err = ProcessSetupPacket(&NumBytes, &pBuffer);
							
	if(USB_ERR_UNKNOWN_REQUEST == err)
	{
		/*Can't handle this setup packet*/
		/*Let upper layer try - call registered callback*/
		err = g_fpCBSetupPacket(&g_oSetupPacket, &NumBytes, &pBuffer);
	}
	
	if(USB_ERR_OK == err)
	{
		/*Is there a data stage?*/
		if(0 != g_oSetupPacket.wLength)
		{
			/* Is this a Data IN or OUT */
			if(0 != EXTRACT_bmRequest_DIRECTION(g_oSetupPacket.bmRequest))
			{
				/*IN*/
				/*Don't send more data than host has requested*/
				if(NumBytes > g_oSetupPacket.wLength)
				{
					NumBytes = g_oSetupPacket.wLength; 
				}
			
				/*Send Data*/
				USBHAL_Control_IN(NumBytes, pBuffer);
			}
			else
			{
				/*OUT*/
				assert(g_oSetupPacket.wLength == NumBytes);
				
				USBHAL_Control_OUT(NumBytes, pBuffer, g_fpCBControlOut);
			}
		}
		else
		{
			/*No data stage - just need to send ACK*/
			USBHAL_Control_Status();
		}
	}
	else
	{
		/*Something wrong with this control pipe so stall it.*/
		USBHAL_Control_Stall();
	}
}
/***********************************************************************************
End of function CBSetup
***********************************************************************************/


/**********************************************************************
* Outline 		: HandleSetupCmd
* Description 	: Have received a Contol Setup packet.
*				  Use registered callback to inform user.
* Argument  	: none
* Return value  : none
**********************************************************************/
static void HandleSetupCmd(void)
{
	unsigned char SetupCmdBuffer[USB_SETUP_PACKET_SIZE];
	unsigned short* p_16;
	
	/*Don't check state as host may have aborted other states - 
	so this just sets the state to STATE_CONTROL_SETUP*/

	/*Clear VALID flag*/
	USBIO.INTSTS0.BIT.VALID = 0;
	
	/*Read the 8 bytes setup command into temporary buffer*/
	p_16 = (unsigned short*)SetupCmdBuffer;
	*p_16++ = USBIO.USBREQ.WORD;
	*p_16++ = USBIO.USBVAL;
	*p_16++ = USBIO.USBINDX;
	*p_16 = USBIO.USBLENG;
	
	g_Control.m_etState = STATE_CONTROL_SETUP;
	/*NOTE: Expect to continue with a Data Stage
	 - (either Control IN or Control OUT) or straight to a Status Stage ,
	Users of this HAL must set this up when handling the Setup callback.*/
	
	/*Call Registered Callback*/
	g_CBs.fpSetup((const unsigned char(*)[USB_SETUP_PACKET_SIZE])SetupCmdBuffer);
}
/**********************************************************************
End HandleSetupCmd function
**********************************************************************/


