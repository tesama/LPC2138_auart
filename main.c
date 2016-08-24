//---------------------------------------------------------------------
//
//	Program written by Sathit Wanitchaikit for
//		Aiyarafun Co., Ltd 2016/June 26th
//	Copyright Aiyarafun Co., Ltd, 2016. All rights reserved
//
//---------------------------------------------------------------------


//---------------------------------------------------------------------
//	INCLUDES
//---------------------------------------------------------------------

#include <LPC213x.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "auart.h"

//---------------------------------------------------------------------
//	DEFINES
//---------------------------------------------------------------------

#define PCLK	58982400
#define INTERRUPT_PRIORITY_UART0	5


//---------------------------------------------------------------------
//	GLOBALS
//---------------------------------------------------------------------

char gRawMessage[66];
char gMessage[33];
char gPrintfStr[64];
uint8_t buffer[33];

uint16_t gCounts=0;


//---------------------------------------------------------------------
//	HELPER FUNCTIONS
//---------------------------------------------------------------------

// ---------------------------------------
void delay_ms(uint32_t count)
{
	uint32_t i,j;
	for( i=0;i<count;i++)
		for( j=0;j<6664;j++);
}


// ---------------------------------------
void init()
{
	// for GPIO
	PINSEL2 = 0x00000000;
	IODIR1  = 0xFFFF0000;
	IOCLR1 	= 0xFFFF0000;
	
	// for UART0
	AUART_init(PCLK, 115200, INTERRUPT_PRIORITY_UART0);
}

// ---------------------------------------
void test_01()
{
	int32_t numReadBytes;
	char* carriageReturnPtr;
	uint32_t numCopyingBytes;
	
	//
	// Check the data from serial port
	//
	numReadBytes = AUART_read_bytes(AUART_get_recv_available(), buffer);
	if (numReadBytes > 0)
	{
		buffer[numReadBytes] = 0;
		strcat(gRawMessage, (char*)buffer);
	}
	
	//
	// Check end of message character (in this case it is CR)
	//	Then send out all completed messages.
	//
	while(true)
	{
		carriageReturnPtr = strchr(gRawMessage, '\r');
		if (carriageReturnPtr == NULL)
			// no any CR in the raw message.
			// so keep the them for next time processing
			break;
		
		// split the message out and send it to the serial port
		numCopyingBytes = (uint32_t)(carriageReturnPtr-gRawMessage);
		strncpy(gMessage, gRawMessage, numCopyingBytes);
		gMessage[numCopyingBytes] = 0;
		printf("we got: %s\r\n", gMessage);
		
		// eliminate the data which was splitted out from the raw message
		strcpy(gRawMessage, &gRawMessage[numCopyingBytes+1]);
	}
	
	//
	// do LED blinking
	//
	if (IOPIN1 == 0xFFFF0000)
		IOCLR1 	= 0xFFFF0000;
	else
		IOSET1 	= 0xFFFF0000;
	
	delay_ms(1000);
}


// ---------------------------------------
void test_02()
{
	sprintf(gPrintfStr, "gCounts = %d\r\n", gCounts);
	AUART_write_bytes(strlen(gPrintfStr), (uint8_t*)gPrintfStr, true);
	gCounts++;
	delay_ms(1000);
}


// ---------------------------------------
void test_03()
{
		int32_t numReadBytes;
	char* carriageReturnPtr;
	uint32_t numCopyingBytes;
	
	//
	// Check the data from serial port
	//
	numReadBytes = AUART_read_bytes(AUART_get_recv_available(), buffer);
	if (numReadBytes > 0)
	{
		buffer[numReadBytes] = 0;
		strcat(gRawMessage, (char*)buffer);
	}
	
	//
	// Check end of message character (in this case it is CR)
	//	Then send out all completed messages.
	//
	while(true)
	{
		carriageReturnPtr = strchr(gRawMessage, '\r');
		if (carriageReturnPtr == NULL)
			// no any CR in the raw message.
			// so keep the them for next time processing
			break;
		
		// split the message out and send it to the serial port
		numCopyingBytes = (uint32_t)(carriageReturnPtr-gRawMessage);
		strncpy(gMessage, gRawMessage, numCopyingBytes);
		gMessage[numCopyingBytes] = 0;
		sprintf(gPrintfStr, "we got: %s\r\n", gMessage);
		AUART_write_bytes(strlen(gPrintfStr), (uint8_t*)gPrintfStr, true);
		
		// eliminate the data which was splitted out from the raw message
		strcpy(gRawMessage, &gRawMessage[numCopyingBytes+1]);
	}
	
	//
	// do LED blinking
	//
	if (IOPIN1 == 0xFFFF0000)
		IOCLR1 	= 0xFFFF0000;
	else
		IOSET1 	= 0xFFFF0000;
	
	delay_ms(1000);
}



//---------------------------------------------------------------------
//	MAIN FUNCTION
//---------------------------------------------------------------------

int32_t main(void)
{
	// initialize GPIO and UART0
	init();
	
	//
	// main loop
	//
	gRawMessage[0] = 0;
	gMessage[0] = 0;
	printf("Hello, Saranchana ^w^/\r\n");
	while(true)
	{
		//test_01();
		//test_02();
		test_03();
	}
	
	return 0;
}
