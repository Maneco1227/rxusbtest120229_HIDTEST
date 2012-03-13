#include "iodefine.h"
#include "DEBUGFIFO.h"

unsigned char *p_dcpBuf;
unsigned short dcpBufCnt;

unsigned char *p_pipe2Buf;
unsigned short pipe2BufCnt;

void WriteControlINPacket(void)
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
	} 
	while(USB0.CFIFOSEL.BIT.ISEL == 0);
	
	/*Wait for buffer to be ready*/
	while(USB0.CFIFOCTR.BIT.FRDY == 0);
	
	/* Write data to the IN Fifo untill have written a full packet
	 or we have no more data to write */
	while((Count < 64) && (dcpBufCnt != 0))
	{
		/*For 8 bit access write only to low 8 bits of CFIFO.*/
		USB0.CFIFO.BYTE.L = *p_dcpBuf++;
		dcpBufCnt--;
		Count++;
	}
	
	/*Set PID to BUF*/
	USB0.DCPCTR.BIT.PID = 1; // PID = BUF.
	
	/*If we have not written a full packets worth to the buffer then need to
	signal that the buffer is now ready to be sent, set the buffer valid flag (BVAL).*/
	if(Count != 64)
	{
		USB0.CFIFOCTR.BIT.BVAL = 1;
	}
					  
	/*Expect a BEMP interrupt when this has been sent.
	Can then send more data if required or status stage.*/		
}



void WriteBulkInPacket(void)
{
	unsigned long Count = 0;
	
	/*Write data to Bulk IN pipe using D0FIFO*/
	/*Select Interrupt Pipe and 16 bit access*/
	USB0.D0FIFOSEL.BIT.MBW = 0;
	USB0.D0FIFOSEL.BIT.CURPIPE = 2;

	/*Wait for buffer to be ready*/
	while(USB0.D0FIFOCTR.BIT.FRDY == 0){;}
	
	/* Write data to the IN Fifo untill have written a full packet
	 or we have no more data to write */
	while((Count < 64) && (pipe2BufCnt != 0))
	{
		/*The last byte. This is written to the Least Significant Byte of 16bit data port*/
		if(p_pipe2Buf != (void *)0)
		{
			USB0.D0FIFO.BYTE.L = *p_pipe2Buf;
			p_pipe2Buf++;

			pipe2BufCnt--;
			Count++;
		}
		else
		{
			/*This BULK IN was setup to just send dummy data*/
			USB0.D0FIFO.BYTE.L = 0xAA;
		}
	}
	
	/*Often commented as it slows it down coniderably.*/
	//DEBUG_MSG_HIGH( ("USBHAL: BULK IN - sending %lu, %lu remaining\r\n",Count, g_Bulk.m_IN.m_DataBuff.NumBytes));
	
	/*Send the packet */
	/*Set PID to BUF*/
	USB0.PIPE2CTR.BIT.PID = 1;
		
	/*If we have not written a full packets worth to the buffer then need to
	signal that the buffer is now ready to be sent, set the buffer valid flag (BVAL).*/
	if(Count != 64)
	{
		USB0.D0FIFOCTR.BIT.BVAL = 1;
	}

	if(Count != 64 && pipe2BufCnt == 0)
	{
		USB0.BRDYENB.BIT.PIPE2BRDYE = 0;
	}
	else
	{
		USB0.BRDYENB.BIT.PIPE2BRDYE = 1;
	}
}
