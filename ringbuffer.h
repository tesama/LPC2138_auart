//---------------------------------------------------------------------
//
//	Program written by Sathit Wanitchaikit for
//		FIBO 2008/March 10
//	Copyright Revenger, 2008. All rights reserved
//
//---------------------------------------------------------------------

#ifndef __RING_BUFFER_H
#define __RING_BUFFER_H


//------------------------------------------
//
// INCLUDES
//
//------------------------------------------

#include <stdint.h>

//------------------------------------------
//
// DEFINES
//
//------------------------------------------

#define RingBuffer_SUCCESS	(int32_t)0
#define RingBuffer_FAIL			(int32_t)-1


//
// Change the following line to adjust the buffer size
//
#define RingBuffer_BUFF_SIZE	(uint8_t)32

typedef struct
{
	uint8_t buff[RingBuffer_BUFF_SIZE];
	uint8_t *pBegin;
	uint8_t *pEnd;
	uint8_t counts;
} RingBuffer_t;


//------------------------------------------
//
// EXTERNAL LINKED VARIABLES
//
//------------------------------------------

//------------------------------------------
//
// STATIC VARIABLES
//
//------------------------------------------


//------------------------------------------
//
// FUNCTION PROTOTYPES
//
//------------------------------------------

// clear buffer
void RingBuffer_clearBuffer(RingBuffer_t *pBuffer);

// push a single data to the buffer
//
//	RETURN	RingBuffer_SUCCESS	if sucess to push the data
//					RingBuffer_FAIL,	otherwise
int32_t RingBuffer_push(RingBuffer_t *pBuffer, uint8_t data);

// push an array of data to buffer
//
//	RETURN	RingBuffer_SUCCESS	if sucess to push the data
//					RingBuffer_FAIL,	otherwise
int32_t RingBuffer_pushArray(RingBuffer_t *pBuffer, uint8_t *pData, uint8_t nDatas);

// pop a single data from the buffer
//
//	RETURN	RingBuffer_SUCCESS	if sucess to pop the data
//					RingBuffer_FAIL,	otherwise
int32_t RingBuffer_pop(RingBuffer_t *pBuffer, uint8_t *data_ret);


#endif

//---------------------------------------------------------------------
//
//	EOF
//
//---------------------------------------------------------------------
