#include "iodefine.h"

/*PID Values*/
#define PID_NAK 	0
#define PID_BUF 	1
#define PID_STALL_1 2
#define PID_STALL_2 3


/**********************************************************************
* Outline 		: WriteControlINPacket
* Description 	: If the Control IN buffer contains data then this will
*				  write it to the pipe buffer until either the packet
*				  is full or all the data has been written.
*				  If there is no data in the buffer this will send
*				  a zero length packet.
*				
* Argument  	: none
* Return value  : none
**********************************************************************/
static void WriteControlINPacket(void)
{
	unsigned short Count = 0;
	
	/*Write data to DCP using CFIFO*/
	/*Select DCP (Default Control Pipe)*/
	USB0.CFIFOSEL.BIT.CURPIPE = 0;
	
	/*Enable buffer for reading*/
	/*HW Manual says to check the setting has worked*/
	do
	{
		USB0.CFIFOSEL.BIT.ISEL = 1;
	} while(USB0.CFIFOSEL.BIT.ISEL == 0);
	
	/*Wait for buffer to be ready*/
	while(USB0.CFIFOCTR.BIT.FRDY == 0);
	
	/* Write data to the IN Fifo untill have written a full packet
	 or we have no more data to write */
	while((Count < CONTROL_IN_PACKET_SIZE) && (g_Control.m_IN.m_DataBuff.NumBytes != 0))
	{
		/*For 8 bit access write only to low 8 bits of CFIFO.*/
		USBIO.CFIFO.BYTE.L = *g_Control.m_IN.m_DataBuff.pucBuf;
		g_Control.m_IN.m_DataBuff.pucBuf++;
		g_Control.m_IN.m_DataBuff.NumBytes--;
		Count++;
	}
	
	/*Set PID to BUF*/
	USB0.DCPCTR.BIT.PID = PID_BUF;
	
	/*If we have not written a full packets worth to the buffer then need to
	signal that the buffer is now ready to be sent, set the buffer valid flag (BVAL).*/
	if(BULK_IN_PACKET_SIZE != Count)
	{
		USB0.CFIFOCTR.BIT.BVAL = 1;
	}
	
	/*Expect a BEMP interrupt when this has been sent.
	Can then send more data if required or status stage.*/		
}
/**********************************************************************
End WriteControlINPacket function
**********************************************************************/


/**********************************************************************
* Outline 		: ReadControlOUTPacket
* Description 	: If a buffer has been setup for Control OUT then
*				  read data into the buffer.
*				  If buffer is full or a short packet
*				  is received then call the done callback.
*				  If data is available but there is no buffer
*				  setup for it then call the error callback.

* Argument  	: none
* Return value  : none
**********************************************************************/
static void ReadControlOUTPacket(void)
{
	volatile unsigned char dummy;
	unsigned long Count = 0;
	
	/*Read data from DCP using CFIFO*/
	/*Select DCP*/
	/*NOTE this probably will have already been done if using BRDY interrupt.*/
	USB0.CFIFOSEL.BIT.CURPIPE = 0;
	
	/*Set PID to BUF*/
	USB0.DCPCTR.BIT.PID = PID_BUF;
	
	/*Enable buffer for reading*/
	/*HW Manual says to check the setting has worked*/
	do
	{
		USB0.CFIFOSEL.BIT.ISEL = 0;
	} while(USB0.CFIFOSEL.BIT.ISEL == 1);
	
	/*Set Read Count Mode - so DTLN count will decrement as data read from buffer*/
	USB0.CFIFOSEL.BIT.RCNT = 1;
	
	/*Wait for buffer to be ready*/
	while(USB0.CFIFOCTR.BIT.FRDY == 0);
	
	/*Read data while there is data in the FIFO and 
	room in buffer to store it in.*/
	while((g_Control.m_OUT.m_BuffSize > g_Control.m_OUT.m_DataBuff.NumBytes) && (USB0.CFIFOCTR.BIT.DTLN != 0))
	{
		if(g_Control.m_OUT.m_DataBuff.pucBuf != NULL)
		{
			*g_Control.m_OUT.m_DataBuff.pucBuf = (unsigned char)USBIO.CFIFO.BYTE.L;
			g_Control.m_OUT.m_DataBuff.pucBuf++;
			Count++;
		}
		else
		{
			dummy = (unsigned char)USBIO.CFIFO.BYTE.L;
		}
		g_Control.m_OUT.m_DataBuff.NumBytes++;
	}

	/*Have we completed the CONTROL OUT request?*/
	/*If this wasn't a full packet or we have filled the buffer*/
	/*Check CONTROL OUT has been setup*/
	if(((g_Control.m_OUT.m_BuffSize != 0) && (g_Control.m_OUT.m_BuffSize == g_Control.m_OUT.m_DataBuff.NumBytes)) ||
	   (Count != CONTROL_OUT_PACKET_SIZE))
	{
		/*Buffer not valid anymore*/
		g_Control.m_OUT.m_BuffSize = 0;
		
		/*Disable this interrupt*/
		USB0.BRDYENB.BIT.PIPE2BRDYE = 0;
		
		/*End of data transfer - now onto status stage*/
		/*Set CCPL to 1 - this will cause module to automatically handle the status stage.
		i.e Read a zero length packet from host and send ACK response*/
		/*Set PID to BUF before setting CCPL*/
		USB0.DCPCTR.BIT.PID = PID_BUF;
		USB0.DCPCTR.BIT.CCPL = 1; 

		/*Call registered callback*/
		if(g_Control.m_OUT.m_fpDone != NULL)
		{
			g_Control.m_OUT.m_fpDone(USB_ERR_OK, g_Control.m_OUT.m_DataBuff.NumBytes);
		}
	}
	
	/*If at this point more data is available then call Error CallBack*/
	if(USBIO.CFIFOCTR.BIT.DTLN != 0)
	{
		/*No buffer available for CONTROL OUT*/
		g_CBs.fpError(USB_ERR_CONTROL_OUT);
	}	
}
/**********************************************************************
End ReadControlOUTPacket function
**********************************************************************/