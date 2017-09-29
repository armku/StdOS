/******************************************************************************/
/***************************** Include Files **********************************/
/******************************************************************************/
#include "AD7124.h"

/* Error codes */
#define INVALID_VAL -1 /* Invalid argument */
#define COMM_ERR    -2 /* Communication error on receive */
#define TIMEOUT     -3 /* A timeout has occured */

/***************************************************************************//**
* @brief Reads the value of the specified register without checking if the
*        device is ready to accept user requests.
*
* @param device - The handler of the instance of the driver.
* @param pReg - Pointer to the register structure holding info about the
*               register to be read. The read value is stored inside the
*               register structure.
*
* @return Returns 0 for success or negative error code.
*******************************************************************************/
int32_t AD7124_NoCheckReadRegister(ad7124_device *device, ad7124_st_reg* pReg)
{
	int32_t ret       = 0;
	uint8_t buffer[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	uint8_t i         = 0;
	uint8_t check8    = 0;
	uint8_t msgBuf[8] = {0, 0, 0, 0, 0, 0, 0, 0};

	if(!device || !pReg)
		return INVALID_VAL;

	/* Build the Command word */
	buffer[0] = AD7124_COMM_REG_WEN | AD7124_COMM_REG_RD |
	  		AD7124_COMM_REG_RA(pReg->addr);

	/* Read data from the device */
	ret = SPI_Read(device->slave_select_id,
			buffer,
			((device->useCRC != AD7124_DISABLE_CRC) ? pReg->size + 1
							: pReg->size) + 1);
	if(ret < 0)
		return ret;

	/* Check the CRC */
	if(device->useCRC == AD7124_USE_CRC)
	{
		msgBuf[0] = AD7124_COMM_REG_WEN | AD7124_COMM_REG_RD | 
		  		AD7124_COMM_REG_RA(pReg->addr);
		for(i = 1; i < pReg->size + 2; ++i)
		{
			msgBuf[i] = buffer[i];
		}
		check8 = AD7124_ComputeCRC8(msgBuf, pReg->size + 2);
	}

	if(check8 != 0)
	{
		/* ReadRegister checksum failed. */
		return COMM_ERR;
	}

	/* Build the result */
	pReg->value = 0;
	for(i = 1; i < pReg->size + 1; i++)
	{
		pReg->value <<= 8;
		pReg->value += buffer[i];
	}

	return ret;
}

/***************************************************************************//**
* @brief Writes the value of the specified register without checking if the
*        device is ready to accept user requests.
*
* @param device - The handler of the instance of the driver.
* @param reg - Register structure holding info about the register to be written
*
* @return Returns 0 for success or negative error code.
*******************************************************************************/
int32_t AD7124_NoCheckWriteRegister(ad7124_device *device, ad7124_st_reg reg)
{
	int32_t ret      = 0;
	int32_t regValue = 0;
	uint8_t wrBuf[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	uint8_t i        = 0;
	uint8_t crc8     = 0;

	if(!device)
		return INVALID_VAL;

	/* Build the Command word */
	wrBuf[0] = AD7124_COMM_REG_WEN | AD7124_COMM_REG_WR |
	  		AD7124_COMM_REG_RA(reg.addr);

	/* Fill the write buffer */
	regValue = reg.value;
	for(i = 0; i < reg.size; i++)
	{
		wrBuf[reg.size - i] = regValue & 0xFF;
		regValue >>= 8;
	}

	/* Compute the CRC */
	if(device->useCRC != AD7124_DISABLE_CRC)
	{
		crc8 = AD7124_ComputeCRC8(wrBuf, reg.size + 1);
		wrBuf[reg.size + 1] = crc8;
	}

	/* Write data to the device */
	ret = SPI_Write(device->slave_select_id,
			wrBuf,
			(device->useCRC != AD7124_DISABLE_CRC) ? reg.size + 2
							: reg.size + 1);

	return ret;
}

/***************************************************************************//**
* @brief Reads the value of the specified register only when the device is ready
*        to accept user requests. If the device ready flag is deactivated the
*        read operation will be executed without checking the device state.
*
* @param device - The handler of the instance of the driver.
* @param pReg - Pointer to the register structure holding info about the
*               register to be read. The read value is stored inside the
*               register structure.
*
* @return Returns 0 for success or negative error code.
*******************************************************************************/
int32_t AD7124_ReadRegister(ad7124_device *device, ad7124_st_reg* pReg)
{
	int32_t ret;
	
	if (pReg->addr != AD7124_ERR_REG && device->check_ready)
	{
		ret = AD7124_WaitForSpiReady(device, device->spi_rdy_poll_cnt);
		if (ret < 0)
			return ret;
	}
	ret = AD7124_NoCheckReadRegister(device, pReg);
	
	return ret;
}

/***************************************************************************//**
* @brief Writes the value of the specified register only when the device is
*        ready to accept user requests. If the device ready flag is deactivated
*        the write operation will be executed without checking the device state.
*
* @param device - The handler of the instance of the driver.
* @param reg - Register structure holding info about the register to be written
*
* @return Returns 0 for success or negative error code.
*******************************************************************************/
int32_t AD7124_WriteRegister(ad7124_device *device, ad7124_st_reg pReg)
{
	int32_t ret;
	
	if (device->check_ready)
	{  
		ret = AD7124_WaitForSpiReady(device, device->spi_rdy_poll_cnt);
		if (ret < 0)
			return ret;
	}
	ret = AD7124_NoCheckWriteRegister(device, pReg);
	
	return ret;
}

/***************************************************************************//**
* @brief Resets the device.
*
* @param device - The handler of the instance of the driver.
*
* @return Returns 0 for success or negative error code.
*******************************************************************************/
int32_t AD7124_Reset(ad7124_device *device)
{
	int32_t ret = 0;
	uint8_t wrBuf[8] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

	if(!device)
		return INVALID_VAL;

	ret = SPI_Write(device->slave_select_id, wrBuf, 8);

	/* Wait for the reset to complete */
	ret = AD7124_WaitToPowerOn(device, device->spi_rdy_poll_cnt);

	return ret;
}

/***************************************************************************//**
* @brief Waits until the device can accept read and write user actions.
*
* @param device - The handler of the instance of the driver.
* @param timeout - Count representing the number of polls to be done until the
*                  function returns.
*
* @return Returns 0 for success or negative error code.
*******************************************************************************/
int32_t AD7124_WaitForSpiReady(ad7124_device *device, uint32_t timeout)
{
	ad7124_st_reg *regs;
	int32_t ret;
	int8_t ready = 0;

	if(!device)
		return INVALID_VAL;

	regs = device->regs;

	while(!ready && --timeout)
	{
		/* Read the value of the Error Register */
		ret = AD7124_ReadRegister(device, &regs[AD7124_Error]);
		if(ret < 0)
			return ret;

		/* Check the SPI IGNORE Error bit in the Error Register */
		ready = (regs[AD7124_Error].value &
			 	AD7124_ERR_REG_SPI_IGNORE_ERR) == 0;
	}

	return timeout ? 0 : TIMEOUT;
}

/***************************************************************************//**
* @brief Waits until the device finishes the power-on reset operation.
*
* @param device - The handler of the instance of the driver.
* @param timeout - Count representing the number of polls to be done until the
*                  function returns.
*
* @return Returns 0 for success or negative error code.
*******************************************************************************/
int32_t AD7124_WaitToPowerOn(ad7124_device *device, uint32_t timeout)
{
	ad7124_st_reg *regs;
	int32_t ret;
	int8_t powered_on = 0;

	if(!device)
		return INVALID_VAL;

	regs = device->regs;

	while(!powered_on && timeout--)
	{
		ret = AD7124_ReadRegister(device, &regs[AD7124_Status]);
		if(ret < 0)
			return ret;

		/* Check the POR_FLAG bit in the Status Register */
		powered_on = (regs[AD7124_Status].value &
				AD7124_STATUS_REG_POR_FLAG) == 0;
	}

	return (timeout || powered_on) ? 0 : TIMEOUT;
}

/***************************************************************************//**
* @brief Waits until a new conversion result is available.
*
* @param device - The handler of the instance of the driver.
* @param timeout - Count representing the number of polls to be done until the
*                  function returns if no new data is available.
*
* @return Returns 0 for success or negative error code.
*******************************************************************************/
int32_t AD7124_WaitForConvReady(ad7124_device *device, uint32_t timeout)
{
	ad7124_st_reg *regs;
	int32_t ret;
	int8_t ready = 0;

	if(!device)
		return INVALID_VAL;

	regs = device->regs;

	while(!ready && --timeout)
	{
		/* Read the value of the Status Register */
		ret = AD7124_ReadRegister(device, &regs[AD7124_Status]);
		if(ret < 0)
			return ret;

		/* Check the RDY bit in the Status Register */
		ready = (regs[AD7124_Status].value &
				AD7124_STATUS_REG_RDY) == 0;
	}

	return timeout ? 0 : TIMEOUT;
}

/***************************************************************************//**
* @brief Reads the conversion result from the device.
*
* @param device - The handler of the instance of the driver.
* @param pData - Pointer to store the read data.
*
* @return Returns 0 for success or negative error code.
*******************************************************************************/
int32_t AD7124_ReadData(ad7124_device *device, int32_t* pData)
{
	ad7124_st_reg *regs;
	int32_t ret;

	if(!device)
		return INVALID_VAL;

	regs = device->regs;

	/* Read the value of the Data Register */
	ret = AD7124_ReadRegister(device, &regs[AD7124_Data]);

	/* Get the read result */
	*pData = regs[AD7124_Data].value;

	return ret;
}

/***************************************************************************//**
* @brief Computes the CRC checksum for a data buffer.
*
* @param pBuf - Data buffer
* @param bufSize - Data buffer size in bytes
*
* @return Returns the computed CRC checksum.
*******************************************************************************/
uint8_t AD7124_ComputeCRC8(uint8_t * pBuf, uint8_t bufSize)
{
	uint8_t i   = 0;
	uint8_t crc = 0;

	while(bufSize)
	{
		for(i = 0x80; i != 0; i >>= 1)
		{
			if(((crc & 0x80) != 0) != ((*pBuf & i) != 0)) /* MSB of CRC register XOR input Bit from Data */
			{
				crc <<= 1;
				crc ^= AD7124_CRC8_POLYNOMIAL_REPRESENTATION;
			}
			else
			{
				crc <<= 1;
			}
		}
		pBuf++;
		bufSize--;
	}
	return crc;
}

/***************************************************************************//**
* @brief Updates the CRC settings.
*
* @param device - The handler of the instance of the driver.
*
* @return None.
*******************************************************************************/
void AD7124_UpdateCRCSetting(ad7124_device *device)
{
	ad7124_st_reg *regs;

	if(!device)
		return;

	regs = device->regs;
	
	/* Get CRC State. */
	if (regs[AD7124_Error_En].value & AD7124_ERREN_REG_SPI_CRC_ERR_EN)
	{
		device->useCRC = AD7124_USE_CRC;
	}
	else
	{
		device->useCRC = AD7124_DISABLE_CRC;
	}
}

/***************************************************************************//**
* @brief Updates the device SPI interface settings.
*
* @param device - The handler of the instance of the driver.
*
* @return None.
*******************************************************************************/
void AD7124_UpdateDevSpiSettings(ad7124_device *device)
{
	ad7124_st_reg *regs;

	if(!device)
		return;

	regs = device->regs;
	
	if (regs[AD7124_Error_En].value & AD7124_ERREN_REG_SPI_IGNORE_ERR_EN)
	{
		device->check_ready = 1;
	}
	else
	{
		device->check_ready = 0;
	}
}

/***************************************************************************//**
* @brief Initializes the AD7124.
*
* @param device - The handler of the instance of the driver.
* @param slave_select - The Slave Chip Select Id to be passed to the SPI calls.
* @param regs - The list of registers of the device (initialized or not) to be
*               added to the instance of the driver.
*
* @return Returns 0 for success or negative error code.
*******************************************************************************/
int32_t AD7124_Setup(ad7124_device *device, int slave_select,
			ad7124_st_reg *regs)
{
	int32_t ret;
	enum ad7124_registers regNr;

	if(!device || !regs)
		return INVALID_VAL;

	device->regs = regs;
	device->slave_select_id = slave_select;
	device->spi_rdy_poll_cnt = 25000;

	/* Initialize the SPI communication. */
	ret = SPI_Init(0, 1000000, 1, 0);
	if (ret < 0)
		return ret;

	/*  Reset the device interface.*/
	ret = AD7124_Reset(device);
	if (ret < 0)
		return ret;
	
	/* Update the device structure with power-on/reset settings */
	device->check_ready = 1;

	/* Initialize registers AD7124_ADC_Control through AD7124_Filter_7. */
	for(regNr = AD7124_Status; (regNr < AD7124_Offset_0) && !(ret < 0);	regNr=(ad7124_registers)(regNr+1))
	{
		if (regs[regNr].rw == AD7124_RW)
		{
			ret = AD7124_WriteRegister(device, regs[regNr]);
			if (ret < 0)
		  		break;
		}
		
		/* Get CRC State and device SPI interface settings */
		if (regNr == AD7124_Error_En)
		{
			AD7124_UpdateCRCSetting(device);
			AD7124_UpdateDevSpiSettings(device);
		}
	}

	return ret;
}
#include "Drivers\AD7124_8.h"
extern SpiSoft spi128;
/***************************************************************************//**
 * @brief Initializes the SPI communication peripheral.
 *
 * @param lsbFirst - Transfer format (0 or 1).
 *                   Example: 0x0 - MSB first.
 *                            0x1 - LSB first.
 * @param clockFreq - SPI clock frequency (Hz).
 *                    Example: 1000 - SPI clock frequency is 1 kHz.
 * @param clockPol - SPI clock polarity (0 or 1).
 *                   Example: 0x0 - Idle state for clock is a low level; active
 *                                  state is a high level;
 *	                      0x1 - Idle state for clock is a high level; active
 *                                  state is a low level.
 * @param clockEdg - SPI clock edge (0 or 1).
 *                   Example: 0x0 - Serial output data changes on transition
 *                                  from idle clock state to active clock state;
 *                            0x1 - Serial output data changes on transition
 *                                  from active clock state to idle clock state.
 *
 * @return status - Result of the initialization procedure.
 *                  Example: 1 - if initialization was successful;
 *                           0 - if initialization was unsuccessful.
*******************************************************************************/
unsigned char SPI_Init(unsigned char lsbFirst,
                       unsigned long clockFreq,
                       unsigned char clockPol,
                       unsigned char clockEdg)
{
    /* Add your code here. */
	return 1;
}

/***************************************************************************//**
 * @brief Reads data from SPI.
 *
 * @param slaveDeviceId - The ID of the selected slave device.
 * @param data - Data represents the write buffer as an input parameter and the
 *               read buffer as an output parameter.
 * @param bytesNumber - Number of bytes to read.
 *
 * @return Number of read bytes.
*******************************************************************************/
unsigned char SPI_Read(unsigned char slaveDeviceId,
                       unsigned char* data,
                       unsigned char bytesNumber)
{
    /* Add your code here. */
	for(int i=0;i<bytesNumber;i++)
	{
		data[i]=spi128.Write(0XFF);
	}
	return bytesNumber;
}

/***************************************************************************//**
 * @brief Writes data to SPI.
 *
 * @param slaveDeviceId - The ID of the selected slave device.
 * @param data - Data represents the write buffer.
 * @param bytesNumber - Number of bytes to write.
 *
 * @return Number of written bytes.
*******************************************************************************/
unsigned char SPI_Write(unsigned char slaveDeviceId,
                        unsigned char* data,
                        unsigned char bytesNumber)
{
    /* Add your code here. */
	for(int i=0;i<bytesNumber;i++)
	{
		spi128.Write(data[i]);
	}
	return bytesNumber;
}
