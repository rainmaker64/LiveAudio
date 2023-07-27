#include "mbed.h"
#include "flash.h"

FlashIAP flash;

/*
 *	Description:
 *		Initialize Flash IAP instance. It shall be called once after creating Flash Instance
*/
int Flash_init()
{
	return flash.init();
}

/*
 *	Description:
 *		Erase the specific Flash Sector
 *	address - sector address
 *  size - size of bytes to be erased. Shall be multiple of sector size.
*/
int Flash_erase(uint64_t address, uint64_t size)
{
	return flash.erase(address, size);
}


/*
 *	Description:
 *		Write Data into Flash Region
 *  psrc - pointer to source buffer
 *  addess - destination address in flash
 *  ndat - size of bytes to be written
*/
int Flash_write(uint8_t* psrc, uint64_t address, uint64_t ndat)
{
	return flash.program(psrc, address, ndat);
}

/*
 *	Description:
 *		Read data from Flash Memory
 *  pbuf - pointer of buffer
 *  addess - destination address in flash
 *  ndat - size of bytes to be read
*/
int Flash_read(uint8_t* pbuf, uint64_t address, uint64_t ndat)
{
	uint8_t* psrc;
	int ret = 0;

	if (pbuf != NULL)
	{
		psrc = (uint8_t*)address;

		while (ndat--)
		{
			*pbuf++ = *psrc++;
		}

		ret = 1;
	}

	return ret;
}

/*
 *	Description:
 *		Deinitialize Flash IAP instance.
*/
int Flash_deinit()
{
	return flash.deinit();
}
