//---------------------------------------------------------------------
//
//	Program written by Sathit Wanitchaikit for
//		Aiyarafun Co., Ltd 2016/June 26th
//	Copyright Aiyarafun Co., Ltd, 2016. All rights reserved
//
//---------------------------------------------------------------------
#ifndef ___auart_h
#define ___auart_h


//
// NOTE:
//	This moudle is just an example how to implement UART for NXP LPC2138
//	All functions are hard code to use with UART0 only
//


//---------------------------------------------------------------------
//
//	INCLUDES
//
//---------------------------------------------------------------------

#include <LPC213x.h>
#include <stdint.h>
#include <stdbool.h>

//---------------------------------------------------------------------
//
//	DEFINES
//
//---------------------------------------------------------------------


//---------------------------------------------------------------------
//
//	GLOBALS
//
//---------------------------------------------------------------------

//---------------------------------------------------------------------
//
// INTERFACE FUNCTION PROTOTYPES
//
//---------------------------------------------------------------------

// initialize UART
//	return 0, no error
//				-1, otherwise
int32_t AUART_init(uint32_t pclk, uint32_t baudRate_bps, uint8_t interruptLevel);

// get number of aviable data for reading from serial port
int32_t AUART_get_recv_available(void);

// read one byte of the incoming data buffer
//	RETURN 0, if there is at least one data in the receiver buffer
//					-1, otherwise
int32_t AUART_read(uint8_t *pData_ret);

// read certain numbers of bytes for the incoming data buffer
//	RETURN number of successful read bytes
//					-1, when there is no data in the buffer
int32_t AUART_read_bytes(uint8_t numBytes, uint8_t* pData_ret);

// write one byte of data to the serial port
void AUART_write(uint8_t data);

// write bytes to the serial port;
void AUART_write_bytes(uint8_t numBytes, uint8_t* pData, bool noWait);



#endif
//---------------------------------------------------------------------
//
//	EOF
//
//---------------------------------------------------------------------
