#include "iodefine.h"



/**********************************************************************
* Outline 		: ProcessGetDescriptor
* Description 	: Process a GET_DESCRIPTOR setup packet.
*				  Supports Device, Configuration and String requests.
*
* Argument  	: _pNumBytes: (OUT)If this layer can handle this then
*				 			this will be set with the size of the descriptor.
*				  _ppDescriptor: (OUT)If this layer can handle this then
*				 			this will be set to point to the descriptor.
*			
* Return value  : Error code - USB_ERR_OK if supported.
**********************************************************************/
static USB_ERR ProcessGetDescriptor(uint16_t* _pNumBytes, const uint8_t** _ppDescriptor)
{
	USB_ERR err = USB_ERR_OK;
	
	/*The wValue field of the setup packet,
	specifies the descriptor type in the high byte
	and the descriptor index in the low byte.*/
	
	/*Get Descriptor Type*/
	uint8_t DescriptorType = (uint8_t)((g_oSetupPacket.wValue >> 8) & 0x00FF);
	
	switch(DescriptorType)
	{
		case DEVICE:
		{
//			DEBUG_MSG_MID(("USBCORE: Device Descriptor\r\n"));
		
			*_pNumBytes = g_Descriptors.Device.NumBytes;
			*_ppDescriptor = g_Descriptors.Device.pucBuf;	
			break;
		}
		case CONFIGURATION:
		{
//			DEBUG_MSG_MID(("USBCORE: Config Descriptor\r\n"));
			
			*_pNumBytes = g_Descriptors.Config.NumBytes;
			*_ppDescriptor = g_Descriptors.Config.pucBuf;
			break;
		}
		case STRING:
		{
//			DEBUG_MSG_MID(("USBCORE: String Descriptor\r\n"));
			err = ProcessGetDescriptorString(_pNumBytes, _ppDescriptor);
			break;
		}
		case INTERFACE:
			/*Interface descriptor is avalable through Configuration descriptor only.*/		
		case ENDPOINT:
			/*Endpoint descriptor is avalable through Configuration descriptor only.*/
		case DEVICE_QUALIFIER:
			/*Note1: May get a DEVICE_QUALIFIER request - but OK not to handle
			it as we don't support different configurations for high and full speed*/
		default:
		{
			/*Unknown descriptor request*/
			
			/*Note2: HID uses this standard GET_DESCRIPTOR to get
			class specific descriptors.*/
//			DEBUG_MSG_MID(("USBCORE: Unhandled GetDescriptor Type %d\r\n", DescriptorType));
			
			*_pNumBytes = 0;
			*_ppDescriptor = NULL;
			err = USB_ERR_UNKNOWN_REQUEST;
		}
	}
	
	return err;
}
/***********************************************************************************
End of function ProcessGetDescriptor
***********************************************************************************/

/**********************************************************************
* Outline 		: ProcessGetConfiguration
* Description 	: Process a Get Configuration USB request,
* Argument  	: _pNumBytes: (OUT)If this layer can handle this then
*				 			this will be set with the size of the data.
*				 			(If there is no data stage then this will be set to zero)
*				  _ppBuffer: (OUT)If this layer can handle this then
*				 			this will be set to point to the data(IN) or a buffer(OUT).
*				 			(If there is a data stage for this packet).
* Return value  : Error code. - USB_ERR_OK if supported.
**********************************************************************/
static USB_ERR ProcessGetConfiguration(uint16_t* _pNumBytes, const uint8_t**_ppBuffer)
{
	USB_ERR err = USB_ERR_OK;
	
	/*This assumes the device is in the configured state.*/

//	DEBUG_MSG_MID(("USBCORE: GetConfiguration: %d \r\n", g_ConfigurationValue));
	
	/*There is a 1 byte data stage for this request*/
	*_pNumBytes = 1;
	*_ppBuffer = &g_ConfigurationValue;
	
	return err;
}
/***********************************************************************************
End of function ProcessGetConfiguration
***********************************************************************************/



/**********************************************************************
* Outline 		: ProcessStandardSetupPacket 
* Description 	: Checks to see if this standard setup packet can be
*				  handled by this layer.
*				  Handles GET_DESCRIPTOR.
* Argument  	: _pNumBytes: (OUT)If this layer can handle this then
*				 			this will be set with the size of the data.
*				 			(If there is no data stage then this will be set to zero)
*				  _ppBuffer: (OUT)If this layer can handle this then
*				 			this will be set to point to the data(IN) or a buffer(OUT).
*				 			(If there is a data stage for this packet).
* Return value  : Error code. - USB_ERR_OK if supported.
**********************************************************************/

static USB_ERR ProcessStandardSetupPacket(uint16_t* _pNumBytes, uint8_t** _ppBuffer)
{
	USB_ERR err;
	
	/*NOTE: SET_ADDRESS is supported by the HW.*/

	switch(g_oSetupPacket.bRequest)
	{
		case GET_DESCRIPTOR:
		{
			err = ProcessGetDescriptor(_pNumBytes, (const uint8_t**)_ppBuffer);
			break;
		}
		case GET_CONFIGURATION:
		{
			err = ProcessGetConfiguration(_pNumBytes, (const uint8_t**)_ppBuffer);
			break;
		}
		case SET_CONFIGURATION:
		{
			err = ProcessSetConfiguration(_pNumBytes, (const uint8_t**)_ppBuffer);
			break;
		}
		case SET_FEATURE:
		{
			err = ProcessSetFeature(_pNumBytes, (const uint8_t**)_ppBuffer);
			break;
		}
		case CLEAR_FEATURE:
		{
			err = ProcessClearFeature(_pNumBytes, (const uint8_t**)_ppBuffer);
			break;
		}
		case GET_STATUS:
		{
			err = ProcessGetStatus(_pNumBytes, (const uint8_t**)_ppBuffer);
			break;
		}
		default:
		{
			/*Unsupported request*/
			err = USB_ERR_UNKNOWN_REQUEST;
		}
	}
	
	return err;
}
/***********************************************************************************
End of function ProcessStandardSetupPacket
***********************************************************************************/