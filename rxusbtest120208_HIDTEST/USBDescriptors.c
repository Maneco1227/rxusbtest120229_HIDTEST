//
// USB Descriptors.
//

/* Standard Device Descriptor */
unsigned short usb_gpcdc_DeviceDescriptorLength = 18;
unsigned char  usb_gpcdc_DeviceDescriptor[] =
{
	0x12,
	0x01,
	0x00, 0x01,
	0x00,
	0x00,
	0x00,
	0x40,
	0xCB, 0xAB,
	0x01, 0x00,
	0x00, 0x01,
	0x01,
	0x02,
	0x03,
	0x01
};


// Configuration Descriptor.
unsigned short usb_gpcdc_ConfigrationDescriptorLength = 34;
unsigned char  usb_gpcdc_ConfigrationDescriptor[] =
{
	0x09,
	0x02,
	34, 0x00,
	0x01,
	0x01,
	0x00,
	0xA0,
	0x32,

	0x09,
	0x04,
	0x00,
	0x00,
	0x01,
	0x03,
	0x01,
	0x01,
	0x00,
	
	0x09,
	0x21,
	0x01, 0x01,
	0x00,
	0x01,
	0x22,
	0x3F, 0x00,

	0x07,
	0x05,
	0x81, // IN. EP1.
	0x02, // BULK.
	0x40, 0x00,
	0xFF,

	//*********************
	// End of Descriptor.
	//*********************
	0
};


unsigned char usb_gpcdc_StringDescriptor0[] =
{	
	0x04, 0x03, 0x09, 0x04
};

unsigned char usb_gpcdc_StringDescriptor1[] =
{
	/* Length of this descriptor*/
	16,
	/* Descriptor Type = STRING */
	0x03,
	/* Descriptor Text (unicode) */
	'R', 0x00, 'E', 0x00, 'N', 0x00, 'E', 0x00,
	'S', 0x00, 'A', 0x00, 'S', 0x00
};

unsigned char usb_gpcdc_StringDescriptor2[] =
{
	/* Length of this descriptor*/
	44,
	/* Descriptor Type = STRING */
	0x03,
	/* Descriptor Text (unicode) */
	'H', 0x00, 'I', 0x00, 'D', 0x00, ' ', 0x00,
	'U', 0x00, 'S', 0x00, 'B', 0x00, ' ', 0x00,
	'D', 0x00, 'e', 0x00, 'm', 0x00, 'o', 0x00,
	'n', 0x00, 's', 0x00, 't', 0x00, 'r', 0x00,
	'a', 0x00, 't', 0x00, 'i', 0x00, 'o', 0x00,
	'n', 0x00
};

unsigned char usb_gpcdc_StringDescriptor3[] =
{	
	/* Length of this descriptor*/
	8,
	/* Descriptor Type = STRING */
	0x03,
	/* Descriptor Text (unicode) */
	'1', 0x00, '.', 0x00, '1', 0x00
};

unsigned char usb_gpcdc_StringDescriptor4[] =
{	/* iConfiguration */
	22,		/*	0:bLength */
	0x03,	/*	1:bDescriptorType */
	'F', 0x00,	/*	2:wLANGID[0] */
	'u', 0x00,
	'l', 0x00,
	'l', 0x00,
	'-', 0x00,
	'S', 0x00,
	'p', 0x00,
	'e', 0x00,
	'e', 0x00,
	'd', 0x00
};

unsigned char usb_gpcdc_StringDescriptor5[] =
{	/* iInterface */
	32,		/*	0:bLength */
	0x03,	/*	1:bDescriptorType */
	'R', 0x00,
	'X', 0x00,
	'6', 0x00,
	'2', 0x00,
	' ', 0x00,
	'U', 0x00,
	'S', 0x00,
	'B', 0x00,
	' ', 0x00,
	'S', 0x00,
	'a', 0x00,
	'm', 0x00,
	'p', 0x00,
	'l', 0x00,
	'e', 0x00,
};

unsigned char usb_gpcdc_StringDescriptor6[] =
{	/* iSerialNumber */
	18,		/*	0:bLength */
	0x03,	/*	1:bDescriptorType */
	'0', 0x00,
	'0', 0x00,
	'0', 0x00,
	'0', 0x00,
	'0', 0x00,
	'0', 0x00,
	'0', 0x00,
	'1', 0x00,
};

unsigned char usb_gpcdc_StringDescriptor7[] =
{	/* iSerialNumber */
	18,		/*	0:bLength */
	0x03,	/*	1:bDescriptorType */
	'0', 0x00,
	'0', 0x00,
	'0', 0x00,
	'0', 0x00,
	'0', 0x00,
	'0', 0x00,
	'0', 0x00,
	'1', 0x00,
};

