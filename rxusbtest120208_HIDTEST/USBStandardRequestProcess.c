#include "iodefine.h"

//
// USB Standard Device Request Variables.
//
extern unsigned short bmRequestType;
extern unsigned short bRequest;
extern unsigned short wValue;
extern unsigned short wIndex;
extern unsigned short wLength;


void GetStatus(void)
{
	switch(bmRequestType)
	{
	case 0x80:
		break;
	case 0x81:
		break;
	case 0x82:
		break;
	default:
		break;
	}
}


void ClearFeature(void)
{
	switch(bmRequestType)
	{
	case 0x80:
		break;
	case 0x81:
		break;
	case 0x82:
		break;
	default:
		break;
	}
}


void SetFeature(void)
{
	switch(bmRequestType)
	{
	case 0x00:
		break;
	case 0x01:
		break;
	case 0x02:
		break;
	default:
		break;
	}
}


void SetAddress(void)
{
	switch(bmRequestType)
	{
	case 0x00:
		break;
	default:
		break;
	}		
}




void SetDescriptor(void)
{
	switch(bmRequestType)
	{
	case 0x00:
		break;
	default:
		break;
	}		
}


void GetConfiguration(void)
{
	switch(bmRequestType)
	{
	case 0x80:
		break;
	default:
		break;
	}		
}





void GetInterface(void)
{
	switch(bmRequestType)
	{
	case 0x81:
		break;
	default:
		break;
	}		
}


void SetInterface(void)
{
	switch(bmRequestType)
	{
	case 0x01:
		break;
	default:
		break;
	}	
}

void SynchFrame(void)
{	
	switch(bmRequestType)
	{
	case 0x82:
		break;
	default:
		break;
	}	
}






