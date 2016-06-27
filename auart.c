//---------------------------------------------------------------------
//
//	Program written by Sathit Wanitchaikit for
//		Aiyarafun Co., Ltd 2016/June 26th
//	Copyright Aiyarafun Co., Ltd, 2016. All rights reserved
//
//---------------------------------------------------------------------


//---------------------------------------------------------------------
//
//	INCLUDES
//
//---------------------------------------------------------------------

#include "auart.h"
#include "ringbuffer.h"

//---------------------------------------------------------------------
//	DEFINES AND INLINES
//---------------------------------------------------------------------

#define UART0_VIC	6

#define BITMASK_INTERRUPT_PENDING					0x00000001
#define BITMASK_INTERRUPT_IDENTIFICATION	0x0000000E

#define BITMASK_ENABLE_RBR_INTERRUPT		0x00000001
#define BITMASK_ENABLE_THRE_INTERRUPT		0x00000002

#define EVENT_RECEIVE_LINE_STATUS							0x06
#define EVENT_RECEIVE_DATA_AVAILABLE					0x04
#define EVENT_CHARACTER_TIME_OUT_INDICATOR		0x0C
#define EVENT_TRANSMIT_HOLDING_REGISTER_EMPTY	0x02

//---------------------------------------------------------------------
//	GLOBALS
//---------------------------------------------------------------------

RingBuffer_t gReceiverBuffer;

//---------------------------------------------------------------------
//	FILE SCOPE FUNCTIONS
//---------------------------------------------------------------------

//------------------------------------------
// INTERRUPT SERVICE ROUTINES
//------------------------------------------

void isr_uart0( void ) __irq
{
	//
	// try handle all pending interrupt
	//
	while ((U0IIR & BITMASK_INTERRUPT_PENDING) == 0)
	{
		switch( U0IIR & BITMASK_INTERRUPT_IDENTIFICATION )
		{
			case EVENT_RECEIVE_LINE_STATUS:
			{
				//
				// *** Please code the handling ***
				//

				// read LSR to clear interrupt
				U0LSR & 0xFF;
				break;
			}
			
			case EVENT_RECEIVE_DATA_AVAILABLE:
			case EVENT_CHARACTER_TIME_OUT_INDICATOR:
			{														 
				RingBuffer_push(&gReceiverBuffer, U0RBR);
				break;
			}
			
			case EVENT_TRANSMIT_HOLDING_REGISTER_EMPTY:
			{
				//
				// *** Please code the handling ***
				//
				
				break;
			}
			
			default:
				// unkown event, so do nothing
				break;
		}
	}

	// dummy write to signal end of interrupt
	VICVectAddr = 0;
}


//---------------------------------------------------------------------
//	FUNCTION BODIES
//---------------------------------------------------------------------

// ------------------------------------------
// funtion body for 'sendchar' of 'Retarget.c'
int sendchar( int ch )
{
	while ( !( U0LSR & 0x20 ) );
	return ( U0THR = ch );
}


// ------------------------------------------
// funtion body for 'sendchar' of 'Retarget.c'
int getkey()
{
	while( !( U0LSR & 0x01 ) );	
	return ( U0RBR );
}


// ------------------------------------------
// initialize UART
//	return 0, no error
//				-1, otherwise
int32_t AUART_init(uint32_t pclk, uint32_t baudRate_bps, uint8_t interruptLevel)
{
	//
	// Force the settings to be
	//	8 bits data
	//	1 stop bit
	//
	uint16_t u0dl;
	u0dl = (uint16_t)(pclk /(16*baudRate_bps));
	PINSEL0 |= 0x00000005;
	U0LCR = 0x83;
	U0DLL = u0dl & 0xFF;
	U0DLM = ( u0dl >> 8 );
	U0LCR &= 0x7F;
	
	//
	// set UART0 interrupt
	//
	if (interruptLevel > 16)
		interruptLevel = 16;
	
	if (*(&VICVectCntl0 + interruptLevel) != 0)
		// the slot is using by one of VICs
		return -1;
	
	// set the interrupt vector control and vector address
	*(&VICVectCntl0 + interruptLevel) = 0x20 | UART0_VIC;
	*(&VICVectAddr0 + interruptLevel) = (uint32_t)isr_uart0;
	
	// enable UART0 interrupt
	VICIntEnable |= ( 1 << UART0_VIC );
	U0IER |= ( BITMASK_ENABLE_RBR_INTERRUPT | BITMASK_ENABLE_THRE_INTERRUPT );
	
	
	// clear receiver buffer
	RingBuffer_clearBuffer(&gReceiverBuffer);
	
	return 0;
}


// ------------------------------------------
// get number of aviable data for reading from serial port
int32_t AUART_get_recv_available(void)
{
	return gReceiverBuffer.counts;
}


// ------------------------------------------
// read one byte of the incoming data buffer
//	RETURN 0, if there is at least one data in the receiver buffer
//					-1, otherwise
int32_t AUART_read(uint8_t *pData_ret)
{
	if (gReceiverBuffer.counts == 0)
		return -1;
	
	RingBuffer_pop(&gReceiverBuffer, pData_ret);
	return 0;
}


// ------------------------------------------
// read certain numbers of bytes for the incoming data buffer
//	RETURN number of successful read bytes
//					-1, when there is no data in the buffer
int32_t AUART_read_bytes(uint8_t numBytes, uint8_t* pData_ret)
{
	int32_t dataCounts;
	
	if ((gReceiverBuffer.counts == 0) || (numBytes == 0))
		return -1;
	
	dataCounts = 0;
	while((gReceiverBuffer.counts > 0)&&(dataCounts < numBytes))
	{
		RingBuffer_pop(&gReceiverBuffer, &pData_ret[dataCounts]);
		dataCounts++;
	}
	
	return dataCounts;
}


// ------------------------------------------
// write one byte of data to the serial port
void AUART_write(uint8_t data)
{
	sendchar(data);
}


// ------------------------------------------
// write bytes to the serial port;
void AUART_write_bytes(uint8_t numBytes, uint8_t* pData)
{
	while(numBytes > 0)
	{
		sendchar(*pData);
		pData++;
		numBytes--;
	}
}



//---------------------------------------------------------------------
//
//	EOF
//
//---------------------------------------------------------------------
