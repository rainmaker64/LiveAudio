
#ifndef DRVL_FLASH_H
#define	DRVL_FLASH_H

#ifdef __cplusplus
extern "C" {
#endif

#define		SECTOR0123_SIZE			0x00004000		// 16KB
#define		SECTOR4_SIZE			0x00010000		// 64KB
#define		SECTOR567_SIZE			0x00020000		// 128KB

// SECTOR 0
#define		FLASH_SECTOR0_START		0x08000000
#define		FLASH_SECTOR0_END		(FLASH_SECTOR0_START + SECTOR0123_SIZE - 1)

// SECTOR 1
#define		FLASH_SECTOR1_START		(FLASH_SECTOR0_END + 1)
#define		FLASH_SECTOR1_END		(FLASH_SECTOR1_START + SECTOR0123_SIZE - 1)

// SECTOR 2
#define		FLASH_SECTOR2_START		(FLASH_SECTOR1_END + 1)
#define		FLASH_SECTOR2_END		(FLASH_SECTOR2_START + SECTOR0123_SIZE - 1)

// SECTOR 3
#define		FLASH_SECTOR3_START		(FLASH_SECTOR2_END + 1)
#define		FLASH_SECTOR3_END		(FLASH_SECTOR3_START + SECTOR0123_SIZE - 1)

// SECTOR 4
#define		FLASH_SECTOR4_START		(FLASH_SECTOR3_END + 1)
#define		FLASH_SECTOR4_END		(FLASH_SECTOR4_START + SECTOR04_SIZE - 1)

// SECTOR 5
#define		FLASH_SECTOR5_START		(FLASH_SECTOR3_END + 1)
#define		FLASH_SECTOR5_END		(FLASH_SECTOR4_START + SECTOR567_SIZE - 1)

// SECTOR 6
#define		FLASH_SECTOR6_START		(FLASH_SECTOR3_END + 1)
#define		FLASH_SECTOR6_END		(FLASH_SECTOR4_START + SECTOR567_SIZE - 1)

// SECTOR 7
#define		FLASH_SECTOR7_START		(FLASH_SECTOR3_END + 1)
#define		FLASH_SECTOR7_END		(FLASH_SECTOR4_START + SECTOR567_SIZE - 1)


/// Memopry Map
#define		ROM_FIRMWARE_START		FLASH_SECTOR0_START
#define		ROM_WAVE_START			FLASH_SECTOR3_START

#define		ROM_FIRMWARE_SIZE		(FLASH_SECTOR3_START - 1)		// 48KB
#define		ROM_WAVE_SIZE			(ROM_WAVE_START - FLASH_SECTOR7_END - 1)



/*
*	Description:
*		Erase the specific Flash Sector
*	address - sector address
*  size - size of bytes to be erased. Shall be multiple of sector size.
*/
int Flash_erase(uint64_t address, uint64_t size);

/*
	*	Description:
	*		Write Data into Flash Region
	*  psrc - pointer to source buffer
	*  addess - destination address in flash
	*  ndat - size of bytes to be written
	*/
int Flash_write(uint8_t* psrc, uint64_t address, uint64_t ndat);

/*
	*	Description:
	*		Read data from Flash Memory
	*  pbuf - pointer of buffer
	*  addess - destination address in flash
	*  ndat - size of bytes to be read
	*/
int Flash_read(uint8_t* pbuf, uint64_t address, uint64_t ndat);

/*
	*	Description:
	*		Initialize Flash IAP instance. It shall be called once after creating Flash Instance
	*/
int Flash_init();

/*
	*	Description:
	*		Deinitialize Flash IAP instance.
	*/
int Flash_deinit();

#ifdef __cplusplus
}
#endif

#endif