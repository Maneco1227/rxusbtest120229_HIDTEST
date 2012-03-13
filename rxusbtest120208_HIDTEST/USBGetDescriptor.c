#include "iodefine.h"
#include "DEBUGFIFO.h"


//
// USB Standard Device Request Variables.
//
extern unsigned short bmRequestType;
//extern unsigned short bRequest;
extern unsigned short wValue;
extern unsigned short wIndex;
extern unsigned short wLength;


extern unsigned char *p_dcpBuf;
extern unsigned short dcpBufCnt;


extern unsigned char  usb_gpcdc_DeviceDescriptor[];
extern unsigned short usb_gpcdc_DeviceDescriptorLength;

extern unsigned char  usb_gpcdc_ConfigrationDescriptor[];
extern unsigned short usb_gpcdc_ConfigrationDescriptorLength;


extern unsigned char usb_gpcdc_StringDescriptor0[];
extern unsigned char usb_gpcdc_StringDescriptor1[];
extern unsigned char usb_gpcdc_StringDescriptor2[];
extern unsigned char usb_gpcdc_StringDescriptor3[];
extern unsigned char usb_gpcdc_StringDescriptor4[];
extern unsigned char usb_gpcdc_StringDescriptor5[];
extern unsigned char usb_gpcdc_StringDescriptor6[];
extern unsigned char usb_gpcdc_StringDescriptor7[];

extern void WriteControlINPacket();

//
void GetDeviceDescriptor(void)
{
	DEBUGFIFO_OutLine("GetDeviceDescriptor");
	
	p_dcpBuf = &usb_gpcdc_DeviceDescriptor[0];
	dcpBufCnt = usb_gpcdc_DeviceDescriptorLength;
	
	WriteControlINPacket();
}

//
void GetConfigurationDescriptor(void)
{
	DEBUGFIFO_OutLine("GetConfigurationDescriptor");
	
	p_dcpBuf = &usb_gpcdc_ConfigrationDescriptor[0];
	dcpBufCnt = wLength;
	
	
	WriteControlINPacket();
}

//
void GetStringDescriptor(void)
{
	unsigned short i;
	
	DEBUGFIFO_OutLine("GetStringDescriptor");
	DEBUGFIFO_OutWordValue("wValue = ", wValue);
	
	switch(wValue)
	{
	case 0x0300:
		p_dcpBuf = &usb_gpcdc_StringDescriptor0[0];
		dcpBufCnt = 4;
		break;
		
	case 0x0301:
		p_dcpBuf = &usb_gpcdc_StringDescriptor1[0];
		dcpBufCnt = 16;
		break;
	
	case 0x0302:
		p_dcpBuf = &usb_gpcdc_StringDescriptor2[0];
		dcpBufCnt = 44;
		break;
	
	case 0x0303:
		p_dcpBuf = &usb_gpcdc_StringDescriptor3[0];
		dcpBufCnt = 8;
		break;
	
	case 0x0304:
		p_dcpBuf = &usb_gpcdc_StringDescriptor4[0];
		dcpBufCnt = 22;
		break;
	
	case 0x0305:
		p_dcpBuf = &usb_gpcdc_StringDescriptor5[0];
		dcpBufCnt = 32;
		break;
	
	case 0x0306:
		p_dcpBuf = &usb_gpcdc_StringDescriptor6[0];
		dcpBufCnt = 18;
		break;
	
	case 0x0307:
		p_dcpBuf = &usb_gpcdc_StringDescriptor7[0];
		dcpBufCnt = 18;
		break;
		
	default:
		p_dcpBuf = &usb_gpcdc_StringDescriptor7[0];
		dcpBufCnt = 18;
		break;
	}
	
	WriteControlINPacket();
}





void GetDescriptor(void)
{
	switch(bmRequestType)
	{
	case 0x80:
		{
			switch(wValue / 256)
			{
			case 0x01: // DEVICE.
				GetDeviceDescriptor();
				break;
			
			case 0x02: // CONFIGURATION.
			    GetConfigurationDescriptor();
				break; 
			
			case 0x03: // STRING.
				GetStringDescriptor();
				break;
			
			case 0x04: // INTERFACE.
				//GetInterfaceDescriptor();
			
			case 0x05: // ENDPOINT.
				//GetEndpointDescriptor();

			case 0x06: 
				
			case 0x07:
				
			case 0x08:
				
			default:
				
				break;
			}
			
		}	
		break;

	default:
		// STALL.
		USB0.DCPCTR.BIT.PID = 2; 
		break;
	}		
}
